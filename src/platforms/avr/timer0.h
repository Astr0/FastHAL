// Timer template
// Replace 0 to timer num

#ifndef AVR_TIMER0_H_
#define AVR_TIMER0_H_

#define FH_TIMER0

#if defined(TCNT0) && (!defined(FH_TIMER1) || (defined(TCCR1A) && defined(TCCR1B)))

#include "../../utils/functions.h"
#include "timer.h"
#include "interrupts.h"

namespace fasthal{	
	// Clock Source
	#if defined(FH_TIMER0)
	
	#if !defined(__AVR_ATmega128__)
	#define FH_TIMER0_CS_EXT
	#endif
	
	#elif defined(FH_TIMER1)
	
	#define FH_TIMER0_CS_EXT
	
	#elif defined(FH_TIMER2)

	#if defined(__AVR_ATmega128__)
	#define FH_TIMER0_CS_EXT
	#endif

	#endif
	
	#ifdef FH_TIMER0_CS_EXT
	enum class Timer0Cs: uint8_t{
		None    = (0 << CS02) | (0 << CS01) | (0 << CS00),
		P0      = (0 << CS02) | (0 << CS01) | (1 << CS00),
		P8      = (0 << CS02) | (1 << CS01) | (0 << CS00),
		P64     = (0 << CS02) | (1 << CS01) | (1 << CS00),
		P256    = (1 << CS02) | (0 << CS01) | (0 << CS00),
		P1024   = (1 << CS02) | (0 << CS01) | (1 << CS00),
		ExtFall = (1 << CS02) | (1 << CS01) | (0 << CS00),
		ExtRise = (1 << CS02) | (1 << CS01) | (1 << CS00)
	};
	#else
	enum class Timer0Cs: uint8_t{
		None    = (0 << CS02) | (0 << CS01) | (0 << CS00),
		P0      = (0 << CS02) | (0 << CS01) | (1 << CS00),
		P8      = (0 << CS02) | (1 << CS01) | (0 << CS00),
		P32     = (0 << CS02) | (1 << CS01) | (1 << CS00),
		P64     = (1 << CS02) | (0 << CS01) | (0 << CS00),
		P128    = (1 << CS02) | (0 << CS01) | (1 << CS00),
		P256    = (1 << CS02) | (1 << CS01) | (0 << CS00),
		P1024   = (1 << CS02) | (1 << CS01) | (1 << CS00)
	};
	#endif
	const uint8_t Timer0CsMask = (1 << CS02) | (1 << CS01) | (1 << CS00);
	
		
	// Waveform generation mode
	#if defined(WGM00) && defined(WGM01)
	#if defined(WGM03)
	enum class Timer0Wgm: uint8_t{
		Normal     = (0 << WGM03) | (0 << WGM02) | (0 << WGM01) | (0 << WGM00),
		PwmPc8     = (0 << WGM03) | (0 << WGM02) | (0 << WGM01) | (1 << WGM00),
		PwmPc9     = (0 << WGM03) | (0 << WGM02) | (1 << WGM01) | (0 << WGM00),
		PwmPc10    = (0 << WGM03) | (0 << WGM02) | (1 << WGM01) | (1 << WGM00),
		CtcA       = (0 << WGM03) | (1 << WGM02) | (0 << WGM01) | (0 << WGM00),
		PwmFast8   = (0 << WGM03) | (1 << WGM02) | (0 << WGM01) | (1 << WGM00),
		PwmFast9   = (0 << WGM03) | (1 << WGM02) | (1 << WGM01) | (0 << WGM00),
		PwmFast10  = (0 << WGM03) | (1 << WGM02) | (1 << WGM01) | (1 << WGM00),
		PwmPcFcI   = (1 << WGM03) | (0 << WGM02) | (0 << WGM01) | (0 << WGM00),
		PwmPcFcA   = (1 << WGM03) | (0 << WGM02) | (0 << WGM01) | (1 << WGM00),
		PwmPcI     = (1 << WGM03) | (0 << WGM02) | (1 << WGM01) | (0 << WGM00),
		PwmPcA     = (1 << WGM03) | (0 << WGM02) | (1 << WGM01) | (1 << WGM00),
		CtcI       = (1 << WGM03) | (1 << WGM02) | (0 << WGM01) | (0 << WGM00),
		//Reserved = (0 << WGM03) | (1 << WGM02) | (0 << WGM01) | (1 << WGM00),
		PwmFastI   = (1 << WGM03) | (1 << WGM02) | (1 << WGM01) | (0 << WGM00),
		PwmFastA   = (1 << WGM03) | (1 << WGM02) | (1 << WGM01) | (1 << WGM00),
	};
	const uint8_t Timer0WgmMask0 = (1 << WGM01) | (1 << WGM00);
	const uint8_t Timer0WgmMask1 = (1 << WGM03) | (1 << WGM02);
	#elif defined(WGM02)
	enum class Timer0Wgm: uint8_t{
		Normal     = (0 << WGM02) | (0 << WGM01) | (0 << WGM00),
		PwmPcMax   = (0 << WGM02) | (0 << WGM01) | (1 << WGM00),
		CtcA       = (0 << WGM02) | (1 << WGM01) | (0 << WGM00),
		PwmFastMax = (0 << WGM02) | (1 << WGM01) | (1 << WGM00),
		/*Reserved = (1 << WGM02) | (0 << WGM01) | (0 << WGM00),*/
		PwmPcA     = (1 << WGM02) | (0 << WGM01) | (1 << WGM00),
		/*Reserved = (1 << WGM02) | (1 << WGM01) | (0 << WGM00),*/
		PwmFastA   = (1 << WGM02) | (1 << WGM01) | (1 << WGM00)
	};
	const uint8_t Timer0WgmMask0 = (1 << WGM01) | (1 << WGM00);
	const uint8_t Timer0WgmMask1 = (1 << WGM02);
	#else
	enum class Timer0Wgm: uint8_t{
		Normal     = (0 << WGM01) | (0 << WGM00),
		PwmPcMax   = (0 << WGM01) | (1 << WGM00),
		CtcA       = (1 << WGM01) | (0 << WGM00),
		PwmFastMax = (1 < <WGM01) | (1 << WGM00)
	};
	const uint8_t Timer0WgmMask = (1 << WGM01) | (1 << WGM00);
	#endif
	#endif
	
