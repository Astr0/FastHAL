#ifndef FH_AVR_TIME_H_
#define FH_AVR_TIME_H_

/* 	8 bit TCNT _64 cs  	@ 16Mhz     @1Mhz	
	us per tick			4			64
	1M * 64 / 16M
	us per overflow		1024		16384
	us per tick * 256
*/

#include "../../std/std_types.hpp"
#include "timers.hpp"
#include "interrupts.hpp"
#include "../../fields/info.hpp"
#include "../../utils/functions.h"
#include "../../hal/time.hpp"

namespace fasthal{

	static constexpr auto cycles_per_us = (F_CPU / 1000000UL);
	static constexpr auto cycles_to_us(std::uint32_t cycles) { return cycles / cycles_per_us;}

	namespace details{
		struct avr_delay_time{
			static void delay_us(std::uint16_t us){
				static constexpr auto cpu_freq = F_CPU;
					// call = 4 cycles + 2 to 4 cycles to init us(2 for constant delay, 4 for variable)

					// calling avrlib's delay_us() function with low values (e.g. 1 or
					// 2 microseconds) gives delays longer than desired.
					//delay_us(us);
				if constexpr (cpu_freq >= 24000000L) {
					// for the 24 MHz clock for the aventurous ones, trying to overclock

					// zero delay fix
					if (!us) return; //  = 3 cycles, (4 when true)

					// the following loop takes a 1/6 of a microsecond (4 cycles)
					// per iteration, so execute it six times for each microsecond of
					// delay requested.
					us *= 6; // x6 us, = 7 cycles

					// account for the time taken in the preceeding commands.
					// we just burned 22 (24) cycles above, remove 5, (5*4=20)
					// us is at least 6 so we can substract 5
					us -= 5; //=2 cycles
				} else if constexpr(cpu_freq >= 20000000L) {
					// for the 20 MHz clock on rare Arduino boards

					// for a one-microsecond delay, simply return.  the overhead
					// of the function call takes 18 (20) cycles, which is 1us
					__asm__ __volatile__ (
						"nop" "\n\t"
						"nop" "\n\t"
						"nop" "\n\t"
						"nop"); //just waiting 4 cycles
					if (us <= 1) return; //  = 3 cycles, (4 when true)

					// the following loop takes a 1/5 of a microsecond (4 cycles)
					// per iteration, so execute it five times for each microsecond of
					// delay requested.
					us = (us << 2) + us; // x5 us, = 7 cycles

					// account for the time taken in the preceeding commands.
					// we just burned 26 (28) cycles above, remove 7, (7*4=28)
					// us is at least 10 so we can substract 7
					us -= 7; // 2 cycles
				} else if constexpr(cpu_freq >= 16000000L) {
					// for the 16 MHz clock on most Arduino boards

					// for a one-microsecond delay, simply return.  the overhead
					// of the function call takes 14 (16) cycles, which is 1us
					if (us <= 1) return; //  = 3 cycles, (4 when true)

					// the following loop takes 1/4 of a microsecond (4 cycles)
					// per iteration, so execute it four times for each microsecond of
					// delay requested.
					us <<= 2; // x4 us, = 4 cycles

					// account for the time taken in the preceeding commands.
					// we just burned 19 (21) cycles above, remove 5, (5*4=20)
					// us is at least 8 so we can substract 5
					us -= 5; // = 2 cycles,
				} else if constexpr (cpu_freq >= 12000000L) {
					// for the 12 MHz clock if somebody is working with USB

					// for a 1 microsecond delay, simply return.  the overhead
					// of the function call takes 14 (16) cycles, which is 1.5us
					if (us <= 1) return; //  = 3 cycles, (4 when true)

					// the following loop takes 1/3 of a microsecond (4 cycles)
					// per iteration, so execute it three times for each microsecond of
					// delay requested.
					us = (us << 1) + us; // x3 us, = 5 cycles

					// account for the time taken in the preceeding commands.
					// we just burned 20 (22) cycles above, remove 5, (5*4=20)
					// us is at least 6 so we can substract 5
					us -= 5; //2 cycles
				} else if constexpr(cpu_freq >= 8000000L) {
					// for the 8 MHz internal clock

					// for a 1 and 2 microsecond delay, simply return.  the overhead
					// of the function call takes 14 (16) cycles, which is 2us
					if (us <= 2) return; //  = 3 cycles, (4 when true)

					// the following loop takes 1/2 of a microsecond (4 cycles)
					// per iteration, so execute it twice for each microsecond of
					// delay requested.
					us <<= 1; //x2 us, = 2 cycles

					// account for the time taken in the preceeding commands.
					// we just burned 17 (19) cycles above, remove 4, (4*4=16)
					// us is at least 6 so we can substract 4
					us -= 4; // = 2 cycles
				} else {
					// for the 1 MHz internal clock (default settings for common Atmega microcontrollers)

					// the overhead of the function calls is 14 (16) cycles
					if (us <= 16) return; //= 3 cycles, (4 when true)
					if (us <= 25) return; //= 3 cycles, (4 when true), (must be at least 25 if we want to substract 22)

					// compensate for the time taken by the preceeding and next commands (about 22 cycles)
					us -= 22; // = 2 cycles
					// the following loop takes 4 microseconds (4 cycles)
					// per iteration, so execute it us/4 times
					// us is at least 4, divided by 4 gives us 1 (no zero delay bug)
					us >>= 2; // us div 4, = 4 cycles
				}

				// busy wait
				__asm__ __volatile__ (
					"1: sbiw %0,1" "\n\t" // 2 cycles
					"brne 1b" : "=w" (us) : "0" (us) // 2 cycles
				);
				// return = 4 cycles 
			}

