#pragma  once

#ifndef AVR_TIMER_H_
#define AVR_TIMER_H_

#include "../../utils/functions.h"
#include "../../sys/TypeManip.h"

namespace fasthal{
	// Simple timer: No outputs (As Timer0 in ATMega8)
	template<
		class tccr,
		typename clocksource_t,
		uint8_t clocksource_mask,
		class tcnt	
	>
	struct AvrTimerCS{
		typedef typename Loki::Select<sizeof(decltype(tcnt::value())) == 1, uint8_t, uint16_t>::Result count_t;
		
		static void enable(clocksource_t source){
			tccr::value() = (tccr::value & ~clocksource_mask) | source;
		}		
		
		count_t getCount(){
			return tcnt::value();
		}
		
		void setCount(count_t count){
			tcnt::value() = count;
		}
	};	
		
	template<
		class tccr_wgm,
		typename wgm_t,
		uint8_t wgm_mask>
	struct AvrTimerWGM{
		static void setWGM(wgm_t wgm){
			tccr_wgm::value() = (tccr_wgm::value() & ~wgm_mask) | (uint8_t)wgm;
		}
	};
	
	template<
		class tccr_coma,
		typename coma_t,
		uint8_t coma_mask,
		class ocra>
	struct AvrTimerComA{
		typedef typename Loki::Select<sizeof(decltype(ocra::value())) == 1, uint8_t, uint16_t>::Result oca_t;
		
		static void setComA(coma_t com)
		{
			tccr_coma::value() = (tccr_coma::value & ~coma_mask) | (uint8_t) com;
		}
		
		static oca_t getOcA(){
			return ocra::value();
		}
		
		static void setOcA(oca_t v){
			ocra::value() = v;
		}
	};
	
	template<
		class tccr_forcea,
		uint8_t forcea_bit>
	struct AvrTimerForceA{
		static void forceOcA(){
			fh_sbi(tccr_forcea::value(), forcea_bit);
		}	
	};		
	
	template<
	class tccr_comb,
	typename comb_t,
	uint8_t comb_mask,
	class ocrb>
	struct AvrTimerComB{
		typedef typename Loki::Select<sizeof(decltype(ocrb::value())) == 1, uint8_t, uint16_t>::Result ocb_t;				
		
		static void setComB(comb_t com)
		{
			tccr_comb::value() = (tccr_comb::value & ~comb_mask) | (uint8_t) com;
		}		
				
		static ocb_t getOcB(){
			return ocrb::value();
		}
		
		static void setOcB(ocb_t v){
			ocrb::value() = v;
		}
	};
	
	template<
	class tccr_forceb,
	uint8_t forceb_bit>
	struct AvrTimerForceB{
		static void forceOcB(){
			fh_sbi(tccr_forceb::value(), forceb_bit);
		}
	};
}



#endif