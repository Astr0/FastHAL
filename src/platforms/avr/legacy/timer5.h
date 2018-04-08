// Timer template
// Replace 5 to timer num

#ifndef AVR_TIMER5_H_
#define AVR_TIMER5_H_

#define FH_TIMER5

#if defined(TCNT5) && (!defined(FH_TIMER1) || (defined(TCCR1A) && defined(TCCR1B)))

#include "../../utils/functions.h"
#include "timer.h"
#include "interrupts.h"

namespace fasthal{	
	// Clock Source
	#if defined(FH_TIMER0)
	
	#if !defined(__AVR_ATmega128__)
	#define FH_TIMER5_CS_EXT
	#endif
	
	#elif defined(FH_TIMER1)
	
	#define FH_TIMER5_CS_EXT
	
	#elif defined(FH_TIMER2)

	#if defined(__AVR_ATmega128__)
	#define FH_TIMER5_CS_EXT
	#endif

	#endif
	
	#ifdef FH_TIMER5_CS_EXT
	enum class Timer5Cs: uint8_t{
		None    = (0 << CS52) | (0 << CS51) | (0 << CS50),
		P0      = (0 << CS52) | (0 << CS51) | (1 << CS50),
		P8      = (0 << CS52) | (1 << CS51) | (0 << CS50),
		P64     = (0 << CS52) | (1 << CS51) | (1 << CS50),
		P256    = (1 << CS52) | (0 << CS51) | (0 << CS50),
		P1024   = (1 << CS52) | (0 << CS51) | (1 << CS50),
		ExtFall = (1 << CS52) | (1 << CS51) | (0 << CS50),
		ExtRise = (1 << CS52) | (1 << CS51) | (1 << CS50)
	};
	#else
	enum class Timer5Cs: uint8_t{
		None    = (0 << CS52) | (0 << CS51) | (0 << CS50),
		P0      = (0 << CS52) | (0 << CS51) | (1 << CS50),
		P8      = (0 << CS52) | (1 << CS51) | (0 << CS50),
		P32     = (0 << CS52) | (1 << CS51) | (1 << CS50),
		P64     = (1 << CS52) | (0 << CS51) | (0 << CS50),
		P128    = (1 << CS52) | (0 << CS51) | (1 << CS50),
		P256    = (1 << CS52) | (1 << CS51) | (0 << CS50),
		P1024   = (1 << CS52) | (1 << CS51) | (1 << CS50)
	};
	#endif
	const uint8_t Timer5CsMask = (1 << CS52) | (1 << CS51) | (1 << CS50);
	
		
	// Waveform generation mode
	#if defined(WGM50) && defined(WGM51)
	#if defined(WGM53)
	enum class Timer5Wgm: uint8_t{
		Normal     = (0 << WGM53) | (0 << WGM52) | (0 << WGM51) | (0 << WGM50),
		PwmPc8     = (0 << WGM53) | (0 << WGM52) | (0 << WGM51) | (1 << WGM50),
		PwmPc9     = (0 << WGM53) | (0 << WGM52) | (1 << WGM51) | (0 << WGM50),
		PwmPc10    = (0 << WGM53) | (0 << WGM52) | (1 << WGM51) | (1 << WGM50),
		CtcA       = (0 << WGM53) | (1 << WGM52) | (0 << WGM51) | (0 << WGM50),
		PwmFast8   = (0 << WGM53) | (1 << WGM52) | (0 << WGM51) | (1 << WGM50),
		PwmFast9   = (0 << WGM53) | (1 << WGM52) | (1 << WGM51) | (0 << WGM50),
		PwmFast10  = (0 << WGM53) | (1 << WGM52) | (1 << WGM51) | (1 << WGM50),
		PwmPcFcI   = (1 << WGM53) | (0 << WGM52) | (0 << WGM51) | (0 << WGM50),
		PwmPcFcA   = (1 << WGM53) | (0 << WGM52) | (0 << WGM51) | (1 << WGM50),
		PwmPcI     = (1 << WGM53) | (0 << WGM52) | (1 << WGM51) | (0 << WGM50),
		PwmPcA     = (1 << WGM53) | (0 << WGM52) | (1 << WGM51) | (1 << WGM50),
		CtcI       = (1 << WGM53) | (1 << WGM52) | (0 << WGM51) | (0 << WGM50),
		//Reserved = (0 << WGM53) | (1 << WGM52) | (0 << WGM51) | (1 << WGM50),
		PwmFastI   = (1 << WGM53) | (1 << WGM52) | (1 << WGM51) | (0 << WGM50),
		PwmFastA   = (1 << WGM53) | (1 << WGM52) | (1 << WGM51) | (1 << WGM50),
	};
	const uint8_t Timer5WgmMask0 = (1 << WGM51) | (1 << WGM50);
	const uint8_t Timer5WgmMask1 = (1 << WGM53) | (1 << WGM52);
	#elif defined(WGM52)
	enum class Timer5Wgm: uint8_t{
		Normal     = (0 << WGM52) | (0 << WGM51) | (0 << WGM50),
		PwmPcMax   = (0 << WGM52) | (0 << WGM51) | (1 << WGM50),
		CtcA       = (0 << WGM52) | (1 << WGM51) | (0 << WGM50),
		PwmFastMax = (0 << WGM52) | (1 << WGM51) | (1 << WGM50),
		/*Reserved = (1 << WGM52) | (0 << WGM51) | (0 << WGM50),*/
		PwmPcA     = (1 << WGM52) | (0 << WGM51) | (1 << WGM50),
		/*Reserved = (1 << WGM52) | (1 << WGM51) | (0 << WGM50),*/
		PwmFastA   = (1 << WGM52) | (1 << WGM51) | (1 << WGM50)
	};
	const uint8_t Timer5WgmMask0 = (1 << WGM51) | (1 << WGM50);
	const uint8_t Timer5WgmMask1 = (1 << WGM52);
	#else
	enum class Timer5Wgm: uint8_t{
		Normal     = (0 << WGM51) | (0 << WGM50),
		PwmPcMax   = (0 << WGM51) | (1 << WGM50),
		CtcA       = (1 << WGM51) | (0 << WGM50),
		PwmFastMax = (1 < <WGM51) | (1 << WGM50)
	};
	const uint8_t Timer5WgmMask = (1 << WGM51) | (1 << WGM50);
	#endif
	#endif
	
