// Timer template
// Replace @ to timer num

#ifndef AVR_TIMER@_H_
#define AVR_TIMER@_H_

#if defined(TCNT@) && (@ != 1 || (defined(TCCR1A) && defined(TCCR1B)))

#define FH_HAS_TIMER@

// *************** Registers
FH_DECLARE_REGISTER_ONLY(tcnt@, TCNT@)

#if defined(TCCR@)
FH_DECLARE_REGISTER_ONLY(tccr@, TCCR@)		
#endif

#if defined(TCCR@A)
FH_DECLARE_REGISTER_ONLY(tccr@a, TCCR@A)
#endif

#if defined(TCCR@B)
FH_DECLARE_REGISTER_ONLY(tccr@b, TCCR@B)
#endif

#if defined(TCCR@C)
FH_DECLARE_REGISTER_ONLY(tccr@c, TCCR@C)
#endif

#if defined(TIMSK@)
FH_DECLARE_REGISTER_ONLY(timsk@, TIMSK@)
#else
FH_DECLARE_REGISTER_ONLY(timsk@, TIMSK)
#endif

#if defined(TIFR@)
FH_DECLARE_REGISTER_ONLY(tifr@, TIFR@)
#else
FH_DECLARE_REGISTER_ONLY(tifr@, TIFR)
#endif

#if defined(OCR@A)
FH_DECLARE_REGISTER_ONLY(ocr@a, OCR@A)
#elif defined(OCR@)
FH_DECLARE_REGISTER_ONLY(ocr@a, OCR@)		
#endif

#if defined(OCR@B)
FH_DECLARE_REGISTER_ONLY(ocr@b, OCR@B)
#endif

#if defined(OCR@C)
FH_DECLARE_REGISTER_ONLY(ocr@b, OCR@C)
#endif

#if defined(ICR@)
FH_DECLARE_REGISTER_ONLY(icr@, ICR@)
#endif

// ********************* Interrupts
// -------------interrupts - enable
// overflow
static constexpr auto toie@ = fieldBit<TOIE@>(timsk@);

// output compare A
#if defined(OCIE@A)
static constexpr auto ocie@a = fieldBit<OCIE@A>(timsk@);
#elif defined(OCIE@)
static constexpr auto ocie@a = fieldBit<OCIE@>(timsk@);
#endif

// output compare B
#if defined(OCIE@B)
static constexpr auto ocie@b = fieldBit<OCIE@B>(timsk@);
#endif

// output compare C
#if defined(OCIE@C)
static constexpr auto ocie@c = fieldBit<OCIE@C>(timsk@);
#endif

// input cature
#if defined(TICIE@)
static constexpr auto ticie@ = fieldBit<TICIE@>(timsk@);
#endif

// ----------interrupts - flags
// overflow
static constexpr auto tov@ = fieldBit<TOV@>(tifr@);

// output compare A
#if defined(OCF@A)
static constexpr auto ocf@a = fieldBit<OCF@A>(tifr@);
#elif defined(OCF@)
static constexpr auto ocf@a = fieldBit<OCF@>(tifr@);
#endif

// output compare B
#if defined(OCF@B)
static constexpr auto ocf@b = fieldBit<OCF@B>(tifr@);
#endif

// output compare C
#if defined(OCF@C)
static constexpr auto ocf@c = fieldBit<OCF@C>(tifr@);
#endif

// input cature
#if defined(ICF@)
static constexpr auto icf@ = fieldBit<ICF@>(tifr@);
#endif

// enums

// Clock Source
#if (@ == 0 && !defined(__AVR_ATmega128__)) || (@ == 1) || (@ == 2 && defined(__AVR_ATmega128__))
using timer@_cs = timer_csext;
#else
using timer@_cs = timer_cs;
#endif

// Waveform generation mode
#if defined(WGM@0) && defined(WGM@1)
#define FH_HAS_TIMER@_WGM
#if defined(WGM@3)
using timer@_wgm = timer_wgm3;
#elif defined(WGM@2)
using timer@_wgm = timer_wgm2;
#else
using timer@_wgm = timer_wgm1;
#endif
#endif

// Compare mode A
#if defined(COM@0) && defined(COM@1)
#define FH_COM@A0 COM@0
#define FH_COM@A1 COM@1
#elif defined(COM@A0) && defined(COM@A1)
#define FH_COM@A0 COM@A0
#define FH_COM@A1 COM@A1
#endif

#if defined(FOC@A)
#define FH_FOC@A FOC@A
#elif defined(FOC@)
#define FH_FOC@A FOC@
#endif

#if defined(FH_COM@A0) && defined(FH_COM@A1)
FH_DECLARE_TIMER_COM_ENUM(Timer@ComA, FH_COM@A0, FH_COM@A1);
#endif

