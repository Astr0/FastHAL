/*
 * timer0.h
 *
 * Created: 9/26/2017 2:58:14 PM
 *  Author: astr0
 */ 


#ifndef AVR_TIMER0_H_
#define AVR_TIMER0_H_
#if defined(TCNT0)
#include "../../utils/functions.h"
#include "timer.h"
#include "interrupts.h"

namespace fasthal{
	// Clock Source	#if defined(__AVR_ATmega128__)	// CPU Specific	FH_TIMER_DECLARE_CS_ENUM_3(Timer0Cs, CS00, CS01, CS02);	#else	FH_TIMER_DECLARE_CS_ENUM_3_EXT(Timer0Cs, CS00, CS01, CS02);	#endif				// Waveform generation mode	#if defined(WGM00) && defined(WGM01)	#if defined(WGM02)	FH_TIMER_DECLARE_WGM_ENUM_3(Timer0Wgm, WGM00, WGM01, WGM02);	#else	FH_TIMER_DECLARE_WGM_ENUM_2(Timer0Wgm, WGM00, WGM01);		#endif	#endif		// Compare mode A	#if defined(COM00) && defined(COM01)	#define FH_COM0A0 COM00	#define FH_COM0A1 COM01	#elif defined(COM0A0) && defined(COM0A1)	#define FH_COM0A0 COM0A0	#define FH_COM0A1 COM0A1	#endif		#if defined(FOC0)	#define FH_FOC0A FOC0	#elif defined(FOC0A)	#define FH_FOC0A FOC0A	#endif	#if defined(FH_COM0A0) && defined(FH_COM0A1)	FH_TIMER_DECLARE_COM_ENUM(Timer0ComA, FH_COM0A0, FH_COM0A1);	#endif		// compare mode B	#if defined(COM0B0) && defined(COM0B1)	FH_TIMER_DECLARE_COM_ENUM(Timer0ComB, COM0B0, COM0B1);	#endif		// interrupts - enable	enum class Timer0I: uint8_t{		Overflow = (1 << TOIE0)				#if defined(OCIE0A)		, OuputCompareA = (1 << OCIE0A)		#elif defined(OCIE0)		, OuputCompareA = (1 << OCIE0)		#endif				#if defined(OCIE0B)		, OutputCompareB = (1 << OCIE0B)		#endif	};		// interrupts - flag	enum class Timer0F: uint8_t{		#if defined(TOV0)		Overflow = (1 << TOV0)		#endif				#if defined(OCF0A)		, OuputCompareA = (1 << OCF0A)		#elif defined(OCF0)		, OuputCompareA = (1 << OCF0)		#endif				#if defined(OCF0B)		, OuputCompareB = (1 << OCF0B)		#endif			};	// wrap registers	namespace priv{		FH_WRAPVARIABLE(TCNTReg0, TCNT0)				#if defined(TCCR0A)		FH_WRAPVARIABLE(TCCRAReg0, TCCR0A)		#else		FH_WRAPVARIABLE(TCCRAReg0, TCCR0)		#endif				#if defined(TCCR0B)		FH_WRAPVARIABLE(TCCRBReg0, TCCR0B)		#else		typedef TCCRBReg0 TCCRAReg0;		#endif				#if defined(TIMSK0)		FH_WRAPVARIABLE(TIMSKReg0, TIMSK0)		#else		FH_WRAPVARIABLE(TIMSKReg0, TIMSK)		#endif				#if defined(TIFR0)		FH_WRAPVARIABLE(TIFRReg0, TIFR0)		#else		FH_WRAPVARIABLE(TIFRReg0, TIFR)		#endif				#if defined(OCR0)		FH_WRAPVARIABLE(OCRAReg0, OCR0)		#elif defined(OCR0A)		FH_WRAPVARIABLE(OCRAReg0, OCR0A)		#endif				#if defined(OCR0B)		FH_WRAPVARIABLE(OCRBReg0, OCR0B)		#endif	}			// The big timer	struct Timer0:		public AvrTimerCS<priv::TCCRBReg0, Timer0Cs, Timer0CsMask, priv::TCNTReg0>		, public InterruptControl<priv::TIMSKReg0, Timer0I>		, public InterruptFlags<priv::TIFRReg0, Timer0F>		#if defined(WGM00) && defined(WGM01)		// has WGM. 		#if defined(WGM02)		, public AvrTimerWGM2<priv::TCCRAReg0, priv::TCCRBReg0, Timer0Wgm, Timer0WgmMask0, Timer0WgmMask1>		#else		, public AvrTimerWGM<priv::TCCRAReg0, Timer0Wgm, Timer0WgmMask>		#endif		#endif		#if defined(FH_COM0A0) && defined(FH_COM0A1)		// has A		, public AvrTimerComA<priv::TCCRAReg0, Timer0ComA, Timer0ComAMask, priv::OCRAReg0>		#endif		#if defined(FH_FOC0A)		// can force A		, public AvrTimerForceA<priv::TCCRBReg0, FH_FOC0A>		#endif		#if defined(COM0B0) && defined(COM0B1)		// has B		, public AvrTimerComB<priv::TCCRAReg0, Timer0ComB, Timer0ComBMask, priv::OCRBReg0>		#endif		#if defined(FOC0B)		// can force B		, public AvrTimerForceB<priv::TCCRBReg0, FOC0B>		#endif
	{
	};
}


#endif // defined TCCR

#endif /* TIMER0_H_ */