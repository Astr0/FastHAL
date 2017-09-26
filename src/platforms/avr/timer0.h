/*
 * timer0.h
 *
 * Created: 9/26/2017 2:58:14 PM
 *  Author: astr0
 */ 


#ifndef AVR_TIMER0_H_
#define AVR_TIMER0_H_
#if defined(TCCR0B) || defined(TCCR0)
#include "../../utils/functions.h"
#include "timer.h"
#include "interrupts.h"

namespace fasthal{
	// Clock Source	enum class Timer0CS: uint8_t{		None   = (0 << CS02) | (0 << CS01) | (0 << CS00),		P0     = (0 << CS02) | (0 << CS01) | (1 << CS00),		P8     = (0 << CS02) | (1 << CS01) | (0 << CS00),		#if defined(__AVR_ATmega128__)		// CPU specific stuff..		P32    = (0 << CS02) | (1 << CS01) | (1 << CS00),		P64    = (1 << CS02) | (0 << CS01) | (0 << CS00),		P128   = (1 << CS02) | (0 << CS01) | (1 << CS00),		P256   = (1 << CS02) | (1 << CS01) | (0 << CS00),		P1024  = (1 << CS02) | (1 << CS01) | (1 << CS00)		#else		P64    = (0 << CS02) | (1 << CS01) | (1 << CS00),		P256   = (1 << CS02) | (0 << CS01) | (0 << CS00),		P1024  = (1 << CS02) | (0 << CS01) | (1 << CS00),		T0Fall = (1 << CS02) | (1 << CS01) | (0 << CS00),		T0Rise = (1 << CS02) | (1 << CS01) | (1 << CS00)		#endif		#if defined(WGM02)		,TopA   = (1 << WGM02)		#endif	};		#ifdef WGM02	const uint8_t Timer0CSMask = ((1 << CS02) | (1 << CS01) | (1 << CS00) | (1 << WGM02));	#else	const uint8_t Timer0CSMask = ((1 << CS02) | (1 << CS01) | (1 << CS00));	#endif				// Waveform generation mode	#if defined(WGM00) && defined(WGM01)	enum class Timer0WGM: uint8_t{		Normal     = (0 << WGM01) | (0 << WGM00),		PwmPcMax   = (0 << WGM01) | (1 << WGM00),		CtcA       = (1 << WGM01) | (0 << WGM00),		PwmFastMax = (1 << WGM01) | (1 << WGM00)		//#if defined(WGM02)		//,		//// reserved		//PwmPcA     = (1 << WGM02) | (0 << WGM01) | (1 << WGM00),		//// reserved		//PwmFastA   = (1 << WGM02) | (1 << WGM01) | (1 << WGM00)		//#endif	};	#endif		// Compare mode A	#if defined(COM00) && defined(COM01) && defined(FOC0)	#define FH_COM0A0 COM00	#define FH_COM0A1 COM01	#define FH_FOC0A FOC0	#elif defined(COM0A0) && defined(COM0A1) && defined(FOC0A)	#define FH_COM0A0 COM0A0	#define FH_COM0A1 COM0A1	#define FH_FOC0A FOC0A	#endif			#if defined(FH_COM0A1) && defined(FH_COM0A0)	FH_TIMER_DECLARE_COM_ENUM(Timer0COMA, FH_COM0A0, FH_COM0A1);	#endif		// compare mode B	#if defined(COM0B1) && defined(COM0B0)	FH_TIMER_DECLARE_COM_ENUM(Timer0COMB, COM0B0, COM0B1);	#endif		// interrupts - enable	enum class Timer0IE: uint8_t{		Overflow = (1 << TOIE0)				#if defined(OCIE0A)		, OuputCompareA = (1 << OCIE0A)		#elif defined(OCIE0)		, OuputCompareA = (1 << OCIE0A)		#endif				#if defined(OCIE0B)		, OutputCompareB = (1 << OCIE0B)		#endif	};		// interrupts - flag	enum class Timer0IF: uint8_t{		#if defined(TOV)		Overflow = (1 << TOV)		#elif defined(TOV0)					Overflow = (1 << TOV0)		#endif				#if defined(OCF0A)		, OuputCompareA = (1 << OCF0A)		#elif defined(OCF0)		, OuputCompareA = (1 << OCF0)		#elif defined(OCFA)		, OuputCompareA = (1 << OCFA)		#endif				#if defined(OCF0B)		, OuputCompareB = (1 << OCF0B)		#elif defined(OCFB)		, OuputCompareB = (1 << OCFB)		#endif			};	// wrap registers	namespace priv{		#if defined(TCCR0A)		FH_WRAPVARIABLE(TCCRAReg0, TCCR0A)		#else		FH_WRAPVARIABLE(TCCRAReg0, TCCR0)		#endif				#if defined(TCCR0B)		FH_WRAPVARIABLE(TCCRBReg0, TCCR0B)		#else		FH_WRAPVARIABLE(TCCRBReg0, TCCR0)		#endif				FH_WRAPVARIABLE(TCNTReg0, TCNT0)				#if defined(TIMSK0)		FH_WRAPVARIABLE(TIMSKReg0, TIMSK0)		#else		FH_WRAPVARIABLE(TIMSKReg0, TIMSK)		#endif				#if defined(TIFR0)		FH_WRAPVARIABLE(TIFRReg0, TIFR0)		#else		FH_WRAPVARIABLE(TIFRReg0, TIFR)		#endif				#if defined(OCR0)		FH_WRAPVARIABLE(OCRAReg0, OCR0)		#elif defined(OCR0A)		FH_WRAPVARIABLE(OCRAReg0, OCR0A)		#endif				#if defined(OCR0B)		FH_WRAPVARIABLE(OCRBReg0, OCR0B)		#endif	}			// The big timer	struct Timer0:		public AvrTimerCS<priv::TCCRBReg0, Timer0CS, Timer0CSMask, priv::TCNTReg0>		, public InterruptControl<priv::TIMSKReg0, Timer0IE>		, public InterruptFlags<priv::TIFRReg0, Timer0IF>		#if defined(WGM01) && defined(WGM00)		// has WGM		, public AvrTimerWGM<priv::TCCRAReg0, Timer0WGM, (1 << WGM01) | (1 << WGM00)>		#endif		#if defined(FH_COM0A1) && defined(FH_COM0A0)		// has A		, public AvrTimerComA<priv::TCCRAReg0, Timer0COMA, Timer0COMAMask, priv::OCRAReg0>		#endif		#if defined(FH_FOC0A)		// can force A		, public AvrTimerForceA<priv::TCCRBReg0, FH_FOC0A>		#endif		#if defined(COM0B1) && defined(COM0B0)		// has B		, public AvrTimerComB<priv::TCCRAReg0, Timer0COMB, Timer0COMBMask, priv::OCRBReg0>		#endif		#if defined(FOC0B)		// can force B		, public AvrTimerForceB<priv::TCCRBReg0, FOC0B>		#endif
	{
	};
}


#endif // defined TCCR

#endif /* TIMER0_H_ */