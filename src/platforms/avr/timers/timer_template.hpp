// Timer template
// Replace @ to timer num

#ifndef AVR_TIMER@_H_
#define AVR_TIMER@_H_

#if defined(TCNT@) && (@ != 1 || (defined(TCCR1A) && defined(TCCR1B)))

#define FH_HAS_TIMER@

// *************** Registers
// counter
FH_DECLARE_REGISTER_ONLY(tcnt@, TCNT@)

// control - general
#if defined(TCCR@)
FH_DECLARE_REGISTER_ONLY(tccr@, TCCR@)		
#endif

// control A
#if defined(TCCR@A)
FH_DECLARE_REGISTER_ONLY(tccr@a, TCCR@A)
#endif

// control B
#if defined(TCCR@B)
FH_DECLARE_REGISTER_ONLY(tccr@b, TCCR@B)
#endif

// control C
#if defined(TCCR@C)
FH_DECLARE_REGISTER_ONLY(tccr@c, TCCR@C)
#endif

// interrupt mask
#if defined(TIMSK@)
FH_DECLARE_REGISTER_ONLY(timsk@, TIMSK@)
#else
FH_DECLARE_REGISTER_ONLY(timsk@, TIMSK)
#endif

// interrupt flags
#if defined(TIFR@)
FH_DECLARE_REGISTER_ONLY(tifr@, TIFR@)
#else
FH_DECLARE_REGISTER_ONLY(tifr@, TIFR)
#endif

// output compare A
#if defined(OCR@A)
#define FH_HAS_TIMER@_OCA
FH_DECLARE_REGISTER_ONLY(ocr@a, OCR@A)
#elif defined(OCR@)
#define FH_HAS_TIMER@_OCA
FH_DECLARE_REGISTER_ONLY(ocr@a, OCR@)		
#endif

// output compare B
#if defined(OCR@B)
#define FH_HAS_TIMER@_OCB
FH_DECLARE_REGISTER_ONLY(ocr@b, OCR@B)
#endif

// output compare C
#if defined(OCR@C)
#define FH_HAS_TIMER@_OCC
FH_DECLARE_REGISTER_ONLY(ocr@b, OCR@C)
#endif

// input capture
#if defined(ICR@)
#define FH_HAS_TIMER@_IC
FH_DECLARE_REGISTER_ONLY(icr@, ICR@)
#endif

// ********************* bits
// input capture noise canceling
#if defined(ICNC@)
static constexpr auto icnc@ = fieldBit<ICNC@>(tccr@b);
#endif 
// input capture edge select
#if defined(ICES@)
static constexpr auto ices@ = fieldBit<ICES@>(tccr@b);
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

// ******************************** ENUMS
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



// ******************************* Sepcialization
template<>
struct timer<@>{
	static constexpr auto available = true;
	
	static constexpr auto tcnt = tcnt@;
	static constexpr auto toie = toie@;
	static constexpr auto tov = tov@;

	// clock source
	using cs_t = timer@_cs;

	// clock source register
	static constexpr auto csr = mField<(1 << CS@2) | (1 << CS@1) | (1 << CS@0) ,cs_t>(
	#if defined(TCCR@B)
	tccr@b
	#else
	tccr@
	#endif
	);

	// waveform generation
	#ifdef FH_TIMER@_WGM
	using wgm_t = timer@_wgm;
	
	static constexpr auto wgmr = vField<wgm_t>(
	#if defined(WGM@2)
		fieldBit<WGM@0>(tccr@a)
		, fieldBit<WGM@1>(tccr@a)
		, fieldBit<WGM@2>(tccr@b)
		#if defined(WGM@3)
		, fieldBit<WGM@3>(tccr@b)
		#endif
	#else
		fieldBit<WGM@0>(tccr@)
		, fieldBit<WGM@1>(tccr@)
	#endif
	);

	#endif

	// input capture
	#ifdef FH_TIMER@_IC
	static constexpr auto icr = icr@;
	static constexpr auto icnc = icnc@;
	static constexpr auto ices = ices@;
	#endif
};


namespace priv {	
	// COM regs
	#if defined(TCCR@A)
	static constexpr auto tccr@_oc = tccr@a;
	#elif defined(TCCR@)
	static constexpr auto tccr@_oc = tccr@;
	#endif		

	// FOC regs
	#if defined(TCCR@C)
	static constexpr auto tccr@_foc = tccr@c;
	#elif (@ == 0) || (@ == 2)
	// TCCRB or TCCR
	#if defined(TCCR@B)
	static constexpr auto tccr@_foc = tccr@b;
	#elif defined(TCCR@)
	static constexpr auto tccr@_foc = tccr@;
	#endif	
	#elif (@ == 1)	
	// Mega8/32 have it TCCR1A
	static constexpr auto tccr@_foc = tccr@a;
	#endif
}

#ifdef FH_TIMER@_OCA;
template<>
struct timer_oc<@, 0>{
	static constexpr auto available = true;	

	static constexpr auto comr = vField<timer_com>(
	#if defined(COM@0) && defined(COM@1)
		fieldBit<COM@0>(priv::tccr@_oc),
		fieldBit<COM@1>(priv::tccr@_oc)
	#else
		fieldBit<COM@A0>(priv::tccr@_oc),
		fieldBit<COM@A1>(priv::tccr@_oc)
	#endif
	);

	static constexpr auto ocr = ocr@a;
	static constexpr auto ocie = ocie@a;
	static constexpr auto ocf = ocf@a;


	#if defined(FOC@A)
	static constexpr auto foc = fieldBit<FOC@A>(tccr@_foc);
	#elif defined(FOC@)
	static constexpr auto foc = fieldBit<FOC@>(tccr@_foc);
	#endif
};
#endif

#ifdef FH_TIMER@_OCB;
template<>
struct timer_oc<@, 1>{
	static constexpr auto available = true;	

	static constexpr auto comr = vField<timer_com>(
		fieldBit<COM@B0>(priv::tccr@_oc),
		fieldBit<COM@B1>(priv::tccr@_oc)
	);

	static constexpr auto ocr = ocr@b;
	static constexpr auto ocie = ocie@b;
	static constexpr auto ocf = ocf@b;

	#if defined(FOC@B)
	static constexpr auto foc = fieldBit<FOC@B>(tccr@_foc);
	#endif
};
#endif

#ifdef FH_TIMER@_OCC;
template<>
struct timer_oc<@, 2>{
	static constexpr auto available = true;	

	static constexpr auto comr = vField<timer_com>(
		fieldBit<COM@C0>(priv::tccr@_oc),
		fieldBit<COM@C1>(priv::tccr@_oc)
	);

	static constexpr auto ocr = ocr@c;
	static constexpr auto ocie = ocie@c;
	static constexpr auto ocf = ocf@c;

	#if defined(FOC@C)
	static constexpr auto foc = fieldBit<FOC@C>(tccr@_foc);
	#endif
};
#endif


#endif // defined TCCR

#endif /* TIMER0_H_ */