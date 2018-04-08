// Timer template
// Replace 3 to timer num

#ifndef AVR_TIMER3_H_
#define AVR_TIMER3_H_

#define FH_TIMER3

#if defined(TCNT3) && (!defined(FH_TIMER1) || (defined(TCCR1A) && defined(TCCR1B)))

#include "../../utils/functions.h"
#include "timer.h"
#include "interrupts.h"

namespace fasthal{	
	// Clock Source
	#if defined(FH_TIMER0)
	
	#if !defined(__AVR_ATmega128__)
	#define FH_TIMER3_CS_EXT
	#endif
	
	#elif defined(FH_TIMER1)
	
	#define FH_TIMER3_CS_EXT
	
	#elif defined(FH_TIMER2)

	#if defined(__AVR_ATmega128__)
	#define FH_TIMER3_CS_EXT
	#endif

	#endif
	
	#ifdef FH_TIMER3_CS_EXT
	enum class Timer3Cs: uint8_t{
		None    = (0 << CS32) | (0 << CS31) | (0 << CS30),
		P0      = (0 << CS32) | (0 << CS31) | (1 << CS30),
		P8      = (0 << CS32) | (1 << CS31) | (0 << CS30),
		P64     = (0 << CS32) | (1 << CS31) | (1 << CS30),
		P256    = (1 << CS32) | (0 << CS31) | (0 << CS30),
		P1024   = (1 << CS32) | (0 << CS31) | (1 << CS30),
		ExtFall = (1 << CS32) | (1 << CS31) | (0 << CS30),
		ExtRise = (1 << CS32) | (1 << CS31) | (1 << CS30)
	};
	#else
	enum class Timer3Cs: uint8_t{
		None    = (0 << CS32) | (0 << CS31) | (0 << CS30),
		P0      = (0 << CS32) | (0 << CS31) | (1 << CS30),
		P8      = (0 << CS32) | (1 << CS31) | (0 << CS30),
		P32     = (0 << CS32) | (1 << CS31) | (1 << CS30),
		P64     = (1 << CS32) | (0 << CS31) | (0 << CS30),
		P128    = (1 << CS32) | (0 << CS31) | (1 << CS30),
		P256    = (1 << CS32) | (1 << CS31) | (0 << CS30),
		P1024   = (1 << CS32) | (1 << CS31) | (1 << CS30)
	};
	#endif
	const uint8_t Timer3CsMask = (1 << CS32) | (1 << CS31) | (1 << CS30);
	
		
	// Waveform generation mode
	#if defined(WGM30) && defined(WGM31)
	#if defined(WGM33)
	enum class Timer3Wgm: uint8_t{
		Normal     = (0 << WGM33) | (0 << WGM32) | (0 << WGM31) | (0 << WGM30),
		PwmPc8     = (0 << WGM33) | (0 << WGM32) | (0 << WGM31) | (1 << WGM30),
		PwmPc9     = (0 << WGM33) | (0 << WGM32) | (1 << WGM31) | (0 << WGM30),
		PwmPc10    = (0 << WGM33) | (0 << WGM32) | (1 << WGM31) | (1 << WGM30),
		CtcA       = (0 << WGM33) | (1 << WGM32) | (0 << WGM31) | (0 << WGM30),
		PwmFast8   = (0 << WGM33) | (1 << WGM32) | (0 << WGM31) | (1 << WGM30),
		PwmFast9   = (0 << WGM33) | (1 << WGM32) | (1 << WGM31) | (0 << WGM30),
		PwmFast10  = (0 << WGM33) | (1 << WGM32) | (1 << WGM31) | (1 << WGM30),
		PwmPcFcI   = (1 << WGM33) | (0 << WGM32) | (0 << WGM31) | (0 << WGM30),
		PwmPcFcA   = (1 << WGM33) | (0 << WGM32) | (0 << WGM31) | (1 << WGM30),
		PwmPcI     = (1 << WGM33) | (0 << WGM32) | (1 << WGM31) | (0 << WGM30),
		PwmPcA     = (1 << WGM33) | (0 << WGM32) | (1 << WGM31) | (1 << WGM30),
		CtcI       = (1 << WGM33) | (1 << WGM32) | (0 << WGM31) | (0 << WGM30),
		//Reserved = (0 << WGM33) | (1 << WGM32) | (0 << WGM31) | (1 << WGM30),
		PwmFastI   = (1 << WGM33) | (1 << WGM32) | (1 << WGM31) | (0 << WGM30),
		PwmFastA   = (1 << WGM33) | (1 << WGM32) | (1 << WGM31) | (1 << WGM30),
	};
	const uint8_t Timer3WgmMask0 = (1 << WGM31) | (1 << WGM30);
	const uint8_t Timer3WgmMask1 = (1 << WGM33) | (1 << WGM32);
	#elif defined(WGM32)
	enum class Timer3Wgm: uint8_t{
		Normal     = (0 << WGM32) | (0 << WGM31) | (0 << WGM30),
		PwmPcMax   = (0 << WGM32) | (0 << WGM31) | (1 << WGM30),
		CtcA       = (0 << WGM32) | (1 << WGM31) | (0 << WGM30),
		PwmFastMax = (0 << WGM32) | (1 << WGM31) | (1 << WGM30),
		/*Reserved = (1 << WGM32) | (0 << WGM31) | (0 << WGM30),*/
		PwmPcA     = (1 << WGM32) | (0 << WGM31) | (1 << WGM30),
		/*Reserved = (1 << WGM32) | (1 << WGM31) | (0 << WGM30),*/
		PwmFastA   = (1 << WGM32) | (1 << WGM31) | (1 << WGM30)
	};
	const uint8_t Timer3WgmMask0 = (1 << WGM31) | (1 << WGM30);
	const uint8_t Timer3WgmMask1 = (1 << WGM32);
	#else
	enum class Timer3Wgm: uint8_t{
		Normal     = (0 << WGM31) | (0 << WGM30),
		PwmPcMax   = (0 << WGM31) | (1 << WGM30),
		CtcA       = (1 << WGM31) | (0 << WGM30),
		PwmFastMax = (1 < <WGM31) | (1 << WGM30)
	};
	const uint8_t Timer3WgmMask = (1 << WGM31) | (1 << WGM30);
	#endif
	#endif
	
