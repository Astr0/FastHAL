/*
 * timer0.h
 *
 * Created: 9/26/2017 2:58:14 PM
 *  Author: astr0
 */ 


#ifndef AVR_TIMER2_H_
#define AVR_TIMER2_H_
#if defined(TCNT2)
#include "../../utils/functions.h"
#include "timer.h"
#include "interrupts.h"

namespace fasthal{
	// Clock Source	#if defined(__AVR_ATmega128__)	// CPU Specific	FH_TIMER_DECLARE_CS_ENUM_3_EXT(Timer2Cs, CS20, CS21, CS22);	#else	FH_TIMER_DECLARE_CS_ENUM_3(Timer2Cs, CS20, CS21, CS22);	#endif				// Waveform generation mode	#if defined(WGM20) && defined(WGM21)	#if defined(WGM22)	FH_TIMER_DECLARE_WGM_ENUM_3(Timer2Wgm, WGM20, WGM21, WGM22);	#else	FH_TIMER_DECLARE_WGM_ENUM_2(Timer2Wgm, WGM20, WGM21);		#endif	#endif		// Compare mode A	#if defined(COM20) && defined(COM21)	#define FH_COM2A0 COM20	#define FH_COM2A1 COM21	#elif defined(COM2A0) && defined(COM2A1)	#define FH_COM2A0 COM2A0	#define FH_COM2A1 COM2A1	#endif		#if defined(FOC2)	#define FH_FOC2A FOC2	#elif defined(FOC2A)	#define FH_FOC2A FOC2A	#endif	#if defined(FH_COM2A0) && defined(FH_COM2A1)	FH_TIMER_DECLARE_COM_ENUM(Timer2ComA, FH_COM2A0, FH_COM2A1);	#endif		// compare mode B	#if defined(COM2B0) && defined(COM2B1)	FH_TIMER_DECLARE_COM_ENUM(Timer2ComB, COM2B0, COM2B1);	#endif		// interrupts - enable	enum class Timer2I: uint8_t{		Overflow = (1 << TOIE2)				#if defined(OCIE2A)		, OuputCompareA = (1 << OCIE2A)		#elif defined(OCIE2)		, OuputCompareA = (1 << OCIE2)		#endif				#if defined(OCIE2B)		, OutputCompareB = (1 << OCIE2B)		#endif	};		// interrupts - flag	enum class Timer2F: uint8_t{		Overflow = (1 << TOV2)				#if defined(OCF2A)		, OuputCompareA = (1 << OCF2A)		#elif defined(OCF2)		, OuputCompareA = (1 << OCF2)		#endif				#if defined(OCF2B)		, OuputCompareB = (1 << OCF2B)		#endif			};	// wrap registers	namespace priv{		FH_WRAPVARIABLE(TCNTReg2, TCNT2)				#if defined(TCCR0A)		FH_WRAPVARIABLE(TCCRAReg2, TCCR2A)		#else		FH_WRAPVARIABLE(TCCRAReg2, TCCR2)		#endif				#if defined(TCCR2B)		FH_WRAPVARIABLE(TCCRBReg2, TCCR2B)		#else		typedef TCCRAReg2 TCCRBReg2;		#endif				#if defined(TIMSK2)		FH_WRAPVARIABLE(TIMSKReg2, TIMSK2)		#else		FH_WRAPVARIABLE(TIMSKReg2, TIMSK)		#endif				#if defined(TIFR2)		FH_WRAPVARIABLE(TIFRReg2, TIFR2)		#else		FH_WRAPVARIABLE(TIFRReg2, TIFR)		#endif				#if defined(OCR2A)		FH_WRAPVARIABLE(OCRAReg2, OCR2A)		#elif defined(OCR2)		FH_WRAPVARIABLE(OCRAReg2, OCR2)		#endif				#if defined(OCR2B)		FH_WRAPVARIABLE(OCRBReg2, OCR2B)		#endif	}			// The big timer	struct Timer2:		public AvrTimerCS<priv::TCCRBReg2, Timer2Cs, Timer2CsMask, priv::TCNTReg2>		, public InterruptControl<priv::TIMSKReg2, Timer2I>		, public InterruptFlags<priv::TIFRReg2, Timer2F>		#if defined(WGM20) && defined(WGM21)		// has WGM. 		#if defined(WGM22)		, public AvrTimerWGM2<priv::TCCRAReg2, priv::TCCRBReg2, Timer2Wgm, Timer2WgmMask0, Timer2WgmMask1>		#else		, public AvrTimerWGM<priv::TCCRAReg2, Timer2Wgm, Timer2WgmMask>		#endif		#endif		#if defined(FH_COM2A0) && defined(FH_COM2A1)		// has A		, public AvrTimerComA<priv::TCCRAReg2, Timer2ComA, Timer2ComAMask, priv::OCRAReg2>		#endif		#if defined(FH_FOC2A)		// can force A		, public AvrTimerForceA<priv::TCCRBReg2, FH_FOC2A>		#endif		#if defined(COM2B0) && defined(COM2B1)		// has B		, public AvrTimerComB<priv::TCCRAReg2, Timer2ComB, Timer2ComBMask, priv::OCRBReg2>		#endif		#if defined(FOC2B)		// can force B		, public AvrTimerForceB<priv::TCCRBReg2, FOC2B>		#endif
	{
	};
}


#endif // defined TCCR

#endif /* AVR_TIMER2_H_ */