// compare mode B
#if defined(COM@B0) && defined(COM@B1)
FH_DECLARE_TIMER_COM_ENUM(Timer@ComB, COM@B0, COM@B1);
#endif

// compare mode C
#if defined(COM@C0) && defined(COM@C1)
FH_DECLARE_TIMER_COM_ENUM(Timer@ComC, COM@C0, COM@C1);
#endif

namespace priv
	// CS Reg
	#if defined(TCCR@B)
	typedef TCCRBReg@ TCCRCsReg@;
	#else
	typedef TCCRReg@ TCCRCsReg@;
	#endif		

	// WGM0/1 Regs
	#if defined(WGM@0) && defined(WGM@1)
	#if defined(WGM@2)
	typedef TCCRAReg@ TCCRWgm0Reg@;
	typedef TCCRBReg@ TCCRWgm1Reg@;
	#else
	typedef TCCRReg@ TCCRWgmReg@;
	#endif
	#endif
	
	// IC Regs
	#if defined(ICNC@) && defined(ICES@)
	typedef TCCRBReg@ TCCRIcReg@;
	#endif
	
	// COM regs
	#if defined(TCCR@A)
	typedef TCCRAReg@ TCCRComAReg@;
	typedef TCCRAReg@ TCCRComBReg@;
	typedef TCCRAReg@ TCCRComCReg@;
	#elif defined(TCCR@)
	typedef TCCRReg@ TCCRComAReg@;
	#endif		

	// FOC regs
	#if defined(TCCR@C)
	
	typedef TCCRCReg@ TCCRFocAReg@;
	typedef TCCRCReg@ TCCRFocBReg@;
	typedef TCCRCReg@ TCCRFocCReg@;
	
	#elif defined(FH_TIMER0) || defined(FH_TIMER2)
	// TCCRB or TCCR
	#if defined(TCCR@B)
	typedef TCCRBReg@ TCCRFocAReg@;
	typedef TCCRBReg@ TCCRFocBReg@;
	#elif defined(TCCR@)
	typedef TCCRReg@ TCCRFocAReg@;
	typedef TCCRReg@ TCCRFocBReg@;
	#endif
	
	#elif defined(FH_TIMER1)
	
	// Mega8/32 have it TCCR1A
	typedef TCCRAReg@ TCCRFocAReg@;
	typedef TCCRAReg@ TCCRFocBReg@;
	
	#endif
}


// The big timer
struct Timer@:
	public AvrTimerCS<priv::TCCRCsReg@, Timer@Cs, Timer@CsMask, priv::TCNTReg@>
	, public InterruptControl<priv::TIMSKReg@, Timer@I>
	, public InterruptFlags<priv::TIFRReg@, Timer@F>
	
	#if defined(WGM@0) && defined(WGM@1)
	// has WGM. 
	#if defined(WGM@2)
	, public AvrTimerWGM2<priv::TCCRWgm0Reg@, priv::TCCRWgm1Reg@, Timer@Wgm, Timer@WgmMask0, Timer@WgmMask1>
	#else
	, public AvrTimerWGM<priv::TCCRWgmReg@, Timer@Wgm, Timer@WgmMask>
	#endif
	#endif
	
	#if defined(ICNC@) && defined(ICES@)
	, public AvrTimerIc<priv::TCCRIcReg@, ICNC@, ICES@, priv::ICRReg@>
	#endif
	
	#if defined(FH_COM@A0) && defined(FH_COM@A1)
	// has A
	, public AvrTimerOcA<priv::TCCRComAReg@, Timer@ComA, Timer@ComAMask, priv::OCRAReg@>
	#endif
	#if defined(FH_FOC@A)
	// can force A
	, public AvrTimerForceOcA<priv::TCCRFocAReg@, FH_FOC@A>
	#endif
	
	#if defined(COM@B0) && defined(COM@B1)
	// has B
	, public AvrTimerOcB<priv::TCCRComBReg@, Timer@ComB, Timer@ComBMask, priv::OCRBReg@>
	#endif
	#if defined(FOC@B)
	// can force B
	, public AvrTimerForceOcB<priv::TCCRFocBReg@, FOC@B>
	#endif
	
	#if defined(COM@C0) && defined(COM@C1)
	// has C
	, public AvrTimerOcC<priv::TCCRComCReg@, Timer@ComC, Timer@ComCMask, priv::OCRCReg@>
	#endif
	#if defined(FOC@C)
	// can force C
	, public AvrTimerForceOcC<priv::TCCRFocCReg@, FOC@C>
	#endif
{
};


#endif // defined TCCR

#undef FH_TIMER@

#endif /* TIMER0_H_ */