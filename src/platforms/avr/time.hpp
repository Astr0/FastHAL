#ifndef FH_AVR_TIME_H_
#define FH_AVR_TIME_H_

/* 	8 bit TCNT _64 cs  	@ 16Mhz     @1Mhz	
	us per tick			4			64
	1M * 64 / 16M
	us per overflow		1024		16384
	us per tick * 256
*/

#include "../../std/std_types.hpp"

namespace fasthal{
	using time_t = std::uint32_t;

	namespace details{
		template<class TTimer, typename TTimer::cs_t VClock = TTimer::cs_t::def>
		class avr_time{
			std::uint32_t _ms;
			std::uint16_t _frac;
			std::uint32_t _overflows;
		};
	}

	#if defined(FH_TIME_ARDUINO)
	inline time_t time_ms(){return ::millis();}
	inline time_t time_us(){return ::micros();}
	inline void delay_ms(time_t ms){::delay(ms);}
	inline void delay_us(std::uint16_t us){::delayMicroseconds(us);}
	#endif
}

#endif