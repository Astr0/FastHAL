// Timer template
// Replace 4 to timer num

#ifndef AVR_TIMER4_H_
#define AVR_TIMER4_H_

#define FH_TIMER4

#if defined(TCNT4) && (!defined(FH_TIMER1) || (defined(TCCR1A) && defined(TCCR1B)))

#include "../../utils/functions.h"
#include "timer.h"
#include "interrupts.h"

namespace fasthal{	
	// Clock Source
	#if defined(FH_TIMER0)
	
	#if !defined(__AVR_ATmega128__)
	#define FH_TIMER4_CS_EXT
	#endif
	
	#elif defined(FH_TIMER1)
	
	#define FH_TIMER4_CS_EXT
	
	#elif defined(FH_TIMER2)

	#if defined(__AVR_ATmega128__)
	#define FH_TIMER4_CS_EXT
	#endif

	#endif
	
	#ifdef FH_TIMER4_CS_EXT
	enum class Timer4Cs: uint8_t{
		None    = (0 << CS42) | (0 << CS41) | (0 << CS40),
		P0      = (0 << CS42) | (0 << CS41) | (1 << CS40),
		P8      = (0 << CS42) | (1 << CS41) | (0 << CS40),
		P64     = (0 << CS42) | (1 << CS41) | (1 << CS40),
		P256    = (1 << CS42) | (0 << CS41) | (0 << CS40),
		P1024   = (1 << CS42) | (0 << CS41) | (1 << CS40),
		ExtFall = (1 << CS42) | (1 << CS41) | (0 << CS40),
		ExtRise = (1 << CS42) | (1 << CS41) | (1 << CS40)
	};
	#else
	enum class Timer4Cs: uint8_t{
		None    = (0 << CS42) | (0 << CS41) | (0 << CS40),
		P0      = (0 << CS42) | (0 << CS41) | (1 << CS40),
		P8      = (0 << CS42) | (1 << CS41) | (0 << CS40),
		P32     = (0 << CS42) | (1 << CS41) | (1 << CS40),
		P64     = (1 << CS42) | (0 << CS41) | (0 << CS40),
		P128    = (1 << CS42) | (0 << CS41) | (1 << CS40),
		P256    = (1 << CS42) | (1 << CS41) | (0 << CS40),
		P1024   = (1 << CS42) | (1 << CS41) | (1 << CS40)
	};
	#endif
	const uint8_t Timer4CsMask = (1 << CS42) | (1 << CS41) | (1 << CS40);
	
		
	// Waveform generation mode
	#if defined(WGM40) && defined(WGM41)
	#if defined(WGM43)
	enum class Timer4Wgm: uint8_t{
		Normal     = (0 << WGM43) | (0 << WGM42) | (0 << WGM41) | (0 << WGM40),
		PwmPc8     = (0 << WGM43) | (0 << WGM42) | (0 << WGM41) | (1 << WGM40),
		PwmPc9     = (0 << WGM43) | (0 << WGM42) | (1 << WGM41) | (0 << WGM40),
		PwmPc10    = (0 << WGM43) | (0 << WGM42) | (1 << WGM41) | (1 << WGM40),
		CtcA       = (0 << WGM43) | (1 << WGM42) | (0 << WGM41) | (0 << WGM40),
		PwmFast8   = (0 << WGM43) | (1 << WGM42) | (0 << WGM41) | (1 << WGM40),
		PwmFast9   = (0 << WGM43) | (1 << WGM42) | (1 << WGM41) | (0 << WGM40),
		PwmFast10  = (0 << WGM43) | (1 << WGM42) | (1 << WGM41) | (1 << WGM40),
		PwmPcFcI   = (1 << WGM43) | (0 << WGM42) | (0 << WGM41) | (0 << WGM40),
		PwmPcFcA   = (1 << WGM43) | (0 << WGM42) | (0 << WGM41) | (1 << WGM40),
		PwmPcI     = (1 << WGM43) | (0 << WGM42) | (1 << WGM41) | (0 << WGM40),
		PwmPcA     = (1 << WGM43) | (0 << WGM42) | (1 << WGM41) | (1 << WGM40),
		CtcI       = (1 << WGM43) | (1 << WGM42) | (0 << WGM41) | (0 << WGM40),
		//Reserved = (0 << WGM43) | (1 << WGM42) | (0 << WGM41) | (1 << WGM40),
		PwmFastI   = (1 << WGM43) | (1 << WGM42) | (1 << WGM41) | (0 << WGM40),
		PwmFastA   = (1 << WGM43) | (1 << WGM42) | (1 << WGM41) | (1 << WGM40),
	};
	const uint8_t Timer4WgmMask0 = (1 << WGM41) | (1 << WGM40);
	const uint8_t Timer4WgmMask1 = (1 << WGM43) | (1 << WGM42);
	#elif defined(WGM42)
	enum class Timer4Wgm: uint8_t{
		Normal     = (0 << WGM42) | (0 << WGM41) | (0 << WGM40),
		PwmPcMax   = (0 << WGM42) | (0 << WGM41) | (1 << WGM40),
		CtcA       = (0 << WGM42) | (1 << WGM41) | (0 << WGM40),
		PwmFastMax = (0 << WGM42) | (1 << WGM41) | (1 << WGM40),
		/*Reserved = (1 << WGM42) | (0 << WGM41) | (0 << WGM40),*/
		PwmPcA     = (1 << WGM42) | (0 << WGM41) | (1 << WGM40),
		/*Reserved = (1 << WGM42) | (1 << WGM41) | (0 << WGM40),*/
		PwmFastA   = (1 << WGM42) | (1 << WGM41) | (1 << WGM40)
	};
	const uint8_t Timer4WgmMask0 = (1 << WGM41) | (1 << WGM40);
	const uint8_t Timer4WgmMask1 = (1 << WGM42);
	#else
	enum class Timer4Wgm: uint8_t{
		Normal     = (0 << WGM41) | (0 << WGM40),
		PwmPcMax   = (0 << WGM41) | (1 << WGM40),
		CtcA       = (1 << WGM41) | (0 << WGM40),
		PwmFastMax = (1 < <WGM41) | (1 << WGM40)
	};
	const uint8_t Timer4WgmMask = (1 << WGM41) | (1 << WGM40);
	#endif
	#endif
	
