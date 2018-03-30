#pragma  once

#ifndef AVR_TIMER_H_
#define AVR_TIMER_H_

#include "../../utils/functions.h"

// WGM
#define FH_TIMER_DECLARE_WGM_ENUM_2(Name, M0, M1)\


#define FH_TIMER_DECLARE_WGM_ENUM_3(Name, M0, M1, M2)\


// COM
#define FH_TIMER_DECLARE_COM_ENUM(Name, M0, M1) \
	enum class Name: uint8_t{\
		None   = (0 << M1) | (0 << M0),\
		Toggle = (0 << M1) | (1 << M0),\
		Clear  = (1 << M1) | (0 << M0),\
		Set    = (1 << M1) | (1 << M0)\
	};\
	const uint8_t Name ## Mask = (1 << M1) | (1 << M0);


	

namespace fasthal{
	// Simple timer: No outputs (As Timer0 in ATMega8)
	template<
		class tccr,
		typename clocksource_t,
		uint8_t clocksource_mask,
		class tcnt	
	>
	struct AvrTimerCS{
		using count_t = typename std::conditional<sizeof(decltype(tcnt::value())) == 1, uint8_t, uint16_t>::type;
		
		static void enable(clocksource_t source){
			tccr::value() = (tccr::value() & ~clocksource_mask) | (uint8_t)source;
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
		static void setWgm(wgm_t wgm){
			tccr_wgm::value() = (tccr_wgm::value() & ~wgm_mask) | (uint8_t)wgm;
		}
		
		static wgm_t getWgm(){
			return (wgm_t)(tccr_wgm::value() & wgm_mask);
		}
	};
	
	template<
	class tccr_wgm0,
	class tccr_wgm1,
	typename wgm_t,
	uint8_t wgm_mask0,
	uint8_t wgm_mask1>
	struct AvrTimerWGM2{
		static void setWgm(wgm_t wgm){
			tccr_wgm0::value() = (tccr_wgm0::value() & ~wgm_mask0) | ((uint8_t)wgm & wgm_mask0);
			tccr_wgm1::value() = (tccr_wgm1::value() & ~wgm_mask1) | ((uint8_t)wgm & wgm_mask1);
		}
		
		static wgm_t getWgm(){
			return (wgm_t)((tccr_wgm0::value() & wgm_mask0) | (tccr_wgm1::value() & wgm_mask1));
		}
	};
	
	template<class tccr, uint8_t nc_bit, uint8_t es_bit, class icr>
	struct AvrTimerIc{
		using ic_t = typename std::conditional<sizeof(decltype(icr::value())) == 1, uint8_t, uint16_t>::type;
		static void enableIcNoiseCanceller(bool enable){ fh_wbi(tccr::value(), nc_bit, enable); }
		static void setIcEdge(bool rise){ fh_wbi(tccr::value(), es_bit, rise); }
		static ic_t getIc() { return icr::value(); }
		static void setIc(ic_t v){ icr::value() = v; }
	};
	
#define FH_TIMER_DECLARE_OC_TEMPLATE(x, X)\
	template<class tccr, typename com_t, uint8_t com_mask, class ocr>\
	struct AvrTimerOc ## X{\
		using oc ## x ## _t = typename std::conditional<sizeof(decltype(ocr::value())) == 1, uint8_t, uint16_t>::type;\
		static void setCom ## x(com_t com) { tccr::value() = (tccr::value() & ~com_mask) | (uint8_t) com; }\
		static oc ## x ## _t getOc ## X() { return ocr::value(); }\
		static void setOc ## X(oc ## x ## _t v){ ocr::value() = v; }\
	};
FH_TIMER_DECLARE_OC_TEMPLATE(a, A);
FH_TIMER_DECLARE_OC_TEMPLATE(b, B);
FH_TIMER_DECLARE_OC_TEMPLATE(c, C);
	
#define FH_TIMER_DECLARE_FORCEOC_TEMPLATE(X)\
	template<class tccr, uint8_t force_bit>\
	struct AvrTimerForceOc ## X{\
		static void forceOc ## X(){ fh_sbi(tccr::value(), force_bit); }\
	};		
	
FH_TIMER_DECLARE_FORCEOC_TEMPLATE(A);	
FH_TIMER_DECLARE_FORCEOC_TEMPLATE(B);
FH_TIMER_DECLARE_FORCEOC_TEMPLATE(C);

}



#endif