	// Compare mode A
	#if defined(COM50) && defined(COM51)
	#define FH_COM5A0 COM50
	#define FH_COM5A1 COM51
	#elif defined(COM5A0) && defined(COM5A1)
	#define FH_COM5A0 COM5A0
	#define FH_COM5A1 COM5A1
	#endif
	
	#if defined(FOC5A)
	#define FH_FOC5A FOC5A
	#elif defined(FOC5)
	#define FH_FOC5A FOC5
	#endif

	#if defined(FH_COM5A0) && defined(FH_COM5A1)
	FH_TIMER_DECLARE_COM_ENUM(Timer5ComA, FH_COM5A0, FH_COM5A1);
	#endif
	
	// compare mode B
	#if defined(COM5B0) && defined(COM5B1)
	FH_TIMER_DECLARE_COM_ENUM(Timer5ComB, COM5B0, COM5B1);
	#endif
	
	// compare mode C
	#if defined(COM5C0) && defined(COM5C1)
	FH_TIMER_DECLARE_COM_ENUM(Timer5ComC, COM5C0, COM5C1);
	#endif
	
	// interrupts - enable
	enum class Timer5I: uint8_t{
		Overflow = (1 << TOIE5)
		
		#if defined(OCIE5A)
		, OuputCompareA = (1 << OCIE5A)
		#elif defined(OCIE5)
		, OuputCompareA = (1 << OCIE5)
		#endif
		
		#if defined(OCIE5B)
		, OutputCompareB = (1 << OCIE5B)
		#endif
		
		#if defined(OCIE5C)
		, OutputCompareC = (1 << OCIE5C)
		#endif
		
		#if defined(TICIE5)
		, InputCapture = (1 << TICIE5)
		#endif
	};
	
	// interrupts - flag
	enum class Timer5F: uint8_t{
		Overflow = (1 << TOV5)
		
		#if defined(OCF5A)
		, OuputCompareA = (1 << OCF5A)
		#elif defined(OCF5)
		, OuputCompareA = (1 << OCF5)
		#endif
		
		#if defined(OCF5B)
		, OuputCompareB = (1 << OCF5B)
		#endif

		#if defined(OCF5C)
		, OuputCompareC = (1 << OCF5C)
		#endif
		
		#if defined(ICF5)
		, InputCapture = (1 << ICF5)
		#endif
	};


	// wrap registers
	namespace priv{
		FH_WRAPVARIABLE(TCNTReg5, TCNT5)
		
		#if defined(TCCR)
		FH_WRAPVARIABLE(TCCRReg5, TCCR5)		
		#endif

		#if defined(TCCR5A)
		FH_WRAPVARIABLE(TCCRAReg5, TCCR5A)
		#endif
		
		#if defined(TCCR5B)
		FH_WRAPVARIABLE(TCCRBReg5, TCCR5B)
		#endif
		
		#if defined(TCCR5C)
		FH_WRAPVARIABLE(TCCRCReg5, TCCR5C)
		#endif
		
		#if defined(TIMSK5)
		FH_WRAPVARIABLE(TIMSKReg5, TIMSK5)
		#else
		FH_WRAPVARIABLE(TIMSKReg5, TIMSK)
		#endif
		
		#if defined(TIFR5)
		FH_WRAPVARIABLE(TIFRReg5, TIFR5)
		#else
		FH_WRAPVARIABLE(TIFRReg5, TIFR)
		#endif
		