			static void delay_ms(time_t ms){ 
				while (ms > 0){
					delay_us(999); // some compensation for while loop...
					ms--;
				}
			}
		};

		template<bool VImplemented>
		struct global_time_impl: details::avr_delay_time{};
	}

	template<class TTimer = timer<0>>
	struct timer_tov_time: details::avr_delay_time {
		using timer_t = TTimer;
		using cs_t = typename timer_t::cs_t;
		using wgm_t = typename timer_t::wgm_t;

		volatile time_t _overflows;

        static inline constexpr auto begin(){ return enable(timer_t::irq_tov); }

        static inline void begin_() { apply(begin()); }		

		// handle_irq_tov
		inline void operator()(){
			_overflows++;
		}

		inline time_t ms() { 
			auto lock = no_irq{};
			return _overflows; 
		}
		inline time_t us() { return ms() * 1000; }
	};

	template<class TTimer = timer<0>, typename TTimer::cs_t VClock = TTimer::cs_t::def, typename TTimer::wgm_t VWgm = TTimer::wgm_t::pwm_fastdef>
	struct timer_time: details::avr_delay_time{
		using timer_t = TTimer;
		using tcnt_t = field_data_type<decltype(TTimer::tcnt)>;
		using cs_t = typename TTimer::cs_t;
		using wgm_t = typename TTimer::wgm_t;
		//static_assert(sizeof(tcnt_t) == 1, "Only 8 bit timers are supported");

		static constexpr auto prescaler = timer_cs_value(VClock);
		static constexpr auto tov_value = timer_wgm_max(VWgm);
		static_assert(tov_value != 0, "Can't calc time in this WGM mode");
		static constexpr auto tov_top = timer_wgm_tov(VWgm);
		static constexpr auto tov_count = tov_top ? (static_cast<std::uint32_t>(tov_value) + 1) :  (static_cast<std::uint32_t>(tov_value) * 2/* + 1*/);

		static constexpr auto us_per_tick = cycles_to_us(prescaler);
		static constexpr auto us_per_tov = cycles_to_us(tov_count * prescaler);
		// whole ms per overflow
		static constexpr auto ms_inc = us_per_tov / 1000;
		
		// fraction of ms per overflow. shift by 3 to fit fracs in byte, it won't loose percision on common F_CPU and prescalers
		static constexpr auto frac_inc = (us_per_tov % 1000) >> 3;
		static constexpr auto frac_ms = 1000 >> 3;

		volatile time_t _ms;
		volatile std::uint8_t _frac;
		volatile time_t _overflows;
	public:		
		static inline constexpr auto begin(){
			return combine(
				timer_t::begin(integral_constant<cs_t, VClock>{}, integral_constant<wgm_t, VWgm>{}),
				enable(timer_t::irq_tov)
			);
		}

		static inline void begin_(){
			apply(begin());
		}

		// handle_irq_tov
		inline void operator()(){
			auto m = _ms;
			auto f = _frac;

			m += ms_inc;
			f += frac_inc;
			if (f >= frac_ms){
				m++;
				f -= frac_ms;
			}

			_ms = m;
			_frac = f;
			_overflows++;
		}

		inline time_t ms() const{
			no_irq noirq{};
			return _ms;
		}

		inline time_t us() const{
			time_t tovs;
			if constexpr (tov_top){
				tcnt_t t;
				{
					no_irq noirq{};
					tovs = _overflows;
					t = read_(timer_t::tcnt);
					// OVF flag set and timer was reset, we should be handling interrupt, but it's off atm
					if (ready_(timer_t::irq_tov) && t < tov_value)
						tovs++;				
				}
				// don't do the math inside no_irq
				return (tovs * tov_count + t) * us_per_tick;
			} else{
				// in tov bottom we can't get nice accuracy since we don't know count direction
				{
					no_irq noiqr{};
					tovs = _overflows;
				}
				return tovs * tov_count * us_per_tick;
			}
		}

		void delay_ms(time_t ms){
			if constexpr (tov_top){
				// use time_us for better accuracy
				auto start = us();
				while (ms > 0){
					if (us() - start >= 1000)
					{
						ms--;
						start += 1000;						
					}
				}
			} else{
				// use time_ms for smaller code size since time_us doesn't have better accuracy
				auto start = this->ms();
				while (ms > 0){
					if (this->ms() - start > 0)
					{
						ms--; 
						start++;
					}
				}
			}
		}
	};
}

#define FH_TIME(NUM, TIME) FH_ISR(FH_IRQ_TOV ## NUM, TIME)

#endif