	// Compare mode A
	#if defined(COM40) && defined(COM41)
	#define FH_COM4A0 COM40
	#define FH_COM4A1 COM41
	#elif defined(COM4A0) && defined(COM4A1)
	#define FH_COM4A0 COM4A0
	#define FH_COM4A1 COM4A1
	#endif
	
	#if defined(FOC4A)
	#define FH_FOC4A FOC4A
	#elif defined(FOC4)
	#define FH_FOC4A FOC4
	#endif

	#if defined(FH_COM4A0) && defined(FH_COM4A1)
	FH_TIMER_DECLARE_COM_ENUM(Timer4ComA, FH_COM4A0, FH_COM4A1);
	#endif
	
	// compare mode B
	#if defined(COM4B0) && defined(COM4B1)
	FH_TIMER_DECLARE_COM_ENUM(Timer4ComB, COM4B0, COM4B1);
	#endif
	
	// compare mode C
	#if defined(COM4C0) && defined(COM4C1)
	FH_TIMER_DECLARE_COM_ENUM(Timer4ComC, COM4C0, COM4C1);
	#endif
	
	// interrupts - enable
	enum class Timer4I: uint8_t{
		Overflow = (1 << TOIE4)
		
		#if defined(OCIE4A)
		, OuputCompareA = (1 << OCIE4A)
		#elif defined(OCIE4)
		, OuputCompareA = (1 << OCIE4)
		#endif
		
		#if defined(OCIE4B)
		, OutputCompareB = (1 << OCIE4B)
		#endif
		
		#if defined(OCIE4C)
		, OutputCompareC = (1 << OCIE4C)
		#endif
		
		#if defined(TICIE4)
		, InputCapture = (1 << TICIE4)
		#endif
	};
	
	// interrupts - flag
	enum class Timer4F: uint8_t{
		Overflow = (1 << TOV4)
		
		#if defined(OCF4A)
		, OuputCompareA = (1 << OCF4A)
		#elif defined(OCF4)
		, OuputCompareA = (1 << OCF4)
		#endif
		
		#if defined(OCF4B)
		, OuputCompareB = (1 << OCF4B)
		#endif

		#if defined(OCF4C)
		, OuputCompareC = (1 << OCF4C)
		#endif
		
		#if defined(ICF4)
		, InputCapture = (1 << ICF4)
		#endif
	};


	// wrap registers
	namespace priv{
		FH_WRAPVARIABLE(TCNTReg4, TCNT4)
		
		#if defined(TCCR)
		FH_WRAPVARIABLE(TCCRReg4, TCCR4)		
		#endif

		#if defined(TCCR4A)
		FH_WRAPVARIABLE(TCCRAReg4, TCCR4A)
		#endif
		
		#if defined(TCCR4B)
		FH_WRAPVARIABLE(TCCRBReg4, TCCR4B)
		#endif
		
		#if defined(TCCR4C)
		FH_WRAPVARIABLE(TCCRCReg4, TCCR4C)
		#endif
		
		#if defined(TIMSK4)
		FH_WRAPVARIABLE(TIMSKReg4, TIMSK4)
		#else
		FH_WRAPVARIABLE(TIMSKReg4, TIMSK)
		#endif
		
		#if defined(TIFR4)
		FH_WRAPVARIABLE(TIFRReg4, TIFR4)
		#else
		FH_WRAPVARIABLE(TIFRReg4, TIFR)
		#endif
		