		#if defined(OCR5A)
		FH_WRAPVARIABLE(OCRAReg5, OCR5A)
		#elif defined(OCR5)
		FH_WRAPVARIABLE(OCRAReg5, OCR5)		
		#endif
		
		#if defined(OCR5B)
		FH_WRAPVARIABLE(OCRBReg5, OCR5B)
		#endif
		
		#if defined(OCR5C)
		FH_WRAPVARIABLE(OCRCReg5, OCR5C)
		#endif
		
		#if defined(ICR5)
		FH_WRAPVARIABLE(ICRReg5, ICR5)
		#endif
		
		// CS Reg
		#if defined(TCCR5B)
		typedef TCCRBReg5 TCCRCsReg5;
		#else
		typedef TCCRReg5 TCCRCsReg5;
		#endif		

		// WGM0/1 Regs
		#if defined(WGM50) && defined(WGM51)
		#if defined(WGM52)
		typedef TCCRAReg5 TCCRWgm0Reg5;
		typedef TCCRBReg5 TCCRWgm1Reg5;
		#else
		typedef TCCRReg5 TCCRWgmReg5;
		#endif
		#endif
		
		// IC Regs
		#if defined(ICNC5) && defined(ICES5)
		typedef TCCRBReg5 TCCRIcReg5;
		#endif
		
		// COM regs
		#if defined(TCCR5A)
		typedef TCCRAReg5 TCCRComAReg5;
		typedef TCCRAReg5 TCCRComBReg5;
		typedef TCCRAReg5 TCCRComCReg5;
		#elif defined(TCCR5)
		typedef TCCRReg5 TCCRComAReg5;
		#endif		

		// FOC regs
		#if defined(TCCR5C)
		
		typedef TCCRCReg5 TCCRFocAReg5;
		typedef TCCRCReg5 TCCRFocBReg5;
		typedef TCCRCReg5 TCCRFocCReg5;
		
		#elif defined(FH_TIMER0) || defined(FH_TIMER2)
		// TCCRB or TCCR
		#if defined(TCCR5B)
		typedef TCCRBReg5 TCCRFocAReg5;
		typedef TCCRBReg5 TCCRFocBReg5;
		#elif defined(TCCR5)
		typedef TCCRReg5 TCCRFocAReg5;
		typedef TCCRReg5 TCCRFocBReg5;
		#endif
		
		#elif defined(FH_TIMER1)
		
		// Mega8/32 have it TCCR1A
		typedef TCCRAReg5 TCCRFocAReg5;
		typedef TCCRAReg5 TCCRFocBReg5;
		
		#endif
	}
	
	
	// The big timer
	struct Timer5:
		public AvrTimerCS<priv::TCCRCsReg5, Timer5Cs, Timer5CsMask, priv::TCNTReg5>
		, public InterruptControl<priv::TIMSKReg5, Timer5I>
		, public InterruptFlags<priv::TIFRReg5, Timer5F>
		
		#if defined(WGM50) && defined(WGM51)
		// has WGM. 
		#if defined(WGM52)
		, public AvrTimerWGM2<priv::TCCRWgm0Reg5, priv::TCCRWgm1Reg5, Timer5Wgm, Timer5WgmMask0, Timer5WgmMask1>
		#else
		, public AvrTimerWGM<priv::TCCRWgmReg5, Timer5Wgm, Timer5WgmMask>
		#endif
		#endif
		
		#if defined(ICNC5) && defined(ICES5)
		, public AvrTimerIc<priv::TCCRIcReg5, ICNC5, ICES5, priv::ICRReg5>
		#endif
		
		#if defined(FH_COM5A0) && defined(FH_COM5A1)
		// has A
		, public AvrTimerOcA<priv::TCCRComAReg5, Timer5ComA, Timer5ComAMask, priv::OCRAReg5>
		#endif
		#if defined(FH_FOC5A)
		// can force A
		, public AvrTimerForceOcA<priv::TCCRFocAReg5, FH_FOC5A>
		#endif
		
		#if defined(COM5B0) && defined(COM5B1)
		// has B
		, public AvrTimerOcB<priv::TCCRComBReg5, Timer5ComB, Timer5ComBMask, priv::OCRBReg5>
		#endif
		#if defined(FOC5B)
		// can force B
		, public AvrTimerForceOcB<priv::TCCRFocBReg5, FOC5B>
		#endif
		
		#if defined(COM5C0) && defined(COM5C1)
		// has C
		, public AvrTimerOcC<priv::TCCRComCReg5, Timer5ComC, Timer5ComCMask, priv::OCRCReg5>
		#endif
		#if defined(FOC5C)
		// can force C
		, public AvrTimerForceOcC<priv::TCCRFocCReg5, FOC5C>
		#endif
	{
	};
}


#endif // defined TCCR

#undef FH_TIMER5

#endif /* TIMER0_H_ */
