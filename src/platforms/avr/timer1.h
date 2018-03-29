﻿// Timer template
// Replace 1 to timer num

#ifndef AVR_TIMER1_H_
#define AVR_TIMER1_H_

#define FH_TIMER1

#if defined(TCNT1) && (!defined(FH_TIMER1) || (defined(TCCR1A) && defined(TCCR1B)))

#include "../../utils/functions.h"
#include "timer.h"
#include "interrupts.h"

namespace fasthal{	
	// Clock Source
	#if defined(FH_TIMER0)
	
	#if !defined(__AVR_ATmega128__)
	#define FH_TIMER1_CS_EXT
	#endif
	
	#elif defined(FH_TIMER1)
	
	#define FH_TIMER1_CS_EXT
	
	#elif defined(FH_TIMER2)

	#if defined(__AVR_ATmega128__)
	#define FH_TIMER1_CS_EXT
	#endif

	#endif
	
	#ifdef FH_TIMER1_CS_EXT
	enum class Timer1Cs: uint8_t{
		None    = (0 << CS12) | (0 << CS11) | (0 << CS10),
		P0      = (0 << CS12) | (0 << CS11) | (1 << CS10),
		P8      = (0 << CS12) | (1 << CS11) | (0 << CS10),
		P64     = (0 << CS12) | (1 << CS11) | (1 << CS10),
		P256    = (1 << CS12) | (0 << CS11) | (0 << CS10),
		P1024   = (1 << CS12) | (0 << CS11) | (1 << CS10),
		ExtFall = (1 << CS12) | (1 << CS11) | (0 << CS10),
		ExtRise = (1 << CS12) | (1 << CS11) | (1 << CS10)
	};
	#else
	enum class Timer1Cs: uint8_t{
		None    = (0 << CS12) | (0 << CS11) | (0 << CS10),
		P0      = (0 << CS12) | (0 << CS11) | (1 << CS10),
		P8      = (0 << CS12) | (1 << CS11) | (0 << CS10),
		P32     = (0 << CS12) | (1 << CS11) | (1 << CS10),
		P64     = (1 << CS12) | (0 << CS11) | (0 << CS10),
		P128    = (1 << CS12) | (0 << CS11) | (1 << CS10),
		P256    = (1 << CS12) | (1 << CS11) | (0 << CS10),
		P1024   = (1 << CS12) | (1 << CS11) | (1 << CS10)
	};
	#endif
	const uint8_t Timer1CsMask = (1 << CS12) | (1 << CS11) | (1 << CS10);
	
		
	// Waveform generation mode
	#if defined(WGM10) && defined(WGM11)
	#if defined(WGM13)
	enum class Timer1Wgm: uint8_t{
		Normal     = (0 << WGM13) | (0 << WGM12) | (0 << WGM11) | (0 << WGM10),
		PwmPc8     = (0 << WGM13) | (0 << WGM12) | (0 << WGM11) | (1 << WGM10),
		PwmPc9     = (0 << WGM13) | (0 << WGM12) | (1 << WGM11) | (0 << WGM10),
		PwmPc10    = (0 << WGM13) | (0 << WGM12) | (1 << WGM11) | (1 << WGM10),
		CtcA       = (0 << WGM13) | (1 << WGM12) | (0 << WGM11) | (0 << WGM10),
		PwmFast8   = (0 << WGM13) | (1 << WGM12) | (0 << WGM11) | (1 << WGM10),
		PwmFast9   = (0 << WGM13) | (1 << WGM12) | (1 << WGM11) | (0 << WGM10),
		PwmFast10  = (0 << WGM13) | (1 << WGM12) | (1 << WGM11) | (1 << WGM10),
		PwmPcFcI   = (1 << WGM13) | (0 << WGM12) | (0 << WGM11) | (0 << WGM10),
		PwmPcFcA   = (1 << WGM13) | (0 << WGM12) | (0 << WGM11) | (1 << WGM10),
		PwmPcI     = (1 << WGM13) | (0 << WGM12) | (1 << WGM11) | (0 << WGM10),
		PwmPcA     = (1 << WGM13) | (0 << WGM12) | (1 << WGM11) | (1 << WGM10),
		CtcI       = (1 << WGM13) | (1 << WGM12) | (0 << WGM11) | (0 << WGM10),
		//Reserved = (0 << WGM13) | (1 << WGM12) | (0 << WGM11) | (1 << WGM10),
		PwmFastI   = (1 << WGM13) | (1 << WGM12) | (1 << WGM11) | (0 << WGM10),
		PwmFastA   = (1 << WGM13) | (1 << WGM12) | (1 << WGM11) | (1 << WGM10),
	};
	const uint8_t Timer1WgmMask0 = (1 << WGM11) | (1 << WGM10);
	const uint8_t Timer1WgmMask1 = (1 << WGM13) | (1 << WGM12);
	#elif defined(WGM12)
	enum class Timer1Wgm: uint8_t{
		Normal     = (0 << WGM12) | (0 << WGM11) | (0 << WGM10),
		PwmPcMax   = (0 << WGM12) | (0 << WGM11) | (1 << WGM10),
		CtcA       = (0 << WGM12) | (1 << WGM11) | (0 << WGM10),
		PwmFastMax = (0 << WGM12) | (1 << WGM11) | (1 << WGM10),
		/*Reserved = (1 << WGM12) | (0 << WGM11) | (0 << WGM10),*/
		PwmPcA     = (1 << WGM12) | (0 << WGM11) | (1 << WGM10),
		/*Reserved = (1 << WGM12) | (1 << WGM11) | (0 << WGM10),*/
		PwmFastA   = (1 << WGM12) | (1 << WGM11) | (1 << WGM10)
	};
	const uint8_t Timer1WgmMask0 = (1 << WGM11) | (1 << WGM10);
	const uint8_t Timer1WgmMask1 = (1 << WGM12);
	#else
	enum class Timer1Wgm: uint8_t{
		Normal     = (0 << WGM11) | (0 << WGM10),
		PwmPcMax   = (0 << WGM11) | (1 << WGM10),
		CtcA       = (1 << WGM11) | (0 << WGM10),
		PwmFastMax = (1 < <WGM11) | (1 << WGM10)
	};
	const uint8_t Timer1WgmMask = (1 << WGM11) | (1 << WGM10);
	#endif
	#endif
	
