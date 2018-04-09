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

namespace fasthal{
	using time_t = std::uint32_t;

	static constexpr auto cycles_per_us = (F_CPU / 1000000UL);
	static constexpr auto cycles_to_us(std::uint32_t cycles) { return cycles / cycles_per_us;}

	namespace details{
		template<class TTimer = decltype(timer0), typename TTimer::cs_t VClock = TTimer::cs_t::def>
		struct avr_time{
			using tcnt_t = field_data_type<decltype(TTimer::tcnt)>;
			static_assert(sizeof(tcnt_t) == 1, "Only 8 bit timers are supported");

			static constexpr auto prescaler = timer_cs_value(VClock);
			static constexpr auto tov_value = static_cast<tcnt_t>(~(tcnt_t{0}));
			static constexpr auto tov_count = static_cast<std::uint32_t>(tov_value) + 1;

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
			inline void handle_irq_tov(){
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

			inline time_t time_ms() const{
				no_irq noirq{};
				return _ms;
			}

			inline time_t time_us() const{
				time_t tovs;
				tcnt_t t;
				{
					no_irq noirq{};
					tovs = _overflows;
					t = read_(TTimer::tcnt);
					// OVF flag set and timer was reset, we should be handling interrupt, but it's off atm
					if (ready_(TTimer::irq_tov) && t < tov_value)
						tovs++;
				}
				// don't do the math inside no_irq
				return ((tovs << (8 * sizeof(tcnt_t))) + t) * us_per_tick;
			}

			void delay_ms(time_t ms){
				auto start = time_us();
				while (ms > 0){
					if (time_us() - start >= 1000)
					{
						ms--;
						start += 1000;						
					}
				}
			}
		};
	}

	template<class TTimer, typename TTimer::cs_t VClock>
	inline constexpr auto begin(details::avr_time<TTimer, VClock> t){
		return combine(
			begin(TTimer{}, integral_constant<typename TTimer::cs_t, VClock>{}, integral_constant<typename TTimer::wgm_t, TTimer::wgm_t::pwm_fastdef>{}),
			enable(TTimer::irq_tov)
		);
	}

	template<class TTimer, typename TTimer::cs_t VClock>
	inline void begin_(details::avr_time<TTimer, VClock> t){
		apply(begin(t));
	}

	#if defined(FH_TIME)
	#ifndef FH_TIME_CS
	#define FH_TIME_CS _64
	#endif

	static auto time = details::avr_time<details::timer_impl<FH_TIME>, details::timer_impl<FH_TIME>::cs_t::FH_TIME_CS>{};

	namespace details{
    	template<>
    	struct default_isr<details::timer_impl<FH_TIME>::irq_tov.number>{ static inline void handle() { time.handle_irq_tov(); } };
	}
	FH_ISR(FH_JOIN(FH_IRQ_TOV, FH_TIME));

	inline time_t time_ms(){return time.time_ms();}
	inline time_t time_us(){return time.time_us();}
	inline void delay_ms(time_t ms){ time.delay_ms(ms);}
	//inline void delay_us(std::uint16_t us){::delayMicroseconds(us);}
	#elif defined(FH_TIME_ARDUINO)
	inline time_t time_ms(){return ::millis();}
	inline time_t time_us(){return ::micros();}
	inline void delay_ms(time_t ms){::delay(ms);}
	inline void delay_us(std::uint16_t us){::delayMicroseconds(us);}
	#endif
}

#endif