#pragma  once

#ifndef AVR_TIMER_H_
#define AVR_TIMER_H_

#include "../../utils/functions.h"
#include "../../sys/TypeManip.h"

// CS
#define FH_TIMER_DECLARE_CS_ENUM_3(Name, M0, M1, M2)\
	enum class Name: uint8_t{\
		None    = (0 << M2) | (0 << M1) | (0 << M0),\		P0      = (0 << M2) | (0 << M1) | (1 << M0),\		P8      = (0 << M2) | (1 << M1) | (0 << M0),\		P32     = (0 << M2) | (1 << M1) | (1 << M0),\		P64     = (1 << M2) | (0 << M1) | (0 << M0),\		P128    = (1 << M2) | (0 << M1) | (1 << M0),\		P256    = (1 << M2) | (1 << M1) | (0 << M0),\		P1024   = (1 << M2) | (1 << M1) | (1 << M0)\
	};\
	const uint8_t Name ## Mask = (1 << M2) | (1 << M1) | (1 << M0);


#define FH_TIMER_DECLARE_CS_ENUM_3_EXT(Name, M0, M1, M2)\
	enum class Name: uint8_t{\
		None    = (0 << M2) | (0 << M1) | (0 << M0),\		P0      = (0 << M2) | (0 << M1) | (1 << M0),\		P8      = (0 << M2) | (1 << M1) | (0 << M0),\		P64     = (0 << M2) | (1 << M1) | (1 << M0),\		P256    = (1 << M2) | (0 << M1) | (0 << M0),\		P1024   = (1 << M2) | (0 << M1) | (1 << M0),\		ExtFall = (1 << M2) | (1 << M1) | (0 << M0),\		ExtRise = (1 << M2) | (1 << M1) | (1 << M0)\
	};\
	const uint8_t Name ## Mask = (1 << M2) | (1 << M1) | (1 << M0);

// WGM
#define FH_TIMER_DECLARE_WGM_ENUM_2(Name, M0, M1)\
enum class Name: uint8_t{\	Normal     = (0 << M1) | (0 << M0),\	PwmPcMax   = (0 << M1) | (1 << M0),\	CtcA       = (1 << M1) | (0 << M0),\	PwmFastMax = (1 << M1) | (1 << M0)\
};\
const uint8_t Name ## Mask = (1 << M1) | (1 << M0);

#define FH_TIMER_DECLARE_WGM_ENUM_3(Name, M0, M1, M2)\
enum class Name: uint8_t{\	Normal     = (0 << M2) | (0 << M1) | (0 << M0),\	PwmPcMax   = (0 << M2) | (0 << M1) | (1 << M0),\	CtcA       = (0 << M2) | (1 << M1) | (0 << M0),\	PwmFastMax = (0 << M2) | (1 << M1) | (1 << M0),\
	PwmPcA     = (1 << M2) | (0 << M1) | (1 << M0),\	PwmFastA   = (1 << M2) | (1 << M1) | (1 << M0)\
};\
const uint8_t Name ## Mask0 = (1 << M1) | (1 << M0);\
const uint8_t Name ## Mask1 = (1 << M2); 



// COM
#define FH_TIMER_DECLARE_COM_ENUM(Name, M0, M1) \
	enum class Name: uint8_t{\
		None   = (0 << M1) | (0 << M0),\		Toggle = (0 << M1) | (1 << M0),\		Clear  = (1 << M1) | (0 << M0),\		Set    = (1 << M1) | (1 << M0)\
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
		typedef typename Loki::Select<sizeof(decltype(tcnt::value())) == 1, uint8_t, uint16_t>::Result count_t;
		
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
	
#define FH_TIMER_DECLARE_COM_TEMPLATE(x, X)\
	template<class tccr, typename com_t, uint8_t com_mask, class ocr>\
	struct AvrTimerCom ## X{\
		typedef typename Loki::Select<sizeof(decltype(ocr::value())) == 1, uint8_t, uint16_t>::Result oc ## x ## _t;\
		static void setCom ## x(com_t com) { tccr::value() = (tccr::value() & ~com_mask) | (uint8_t) com; }\
		static oc ## x ## _t getOc ## X() { return ocr::value(); }\
		static void setOc ## X(oc ## x ## _t v){ ocr::value() = v; }\
	};
FH_TIMER_DECLARE_COM_TEMPLATE(a, A);
FH_TIMER_DECLARE_COM_TEMPLATE(b, B);
	
#define FH_TIMER_DECLARE_FORCE_TEMPLATE(X)\
	template<class tccr, uint8_t force_bit>\
	struct AvrTimerForce ## X{\
		static void forceOc ## X(){ fh_sbi(tccr::value(), force_bit); }\
	};		
	
FH_TIMER_DECLARE_FORCE_TEMPLATE(A);	
FH_TIMER_DECLARE_FORCE_TEMPLATE(B);
	//template<
	//class tccr_comb,
	//typename comb_t,
	//uint8_t comb_mask,
	//class ocrb>
	//struct AvrTimerComB{
		//typedef typename Loki::Select<sizeof(decltype(ocrb::value())) == 1, uint8_t, uint16_t>::Result ocb_t;				
		//
		//static void setComB(comb_t com)
		//{
			//tccr_comb::value() = (tccr_comb::value() & ~comb_mask) | (uint8_t) com;
		//}		
				//
		//static ocb_t getOcB(){
			//return ocrb::value();
		//}
		//
		//static void setOcB(ocb_t v){
			//ocrb::value() = v;
		//}
	//};
	
	//template<
	//class tccr_forceb,
	//uint8_t forceb_bit>
	//struct AvrTimerForceB{
		//static void forceOcB(){
			//fh_sbi(tccr_forceb::value(), forceb_bit);
		//}
	//};
}



#endif