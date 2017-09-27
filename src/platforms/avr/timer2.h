// Timer template
// Replace 2 to timer num

#ifndef AVR_TIMER2_H_
#define AVR_TIMER2_H_

#define FH_TIMER2

#if defined(TCNT2) && (!defined(FH_TIMER1) || (defined(TCCR1A) && defined(TCCR1B)))

#include "../../utils/functions.h"
#include "timer.h"
#include "interrupts.h"

namespace fasthal{	
	// Clock Source
	#if defined(FH_TIMER0)
	
	#if !defined(__AVR_ATmega128__)
	#define FH_TIMER2_CS_EXT
	#endif
	
	#elif defined(FH_TIMER1)
	
	#define FH_TIMER2_CS_EXT
	
	#elif defined(FH_TIMER2)

	#if defined(__AVR_ATmega128__)
	#define FH_TIMER2_CS_EXT
	#endif

	#endif
	
	#ifdef FH_TIMER2_CS_EXT
	enum class Timer2Cs: uint8_t{
		None    = (0 << CS22) | (0 << CS21) | (0 << CS20),
		P0      = (0 << CS22) | (0 << CS21) | (1 << CS20),
		P8      = (0 << CS22) | (1 << CS21) | (0 << CS20),
		P64     = (0 << CS22) | (1 << CS21) | (1 << CS20),
		P256    = (1 << CS22) | (0 << CS21) | (0 << CS20),
		P1024   = (1 << CS22) | (0 << CS21) | (1 << CS20),
		ExtFall = (1 << CS22) | (1 << CS21) | (0 << CS20),
		ExtRise = (1 << CS22) | (1 << CS21) | (1 << CS20)
	};
	#else
	enum class Timer2Cs: uint8_t{
		None    = (0 << CS22) | (0 << CS21) | (0 << CS20),
		P0      = (0 << CS22) | (0 << CS21) | (1 << CS20),
		P8      = (0 << CS22) | (1 << CS21) | (0 << CS20),
		P32     = (0 << CS22) | (1 << CS21) | (1 << CS20),
		P64     = (1 << CS22) | (0 << CS21) | (0 << CS20),
		P128    = (1 << CS22) | (0 << CS21) | (1 << CS20),
		P256    = (1 << CS22) | (1 << CS21) | (0 << CS20),
		P1024   = (1 << CS22) | (1 << CS21) | (1 << CS20)
	};
	#endif
	const uint8_t Timer2CsMask = (1 << CS22) | (1 << CS21) | (1 << CS20);
	
		
	// Waveform generation mode
	#if defined(WGM20) && defined(WGM21)
	#if defined(WGM23)
	enum class Timer2Wgm: uint8_t{
		Normal     = (0 << WGM23) | (0 << WGM22) | (0 << WGM21) | (0 << WGM20),
		PwmPc8     = (0 << WGM23) | (0 << WGM22) | (0 << WGM21) | (1 << WGM20),
		PwmPc9     = (0 << WGM23) | (0 << WGM22) | (1 << WGM21) | (0 << WGM20),
		PwmPc10    = (0 << WGM23) | (0 << WGM22) | (1 << WGM21) | (1 << WGM20),
		CtcA       = (0 << WGM23) | (1 << WGM22) | (0 << WGM21) | (0 << WGM20),
		PwmFast8   = (0 << WGM23) | (1 << WGM22) | (0 << WGM21) | (1 << WGM20),
		PwmFast9   = (0 << WGM23) | (1 << WGM22) | (1 << WGM21) | (0 << WGM20),
		PwmFast10  = (0 << WGM23) | (1 << WGM22) | (1 << WGM21) | (1 << WGM20),
		PwmPcFcI   = (1 << WGM23) | (0 << WGM22) | (0 << WGM21) | (0 << WGM20),
		PwmPcFcA   = (1 << WGM23) | (0 << WGM22) | (0 << WGM21) | (1 << WGM20),
		PwmPcI     = (1 << WGM23) | (0 << WGM22) | (1 << WGM21) | (0 << WGM20),
		PwmPcA     = (1 << WGM23) | (0 << WGM22) | (1 << WGM21) | (1 << WGM20),
		CtcI       = (1 << WGM23) | (1 << WGM22) | (0 << WGM21) | (0 << WGM20),
		//Reserved = (0 << WGM23) | (1 << WGM22) | (0 << WGM21) | (1 << WGM20),
		PwmFastI   = (1 << WGM23) | (1 << WGM22) | (1 << WGM21) | (0 << WGM20),
		PwmFastA   = (1 << WGM23) | (1 << WGM22) | (1 << WGM21) | (1 << WGM20),
	};
	const uint8_t Timer2WgmMask0 = (1 << WGM21) | (1 << WGM20);
	const uint8_t Timer2WgmMask1 = (1 << WGM23) | (1 << WGM22);
	#elif defined(WGM22)
	enum class Timer2Wgm: uint8_t{
		Normal     = (0 << WGM22) | (0 << WGM21) | (0 << WGM20),
		PwmPcMax   = (0 << WGM22) | (0 << WGM21) | (1 << WGM20),
		CtcA       = (0 << WGM22) | (1 << WGM21) | (0 << WGM20),
		PwmFastMax = (0 << WGM22) | (1 << WGM21) | (1 << WGM20),
		/*Reserved = (1 << WGM22) | (0 << WGM21) | (0 << WGM20),*/
		PwmPcA     = (1 << WGM22) | (0 << WGM21) | (1 << WGM20),
		/*Reserved = (1 << WGM22) | (1 << WGM21) | (0 << WGM20),*/
		PwmFastA   = (1 << WGM22) | (1 << WGM21) | (1 << WGM20)
	};
	const uint8_t Timer2WgmMask0 = (1 << WGM21) | (1 << WGM20);
	const uint8_t Timer2WgmMask1 = (1 << WGM22);
	#else
	enum class Timer2Wgm: uint8_t{
		Normal     = (0 << WGM21) | (0 << WGM20),
		PwmPcMax   = (0 << WGM21) | (1 << WGM20),
		CtcA       = (1 << WGM21) | (0 << WGM20),
		PwmFastMax = (1 < <WGM21) | (1 << WGM20)
	};
	const uint8_t Timer2WgmMask = (1 << WGM21) | (1 << WGM20);
	#endif
	#endif
	