	// Compare mode A
	#if defined(COM10) && defined(COM11)
	#define FH_COM1A0 COM10
	#define FH_COM1A1 COM11
	#elif defined(COM1A0) && defined(COM1A1)
	#define FH_COM1A0 COM1A0
	#define FH_COM1A1 COM1A1
	#endif
	
	#if defined(FOC1A)
	#define FH_FOC1A FOC1A
	#elif defined(FOC1)
	#define FH_FOC1A FOC1
	#endif

	#if defined(FH_COM1A0) && defined(FH_COM1A1)
	FH_TIMER_DECLARE_COM_ENUM(Timer1ComA, FH_COM1A0, FH_COM1A1);
	#endif
	
	// compare mode B
	#if defined(COM1B0) && defined(COM1B1)
	FH_TIMER_DECLARE_COM_ENUM(Timer1ComB, COM1B0, COM1B1);
	#endif
	
	// compare mode C
	#if defined(COM1C0) && defined(COM1C1)
	FH_TIMER_DECLARE_COM_ENUM(Timer1ComC, COM1C0, COM1C1);
	#endif
	
	// interrupts - enable
	enum class Timer1I: uint8_t{
		Overflow = (1 << TOIE1)
		
		#if defined(OCIE1A)
		, OuputCompareA = (1 << OCIE1A)
		#elif defined(OCIE1)
		, OuputCompareA = (1 << OCIE1)
		#endif
		
		#if defined(OCIE1B)
		, OutputCompareB = (1 << OCIE1B)
		#endif
		
		#if defined(OCIE1C)
		, OutputCompareC = (1 << OCIE1C)
		#endif
		
		#if defined(TICIE1)
		, InputCapture = (1 << TICIE1)
		#endif
	};
	
	// interrupts - flag
	enum class Timer1F: uint8_t{
		Overflow = (1 << TOV1)
		
		#if defined(OCF1A)
		, OuputCompareA = (1 << OCF1A)
		#elif defined(OCF1)
		, OuputCompareA = (1 << OCF1)
		#endif
		
		#if defined(OCF1B)
		, OuputCompareB = (1 << OCF1B)
		#endif

		#if defined(OCF1C)
		, OuputCompareC = (1 << OCF1C)
		#endif
		
		#if defined(ICF1)
		, InputCapture = (1 << ICF1)
		#endif
	};


	// wrap registers
	namespace priv{
		FH_WRAPVARIABLE(TCNTReg1, TCNT1)
		
		#if defined(TCCR)
		FH_WRAPVARIABLE(TCCRReg1, TCCR1)		
		#endif

		#if defined(TCCR1A)
		FH_WRAPVARIABLE(TCCRAReg1, TCCR1A)
		#endif
		
		#if defined(TCCR1B)
		FH_WRAPVARIABLE(TCCRBReg1, TCCR1B)
		#endif
		
		#if defined(TCCR1C)
		FH_WRAPVARIABLE(TCCRCReg1, TCCR1C)
		#endif
		
		#if defined(TIMSK1)
		FH_WRAPVARIABLE(TIMSKReg1, TIMSK1)
		#else
		FH_WRAPVARIABLE(TIMSKReg1, TIMSK)
		#endif
		
		#if defined(TIFR1)
		FH_WRAPVARIABLE(TIFRReg1, TIFR1)
		#else
		FH_WRAPVARIABLE(TIFRReg1, TIFR)
		#endif
		