	// Compare mode A
	#if defined(COM30) && defined(COM31)
	#define FH_COM3A0 COM30
	#define FH_COM3A1 COM31
	#elif defined(COM3A0) && defined(COM3A1)
	#define FH_COM3A0 COM3A0
	#define FH_COM3A1 COM3A1
	#endif
	
	#if defined(FOC3A)
	#define FH_FOC3A FOC3A
	#elif defined(FOC3)
	#define FH_FOC3A FOC3
	#endif

	#if defined(FH_COM3A0) && defined(FH_COM3A1)
	FH_TIMER_DECLARE_COM_ENUM(Timer3ComA, FH_COM3A0, FH_COM3A1);
	#endif
	
	// compare mode B
	#if defined(COM3B0) && defined(COM3B1)
	FH_TIMER_DECLARE_COM_ENUM(Timer3ComB, COM3B0, COM3B1);
	#endif
	
	// compare mode C
	#if defined(COM3C0) && defined(COM3C1)
	FH_TIMER_DECLARE_COM_ENUM(Timer3ComC, COM3C0, COM3C1);
	#endif
	
	// interrupts - enable
	enum class Timer3I: uint8_t{
		Overflow = (1 << TOIE3)
		
		#if defined(OCIE3A)
		, OuputCompareA = (1 << OCIE3A)
		#elif defined(OCIE3)
		, OuputCompareA = (1 << OCIE3)
		#endif
		
		#if defined(OCIE3B)
		, OutputCompareB = (1 << OCIE3B)
		#endif
		
		#if defined(OCIE3C)
		, OutputCompareC = (1 << OCIE3C)
		#endif
		
		#if defined(TICIE3)
		, InputCapture = (1 << TICIE3)
		#endif
	};
	
	// interrupts - flag
	enum class Timer3F: uint8_t{
		Overflow = (1 << TOV3)
		
		#if defined(OCF3A)
		, OuputCompareA = (1 << OCF3A)
		#elif defined(OCF3)
		, OuputCompareA = (1 << OCF3)
		#endif
		
		#if defined(OCF3B)
		, OuputCompareB = (1 << OCF3B)
		#endif

		#if defined(OCF3C)
		, OuputCompareC = (1 << OCF3C)
		#endif
		
		#if defined(ICF3)
		, InputCapture = (1 << ICF3)
		#endif
	};


	// wrap registers
	namespace priv{
		FH_WRAPVARIABLE(TCNTReg3, TCNT3)
		
		#if defined(TCCR)
		FH_WRAPVARIABLE(TCCRReg3, TCCR3)		
		#endif

		#if defined(TCCR3A)
		FH_WRAPVARIABLE(TCCRAReg3, TCCR3A)
		#endif
		
		#if defined(TCCR3B)
		FH_WRAPVARIABLE(TCCRBReg3, TCCR3B)
		#endif
		
		#if defined(TCCR3C)
		FH_WRAPVARIABLE(TCCRCReg3, TCCR3C)
		#endif
		
		#if defined(TIMSK3)
		FH_WRAPVARIABLE(TIMSKReg3, TIMSK3)
		#else
		FH_WRAPVARIABLE(TIMSKReg3, TIMSK)
		#endif
		
		#if defined(TIFR3)
		FH_WRAPVARIABLE(TIFRReg3, TIFR3)
		#else
		FH_WRAPVARIABLE(TIFRReg3, TIFR)
		#endif
		