	// Compare mode A
	#if defined(COM20) && defined(COM21)
	#define FH_COM2A0 COM20
	#define FH_COM2A1 COM21
	#elif defined(COM2A0) && defined(COM2A1)
	#define FH_COM2A0 COM2A0
	#define FH_COM2A1 COM2A1
	#endif
	
	#if defined(FOC2A)
	#define FH_FOC2A FOC2A
	#elif defined(FOC2)
	#define FH_FOC2A FOC2
	#endif

	#if defined(FH_COM2A0) && defined(FH_COM2A1)
	FH_TIMER_DECLARE_COM_ENUM(Timer2ComA, FH_COM2A0, FH_COM2A1);
	#endif
	
	// compare mode B
	#if defined(COM2B0) && defined(COM2B1)
	FH_TIMER_DECLARE_COM_ENUM(Timer2ComB, COM2B0, COM2B1);
	#endif
	
	// compare mode C
	#if defined(COM2C0) && defined(COM2C1)
	FH_TIMER_DECLARE_COM_ENUM(Timer2ComC, COM2C0, COM2C1);
	#endif
	
	// interrupts - enable
	enum class Timer2I: uint8_t{
		Overflow = (1 << TOIE2)
		
		#if defined(OCIE2A)
		, OuputCompareA = (1 << OCIE2A)
		#elif defined(OCIE2)
		, OuputCompareA = (1 << OCIE2)
		#endif
		
		#if defined(OCIE2B)
		, OutputCompareB = (1 << OCIE2B)
		#endif
		
		#if defined(OCIE2C)
		, OutputCompareC = (1 << OCIE2C)
		#endif
		
		#if defined(TICIE2)
		, InputCapture = (1 << TICIE2)
		#endif
	};
	
	// interrupts - flag
	enum class Timer2F: uint8_t{
		Overflow = (1 << TOV2)
		
		#if defined(OCF2A)
		, OuputCompareA = (1 << OCF2A)
		#elif defined(OCF2)
		, OuputCompareA = (1 << OCF2)
		#endif
		
		#if defined(OCF2B)
		, OuputCompareB = (1 << OCF2B)
		#endif

		#if defined(OCF2C)
		, OuputCompareC = (1 << OCF2C)
		#endif
		
		#if defined(ICF2)
		, InputCapture = (1 << ICF2)
		#endif
	};


	// wrap registers
	namespace priv{
		FH_WRAPVARIABLE(TCNTReg2, TCNT2)
		
		#if defined(TCCR)
		FH_WRAPVARIABLE(TCCRReg2, TCCR2)		
		#endif

		#if defined(TCCR2A)
		FH_WRAPVARIABLE(TCCRAReg2, TCCR2A)
		#endif
		
		#if defined(TCCR2B)
		FH_WRAPVARIABLE(TCCRBReg2, TCCR2B)
		#endif
		
		#if defined(TCCR2C)
		FH_WRAPVARIABLE(TCCRCReg2, TCCR2C)
		#endif
		
		#if defined(TIMSK2)
		FH_WRAPVARIABLE(TIMSKReg2, TIMSK2)
		#else
		FH_WRAPVARIABLE(TIMSKReg2, TIMSK)
		#endif
		
		#if defined(TIFR2)
		FH_WRAPVARIABLE(TIFRReg2, TIFR2)
		#else
		FH_WRAPVARIABLE(TIFRReg2, TIFR)
		#endif
		