	// Compare mode A
	#if defined(COM00) && defined(COM01)
	#define FH_COM0A0 COM00
	#define FH_COM0A1 COM01
	#elif defined(COM0A0) && defined(COM0A1)
	#define FH_COM0A0 COM0A0
	#define FH_COM0A1 COM0A1
	#endif
	
	#if defined(FOC0A)
	#define FH_FOC0A FOC0A
	#elif defined(FOC0)
	#define FH_FOC0A FOC0
	#endif

	#if defined(FH_COM0A0) && defined(FH_COM0A1)
	FH_TIMER_DECLARE_COM_ENUM(Timer0ComA, FH_COM0A0, FH_COM0A1);
	#endif
	
	// compare mode B
	#if defined(COM0B0) && defined(COM0B1)
	FH_TIMER_DECLARE_COM_ENUM(Timer0ComB, COM0B0, COM0B1);
	#endif
	
	// compare mode C
	#if defined(COM0C0) && defined(COM0C1)
	FH_TIMER_DECLARE_COM_ENUM(Timer0ComC, COM0C0, COM0C1);
	#endif
	
	// interrupts - enable
	enum class Timer0I: uint8_t{
		Overflow = (1 << TOIE0)
		
		#if defined(OCIE0A)
		, OuputCompareA = (1 << OCIE0A)
		#elif defined(OCIE0)
		, OuputCompareA = (1 << OCIE0)
		#endif
		
		#if defined(OCIE0B)
		, OutputCompareB = (1 << OCIE0B)
		#endif
		
		#if defined(OCIE0C)
		, OutputCompareC = (1 << OCIE0C)
		#endif
		
		#if defined(TICIE0)
		, InputCapture = (1 << TICIE0)
		#endif
	};
	
	// interrupts - flag
	enum class Timer0F: uint8_t{
		Overflow = (1 << TOV0)
		
		#if defined(OCF0A)
		, OuputCompareA = (1 << OCF0A)
		#elif defined(OCF0)
		, OuputCompareA = (1 << OCF0)
		#endif
		
		#if defined(OCF0B)
		, OuputCompareB = (1 << OCF0B)
		#endif

		#if defined(OCF0C)
		, OuputCompareC = (1 << OCF0C)
		#endif
		
		#if defined(ICF0)
		, InputCapture = (1 << ICF0)
		#endif
	};


	// wrap registers
	namespace priv{
		FH_WRAPVARIABLE(TCNTReg0, TCNT0)
		
		#if defined(TCCR)
		FH_WRAPVARIABLE(TCCRReg0, TCCR0)		
		#endif

		#if defined(TCCR0A)
		FH_WRAPVARIABLE(TCCRAReg0, TCCR0A)
		#endif
		
		#if defined(TCCR0B)
		FH_WRAPVARIABLE(TCCRBReg0, TCCR0B)
		#endif
		
		#if defined(TCCR0C)
		FH_WRAPVARIABLE(TCCRCReg0, TCCR0C)
		#endif
		
		#if defined(TIMSK0)
		FH_WRAPVARIABLE(TIMSKReg0, TIMSK0)
		#else
		FH_WRAPVARIABLE(TIMSKReg0, TIMSK)
		#endif
		
		#if defined(TIFR0)
		FH_WRAPVARIABLE(TIFRReg0, TIFR0)
		#else
		FH_WRAPVARIABLE(TIFRReg0, TIFR)
		#endif
		