		#if defined(OCR3A)
		FH_WRAPVARIABLE(OCRAReg3, OCR3A)
		#elif defined(OCR3)
		FH_WRAPVARIABLE(OCRAReg3, OCR3)		
		#endif
		
		#if defined(OCR3B)
		FH_WRAPVARIABLE(OCRBReg3, OCR3B)
		#endif
		
		#if defined(OCR3C)
		FH_WRAPVARIABLE(OCRCReg3, OCR3C)
		#endif
		
		#if defined(ICR3)
		FH_WRAPVARIABLE(ICRReg3, ICR3)
		#endif
		
		// CS Reg
		#if defined(TCCR3B)
		typedef TCCRBReg3 TCCRCsReg3;
		#else
		typedef TCCRReg3 TCCRCsReg3;
		#endif		

		// WGM0/1 Regs
		#if defined(WGM30) && defined(WGM31)
		#if defined(WGM32)
		typedef TCCRAReg3 TCCRWgm0Reg3;
		typedef TCCRBReg3 TCCRWgm1Reg3;
		#else
		typedef TCCRReg3 TCCRWgmReg3;
		#endif
		#endif
		
		// IC Regs
		#if defined(ICNC3) && defined(ICES3)
		typedef TCCRBReg3 TCCRIcReg3;
		#endif
		
		// COM regs
		#if defined(TCCR3A)
		typedef TCCRAReg3 TCCRComAReg3;
		typedef TCCRAReg3 TCCRComBReg3;
		typedef TCCRAReg3 TCCRComCReg3;
		#elif defined(TCCR3)
		typedef TCCRReg3 TCCRComAReg3;
		#endif		

		// FOC regs
		#if defined(TCCR3C)
		
		typedef TCCRCReg3 TCCRFocAReg3;
		typedef TCCRCReg3 TCCRFocBReg3;
		typedef TCCRCReg3 TCCRFocCReg3;
		
		#elif defined(FH_TIMER0) || defined(FH_TIMER2)
		// TCCRB or TCCR
		#if defined(TCCR3B)
		typedef TCCRBReg3 TCCRFocAReg3;
		typedef TCCRBReg3 TCCRFocBReg3;
		#elif defined(TCCR3)
		typedef TCCRReg3 TCCRFocAReg3;
		typedef TCCRReg3 TCCRFocBReg3;
		#endif
		
		#elif defined(FH_TIMER1)
		
		// Mega8/32 have it TCCR1A
		typedef TCCRAReg3 TCCRFocAReg3;
		typedef TCCRAReg3 TCCRFocBReg3;
		
		#endif
	}
	
	
	// The big timer
	struct Timer3:
		public AvrTimerCS<priv::TCCRCsReg3, Timer3Cs, Timer3CsMask, priv::TCNTReg3>
		, public InterruptControl<priv::TIMSKReg3, Timer3I>
		, public InterruptFlags<priv::TIFRReg3, Timer3F>
		
		#if defined(WGM30) && defined(WGM31)
		// has WGM. 
		#if defined(WGM32)
		, public AvrTimerWGM2<priv::TCCRWgm0Reg3, priv::TCCRWgm1Reg3, Timer3Wgm, Timer3WgmMask0, Timer3WgmMask1>
		#else
		, public AvrTimerWGM<priv::TCCRWgmReg3, Timer3Wgm, Timer3WgmMask>
		#endif
		#endif
		
		#if defined(ICNC3) && defined(ICES3)
		, public AvrTimerIc<priv::TCCRIcReg3, ICNC3, ICES3, priv::ICRReg3>
		#endif
		
		#if defined(FH_COM3A0) && defined(FH_COM3A1)
		// has A
		, public AvrTimerOcA<priv::TCCRComAReg3, Timer3ComA, Timer3ComAMask, priv::OCRAReg3>
		#endif
		#if defined(FH_FOC3A)
		// can force A
		, public AvrTimerForceOcA<priv::TCCRFocAReg3, FH_FOC3A>
		#endif
		
		#if defined(COM3B0) && defined(COM3B1)
		// has B
		, public AvrTimerOcB<priv::TCCRComBReg3, Timer3ComB, Timer3ComBMask, priv::OCRBReg3>
		#endif
		#if defined(FOC3B)
		// can force B
		, public AvrTimerForceOcB<priv::TCCRFocBReg3, FOC3B>
		#endif
		
		#if defined(COM3C0) && defined(COM3C1)
		// has C
		, public AvrTimerOcC<priv::TCCRComCReg3, Timer3ComC, Timer3ComCMask, priv::OCRCReg3>
		#endif
		#if defined(FOC3C)
		// can force C
		, public AvrTimerForceOcC<priv::TCCRFocCReg3, FOC3C>
		#endif
	{
	};
}


#endif // defined TCCR

#undef FH_TIMER3

#endif /* TIMER0_H_ */
