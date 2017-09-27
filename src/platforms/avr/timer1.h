/*
 * timer1.h
 *
 * Created: 9/26/2017 2:58:14 PM
 *  Author: astr0
 */ 


#ifndef AVR_TIMER1_H_
#define AVR_TIMER1_H_
// Support only "Normal" timer1 for now (not High Speed PWM as on ATTiny25)#if defined(TCNT1) && defined(TCCR1A) && defined(TCCR1B)
#include "../../utils/functions.h"
#include "timer.h"
#include "interrupts.h"

namespace fasthal{
	// Clock Source	FH_TIMER_DECLARE_CS_ENUM_3_EXT(Timer1Cs, CS10, CS11, CS12);				// Waveform generation mode	#if defined(WGM10) && defined(WGM11) && defined(WGM12) && defined(WGM13)	FH_TIMER_DECLARE_WGM_ENUM_4(Timer1Wgm, WGM10, WGM11, WGM12, WGM13);	#endif		// Compare mode A	#if defined(COM1A0) && defined(FH_COM1A1)	FH_TIMER_DECLARE_COM_ENUM(Timer1ComA, COM1A0, FH_COM1A1);	#endif		// compare mode B	#if defined(COM1B0) && defined(COM1B1)	FH_TIMER_DECLARE_COM_ENUM(Timer1ComB, COM1B0, COM1B1);	#endif	// compare mode C	#if defined(COM1C0) && defined(COM1C1)	FH_TIMER_DECLARE_COM_ENUM(Timer1ComC, COM1B0, COM1B1);	#endif		// interrupts - enable	enum class Timer1I: uint8_t{		Overflow = (1 << TOIE1)				#if defined(OCIE1A)		, OuputCompareA = (1 << OCIE1A)		#elif defined(OCIE1)		, OuputCompareA = (1 << OCIE1)		#endif				#if defined(OCIE1B)		, OutputCompareB = (1 << OCIE1B)		#endif		#if defined(OCIE1C)		, OutputCompareC = (1 << OCIE1C)		#endif	};		// interrupts - flag	enum class Timer1F: uint8_t{		Overflow = (1 << TOV1)				#if defined(OCF1A)		, OuputCompareA = (1 << OCF1A)		#elif defined(OCF1)		, OuputCompareA = (1 << OCF1)		#endif				#if defined(OCF1B)		, OuputCompareB = (1 << OCF1B)		#endif				#if defined(OCF1C)		, OuputCompareC = (1 << OCF1C)		#endif	};	// wrap registers	namespace priv{		FH_WRAPVARIABLE(TCNTReg1, TCNT1)				FH_WRAPVARIABLE(TCCRAReg1, TCCR1A)		FH_WRAPVARIABLE(TCCRBReg1, TCCR1B)				#if defined(TCCR1C)		FH_WRAPVARIABLE(TCCRCReg1, TCCR1C);		#else		typedef TCCRAReg1 TCCRCReg1;		#endif				#if defined(TIMSK1)		FH_WRAPVARIABLE(TIMSKReg1, TIMSK1)		#else		FH_WRAPVARIABLE(TIMSKReg1, TIMSK)		#endif				#if defined(TIFR1)		FH_WRAPVARIABLE(TIFRReg1, TIFR1)		#else		FH_WRAPVARIABLE(TIFRReg1, TIFR)		#endif				#if defined(OCR1)		FH_WRAPVARIABLE(OCRAReg1, OCR1)		#elif defined(OCR1A)		FH_WRAPVARIABLE(OCRAReg1, OCR1A)		#endif				#if defined(OCR1B)		FH_WRAPVARIABLE(OCRBReg1, OCR1B)		#endif				#if defined(OCR1C)		FH_WRAPVARIABLE(OCRCReg1, OCR1C)		#endif	}			// The big timer	struct Timer1:		public AvrTimerCS<priv::TCCRBReg1, Timer1Cs, Timer1CsMask, priv::TCNTReg1>		, public InterruptControl<priv::TIMSKReg1, Timer1I>		, public InterruptFlags<priv::TIFRReg1, Timer1F>		#if defined(WGM10) && defined(WGM11) && defined(WGM12) && defined(WGM13)		// has WGM. 		, public AvrTimerWGM2<priv::TCCRAReg1, priv::TCCRBReg1, Timer1Wgm, Timer1WgmMask0, Timer0WgmMask1>		#endif		#if defined(COM1A0) && defined(COM1A1)		// has A		, public AvrTimerComA<priv::TCCRAReg1, Timer1ComA, Timer1ComAMask, priv::OCRAReg1>		#endif		#if defined(FOC1A)		// can force A		, public AvrTimerForceA<priv::TCCRCReg1, FOC1A>		#endif		#if defined(COM1B1) && defined(COM1B0)		// has B		, public AvrTimerComB<priv::TCCRAReg1, Timer1ComB, Timer1ComBMask, priv::OCRBReg1>		#endif		#if defined(FOC1B)		// can force B		, public AvrTimerForce<priv::TCCRCReg1, FOC1B>		#endif
		#if defined(COM1C1) && defined(COM1C0)		// has C		, public AvrTimerComB<priv::TCCRAReg1, Timer1ComC, Timer1ComCMask, priv::OCRCReg1>		#endif		#if defined(FOC1C)		// can force C		, public AvrTimerForceB<priv::TCCRCReg1, FOC1C>		#endif
	{
	};
}


#endif // defined TCCR

#endif /* TIMER0_H_ */