		#if defined(OCR0A)
		FH_WRAPVARIABLE(OCRAReg0, OCR0A)
		#elif defined(OCR0)
		FH_WRAPVARIABLE(OCRAReg0, OCR0)		
		#endif
		
		#if defined(OCR0B)
		FH_WRAPVARIABLE(OCRBReg0, OCR0B)
		#endif
		
		#if defined(OCR0C)
		FH_WRAPVARIABLE(OCRCReg0, OCR0C)
		#endif
		
		#if defined(ICR0)
		FH_WRAPVARIABLE(ICRReg0, ICR0)
		#endif
		
		// CS Reg
		#if defined(TCCR0B)
		typedef TCCRBReg0 TCCRCsReg0;
		#else
		typedef TCCRReg0 TCCRCsReg0;
		#endif		

		// WGM0/1 Regs
		#if defined(WGM00) && defined(WGM01)
		#if defined(WGM02)
		typedef TCCRAReg0 TCCRWgm0Reg0;
		typedef TCCRBReg0 TCCRWgm1Reg0;
		#else
		typedef TCCRReg0 TCCRWgmReg0;
		#endif
		#endif
		
		// IC Regs
		#if defined(ICNC0) && defined(ICES0)
		typedef TCCRBReg0 TCCRIcReg0;
		#endif
		
		// COM regs
		#if defined(TCCR0A)
		typedef TCCRAReg0 TCCRComAReg0;
		typedef TCCRAReg0 TCCRComBReg0;
		typedef TCCRAReg0 TCCRComCReg0;
		#elif defined(TCCR0)
		typedef TCCRReg0 TCCRComAReg0;
		#endif		

		// FOC regs
		#if defined(TCCR0C)
		
		typedef TCCRCReg0 TCCRFocAReg0;
		typedef TCCRCReg0 TCCRFocBReg0;
		typedef TCCRCReg0 TCCRFocCReg0;
		
		#elif defined(FH_TIMER0) || defined(FH_TIMER2)
		// TCCRB or TCCR
		#if defined(TCCR0B)
		typedef TCCRBReg0 TCCRFocAReg0;
		typedef TCCRBReg0 TCCRFocBReg0;
		#elif defined(TCCR0)
		typedef TCCRReg0 TCCRFocAReg0;
		typedef TCCRReg0 TCCRFocBReg0;
		#endif
		
		#elif defined(FH_TIMER1)
		
		// Mega8/32 have it TCCR1A
		typedef TCCRAReg0 TCCRFocAReg0;
		typedef TCCRAReg0 TCCRFocBReg0;
		
		#endif
	}
	
	
	// The big timer
	struct Timer0:
		public AvrTimerCS<priv::TCCRCsReg0, Timer0Cs, Timer0CsMask, priv::TCNTReg0>
		, public InterruptControl<priv::TIMSKReg0, Timer0I>
		, public InterruptFlags<priv::TIFRReg0, Timer0F>
		
		#if defined(WGM00) && defined(WGM01)
		// has WGM. 
		#if defined(WGM02)
		, public AvrTimerWGM2<priv::TCCRWgm0Reg0, priv::TCCRWgm1Reg0, Timer0Wgm, Timer0WgmMask0, Timer0WgmMask1>
		#else
		, public AvrTimerWGM<priv::TCCRWgmReg0, Timer0Wgm, Timer0WgmMask>
		#endif
		#endif
		
		#if defined(ICNC0) && defined(ICES0)
		, public AvrTimerIc<priv::TCCRIcReg0, ICNC0, ICES0, priv::ICRReg0>
		#endif
		
		#if defined(FH_COM0A0) && defined(FH_COM0A1)
		// has A
		, public AvrTimerOcA<priv::TCCRComAReg0, Timer0ComA, Timer0ComAMask, priv::OCRAReg0>
		#endif
		#if defined(FH_FOC0A)
		// can force A
		, public AvrTimerForceOcA<priv::TCCRFocAReg0, FH_FOC0A>
		#endif
		
		#if defined(COM0B0) && defined(COM0B1)
		// has B
		, public AvrTimerOcB<priv::TCCRComBReg0, Timer0ComB, Timer0ComBMask, priv::OCRBReg0>
		#endif
		#if defined(FOC0B)
		// can force B
		, public AvrTimerForceOcB<priv::TCCRFocBReg0, FOC0B>
		#endif
		
		#if defined(COM0C0) && defined(COM0C1)
		// has C
		, public AvrTimerOcC<priv::TCCRComCReg0, Timer0ComC, Timer0ComCMask, priv::OCRCReg0>
		#endif
		#if defined(FOC0C)
		// can force C
		, public AvrTimerForceOcC<priv::TCCRFocCReg0, FOC0C>
		#endif
	{
	};
}


#endif // defined TCCR

#undef FH_TIMER0

#endif /* TIMER0_H_ */