		#if defined(OCR2A)
		FH_WRAPVARIABLE(OCRAReg2, OCR2A)
		#elif defined(OCR2)
		FH_WRAPVARIABLE(OCRAReg2, OCR2)		
		#endif
		
		#if defined(OCR2B)
		FH_WRAPVARIABLE(OCRBReg2, OCR2B)
		#endif
		
		#if defined(OCR2C)
		FH_WRAPVARIABLE(OCRCReg2, OCR2C)
		#endif
		
		#if defined(ICR2)
		FH_WRAPVARIABLE(ICRReg2, ICR2)
		#endif
		
		// CS Reg
		#if defined(TCCR2B)
		typedef TCCRBReg2 TCCRCsReg2;
		#else
		typedef TCCRReg2 TCCRCsReg2;
		#endif		

		// WGM0/1 Regs
		#if defined(WGM20) && defined(WGM21)
		#if defined(WGM22)
		typedef TCCRAReg2 TCCRWgm0Reg2;
		typedef TCCRBReg2 TCCRWgm1Reg2;
		#else
		typedef TCCRReg2 TCCRWgmReg2;
		#endif
		#endif
		
		// IC Regs
		#if defined(ICNC2) && defined(ICES2)
		typedef TCCRBReg2 TCCRIcReg2;
		#endif
		
		// COM regs
		#if defined(TCCR2A)
		typedef TCCRAReg2 TCCRComAReg2;
		typedef TCCRAReg2 TCCRComBReg2;
		typedef TCCRAReg2 TCCRComCReg2;
		#elif defined(TCCR2)
		typedef TCCRReg2 TCCRComAReg2;
		#endif		

		// FOC regs
		#if defined(TCCR2C)
		
		typedef TCCRCReg2 TCCRFocAReg2;
		typedef TCCRCReg2 TCCRFocBReg2;
		typedef TCCRCReg2 TCCRFocCReg2;
		
		#elif defined(FH_TIMER0) || defined(FH_TIMER2)
		// TCCRB or TCCR
		#if defined(TCCR2B)
		typedef TCCRBReg2 TCCRFocAReg2;
		typedef TCCRBReg2 TCCRFocBReg2;
		#elif defined(TCCR2)
		typedef TCCRReg2 TCCRFocAReg2;
		typedef TCCRReg2 TCCRFocBReg2;
		#endif
		
		#elif defined(FH_TIMER1)
		
		// Mega8/32 have it TCCR1A
		typedef TCCRAReg2 TCCRFocAReg2;
		typedef TCCRAReg2 TCCRFocBReg2;
		
		#endif
	}
	
	
	// The big timer
	struct Timer2:
		public AvrTimerCS<priv::TCCRCsReg2, Timer2Cs, Timer2CsMask, priv::TCNTReg2>
		, public InterruptControl<priv::TIMSKReg2, Timer2I>
		, public InterruptFlags<priv::TIFRReg2, Timer2F>
		
		#if defined(WGM20) && defined(WGM21)
		// has WGM. 
		#if defined(WGM22)
		, public AvrTimerWGM2<priv::TCCRWgm0Reg2, priv::TCCRWgm1Reg2, Timer2Wgm, Timer2WgmMask0, Timer2WgmMask1>
		#else
		, public AvrTimerWGM<priv::TCCRWgmReg2, Timer2Wgm, Timer2WgmMask>
		#endif
		#endif
		
		#if defined(ICNC2) && defined(ICES2)
		, public AvrTimerIc<priv::TCCRIcReg2, ICNC2, ICES2, priv::ICRReg2>
		#endif
		
		#if defined(FH_COM2A0) && defined(FH_COM2A1)
		// has A
		, public AvrTimerOcA<priv::TCCRComAReg2, Timer2ComA, Timer2ComAMask, priv::OCRAReg2>
		#endif
		#if defined(FH_FOC2A)
		// can force A
		, public AvrTimerForceOcA<priv::TCCRFocAReg2, FH_FOC2A>
		#endif
		
		#if defined(COM2B0) && defined(COM2B1)
		// has B
		, public AvrTimerOcB<priv::TCCRComBReg2, Timer2ComB, Timer2ComBMask, priv::OCRBReg2>
		#endif
		#if defined(FOC2B)
		// can force B
		, public AvrTimerForceOcB<priv::TCCRFocBReg2, FOC2B>
		#endif
		
		#if defined(COM2C0) && defined(COM2C1)
		// has C
		, public AvrTimerOcC<priv::TCCRComCReg2, Timer2ComC, Timer2ComCMask, priv::OCRCReg2>
		#endif
		#if defined(FOC2C)
		// can force C
		, public AvrTimerForceOcC<priv::TCCRFocCReg2, FOC2C>
		#endif
	{
	};
}


#endif // defined TCCR

#undef FH_TIMER2

#endif /* TIMER0_H_ */