		#if defined(OCR4A)
		FH_WRAPVARIABLE(OCRAReg4, OCR4A)
		#elif defined(OCR4)
		FH_WRAPVARIABLE(OCRAReg4, OCR4)		
		#endif
		
		#if defined(OCR4B)
		FH_WRAPVARIABLE(OCRBReg4, OCR4B)
		#endif
		
		#if defined(OCR4C)
		FH_WRAPVARIABLE(OCRCReg4, OCR4C)
		#endif
		
		#if defined(ICR4)
		FH_WRAPVARIABLE(ICRReg4, ICR4)
		#endif
		
		// CS Reg
		#if defined(TCCR4B)
		typedef TCCRBReg4 TCCRCsReg4;
		#else
		typedef TCCRReg4 TCCRCsReg4;
		#endif		

		// WGM0/1 Regs
		#if defined(WGM40) && defined(WGM41)
		#if defined(WGM42)
		typedef TCCRAReg4 TCCRWgm0Reg4;
		typedef TCCRBReg4 TCCRWgm1Reg4;
		#else
		typedef TCCRReg4 TCCRWgmReg4;
		#endif
		#endif
		
		// IC Regs
		#if defined(ICNC4) && defined(ICES4)
		typedef TCCRBReg4 TCCRIcReg4;
		#endif
		
		// COM regs
		#if defined(TCCR4A)
		typedef TCCRAReg4 TCCRComAReg4;
		typedef TCCRAReg4 TCCRComBReg4;
		typedef TCCRAReg4 TCCRComCReg4;
		#elif defined(TCCR4)
		typedef TCCRReg4 TCCRComAReg4;
		#endif		

		// FOC regs
		#if defined(TCCR4C)
		
		typedef TCCRCReg4 TCCRFocAReg4;
		typedef TCCRCReg4 TCCRFocBReg4;
		typedef TCCRCReg4 TCCRFocCReg4;
		
		#elif defined(FH_TIMER0) || defined(FH_TIMER2)
		// TCCRB or TCCR
		#if defined(TCCR4B)
		typedef TCCRBReg4 TCCRFocAReg4;
		typedef TCCRBReg4 TCCRFocBReg4;
		#elif defined(TCCR4)
		typedef TCCRReg4 TCCRFocAReg4;
		typedef TCCRReg4 TCCRFocBReg4;
		#endif
		
		#elif defined(FH_TIMER1)
		
		// Mega8/32 have it TCCR1A
		typedef TCCRAReg4 TCCRFocAReg4;
		typedef TCCRAReg4 TCCRFocBReg4;
		
		#endif
	}
	
	
	// The big timer
	struct Timer4:
		public AvrTimerCS<priv::TCCRCsReg4, Timer4Cs, Timer4CsMask, priv::TCNTReg4>
		, public InterruptControl<priv::TIMSKReg4, Timer4I>
		, public InterruptFlags<priv::TIFRReg4, Timer4F>
		
		#if defined(WGM40) && defined(WGM41)
		// has WGM. 
		#if defined(WGM42)
		, public AvrTimerWGM2<priv::TCCRWgm0Reg4, priv::TCCRWgm1Reg4, Timer4Wgm, Timer4WgmMask0, Timer4WgmMask1>
		#else
		, public AvrTimerWGM<priv::TCCRWgmReg4, Timer4Wgm, Timer4WgmMask>
		#endif
		#endif
		
		#if defined(ICNC4) && defined(ICES4)
		, public AvrTimerIc<priv::TCCRIcReg4, ICNC4, ICES4, priv::ICRReg4>
		#endif
		
		#if defined(FH_COM4A0) && defined(FH_COM4A1)
		// has A
		, public AvrTimerOcA<priv::TCCRComAReg4, Timer4ComA, Timer4ComAMask, priv::OCRAReg4>
		#endif
		#if defined(FH_FOC4A)
		// can force A
		, public AvrTimerForceOcA<priv::TCCRFocAReg4, FH_FOC4A>
		#endif
		
		#if defined(COM4B0) && defined(COM4B1)
		// has B
		, public AvrTimerOcB<priv::TCCRComBReg4, Timer4ComB, Timer4ComBMask, priv::OCRBReg4>
		#endif
		#if defined(FOC4B)
		// can force B
		, public AvrTimerForceOcB<priv::TCCRFocBReg4, FOC4B>
		#endif
		
		#if defined(COM4C0) && defined(COM4C1)
		// has C
		, public AvrTimerOcC<priv::TCCRComCReg4, Timer4ComC, Timer4ComCMask, priv::OCRCReg4>
		#endif
		#if defined(FOC4C)
		// can force C
		, public AvrTimerForceOcC<priv::TCCRFocCReg4, FOC4C>
		#endif
	{
	};
}


#endif // defined TCCR

#undef FH_TIMER4

#endif /* TIMER0_H_ */