		#if defined(OCR1A)
		FH_WRAPVARIABLE(OCRAReg1, OCR1A)
		#elif defined(OCR1)
		FH_WRAPVARIABLE(OCRAReg1, OCR1)		
		#endif
		
		#if defined(OCR1B)
		FH_WRAPVARIABLE(OCRBReg1, OCR1B)
		#endif
		
		#if defined(OCR1C)
		FH_WRAPVARIABLE(OCRCReg1, OCR1C)
		#endif
		
		#if defined(ICR1)
		FH_WRAPVARIABLE(ICRReg1, ICR1)
		#endif
		
		// CS Reg
		#if defined(TCCR1B)
		typedef TCCRBReg1 TCCRCsReg1;
		#else
		typedef TCCRReg1 TCCRCsReg1;
		#endif		

		// WGM0/1 Regs
		#if defined(WGM10) && defined(WGM11)
		#if defined(WGM12)
		typedef TCCRAReg1 TCCRWgm0Reg1;
		typedef TCCRBReg1 TCCRWgm1Reg1;
		#else
		typedef TCCRReg1 TCCRWgmReg1;
		#endif
		#endif
		
		// IC Regs
		#if defined(ICNC1) && defined(ICES1)
		typedef TCCRBReg1 TCCRIcReg1;
		#endif
		
		// COM regs
		#if defined(TCCR1A)
		typedef TCCRAReg1 TCCRComAReg1;
		typedef TCCRAReg1 TCCRComBReg1;
		typedef TCCRAReg1 TCCRComCReg1;
		#elif defined(TCCR1)
		typedef TCCRReg1 TCCRComAReg1;
		#endif		

		// FOC regs
		#if defined(TCCR1C)
		
		typedef TCCRCReg1 TCCRFocAReg1;
		typedef TCCRCReg1 TCCRFocBReg1;
		typedef TCCRCReg1 TCCRFocCReg1;
		
		#elif defined(FH_TIMER0) || defined(FH_TIMER2)
		// TCCRB or TCCR
		#if defined(TCCR1B)
		typedef TCCRBReg1 TCCRFocAReg1;
		typedef TCCRBReg1 TCCRFocBReg1;
		#elif defined(TCCR1)
		typedef TCCRReg1 TCCRFocAReg1;
		typedef TCCRReg1 TCCRFocBReg1;
		#endif
		
		#elif defined(FH_TIMER1)
		
		// Mega8/32 have it TCCR1A
		typedef TCCRAReg1 TCCRFocAReg1;
		typedef TCCRAReg1 TCCRFocBReg1;
		
		#endif
	}
	
	
	// The big timer
	struct Timer1:
		public AvrTimerCS<priv::TCCRCsReg1, Timer1Cs, Timer1CsMask, priv::TCNTReg1>
		, public InterruptControl<priv::TIMSKReg1, Timer1I>
		, public InterruptFlags<priv::TIFRReg1, Timer1F>
		
		#if defined(WGM10) && defined(WGM11)
		// has WGM. 
		#if defined(WGM12)
		, public AvrTimerWGM2<priv::TCCRWgm0Reg1, priv::TCCRWgm1Reg1, Timer1Wgm, Timer1WgmMask0, Timer1WgmMask1>
		#else
		, public AvrTimerWGM<priv::TCCRWgmReg1, Timer1Wgm, Timer1WgmMask>
		#endif
		#endif
		
		#if defined(ICNC1) && defined(ICES1)
		, public AvrTimerIc<priv::TCCRIcReg1, ICNC1, ICES1, priv::ICRReg1>
		#endif
		
		#if defined(FH_COM1A0) && defined(FH_COM1A1)
		// has A
		, public AvrTimerOcA<priv::TCCRComAReg1, Timer1ComA, Timer1ComAMask, priv::OCRAReg1>
		#endif
		#if defined(FH_FOC1A)
		// can force A
		, public AvrTimerForceOcA<priv::TCCRFocAReg1, FH_FOC1A>
		#endif
		
		#if defined(COM1B0) && defined(COM1B1)
		// has B
		, public AvrTimerOcB<priv::TCCRComBReg1, Timer1ComB, Timer1ComBMask, priv::OCRBReg1>
		#endif
		#if defined(FOC1B)
		// can force B
		, public AvrTimerForceOcB<priv::TCCRFocBReg1, FOC1B>
		#endif
		
		#if defined(COM1C0) && defined(COM1C1)
		// has C
		, public AvrTimerOcC<priv::TCCRComCReg1, Timer1ComC, Timer1ComCMask, priv::OCRCReg1>
		#endif
		#if defined(FOC1C)
		// can force C
		, public AvrTimerForceOcC<priv::TCCRFocCReg1, FOC1C>
		#endif
	{
	};
}


#endif // defined TCCR

#undef FH_TIMER1

#endif /* TIMER0_H_ */