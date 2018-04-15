// Timer template
// Replace 0 to timer num

#ifndef AVR_TIMER0_H_
#define AVR_TIMER0_H_

#if defined(TCNT0) && (0 != 1 || (defined(TCCR1A) && defined(TCCR1B)))

#define FH_HAS_TIMER0

// *************** Registers
// counter
FH_DECLARE_REGISTER_ONLY(tcnt0, TCNT0)

// control - general
#if defined(TCCR0)
FH_DECLARE_REGISTER_ONLY(tccr0, TCCR0)		
#endif

// control A
#if defined(TCCR0A)
FH_DECLARE_REGISTER_ONLY(tccr0a, TCCR0A)
#endif

// control B
#if defined(TCCR0B)
FH_DECLARE_REGISTER_ONLY(tccr0b, TCCR0B)
#endif

// control C
#if defined(TCCR0C)
FH_DECLARE_REGISTER_ONLY(tccr0c, TCCR0C)
#endif

// interrupt mask
#if defined(TIMSK0)
FH_DECLARE_REGISTER_ONLY(timsk0, TIMSK0)
#else
FH_DECLARE_REGISTER_ONLY(timsk0, TIMSK)
#endif

// interrupt flags
#if defined(TIFR0)
FH_DECLARE_REGISTER_ONLY(tifr0, TIFR0)
#else
FH_DECLARE_REGISTER_ONLY(tifr0, TIFR)
#endif

// output compare A
#if defined(OCR0A)
#define FH_HAS_TIMER0_OCA
FH_DECLARE_REGISTER_ONLY(ocr0a, OCR0A)
#elif defined(OCR0)
#define FH_HAS_TIMER0_OCA
FH_DECLARE_REGISTER_ONLY(ocr0a, OCR0)		
#endif

// output compare B
#if defined(OCR0B)
#define FH_HAS_TIMER0_OCB
FH_DECLARE_REGISTER_ONLY(ocr0b, OCR0B)
#endif

// output compare C
#if defined(OCR0C)
#define FH_HAS_TIMER0_OCC
FH_DECLARE_REGISTER_ONLY(ocr0c, OCR0C)
#endif

// input capture
#if defined(ICR0)
#define FH_HAS_TIMER0_IC
FH_DECLARE_REGISTER_ONLY(icr0, ICR0)
#endif

// ********************* bits
// input capture noise canceling
#if defined(ICNC0)
static constexpr auto icnc0 = fieldBit<ICNC0>(tccr0b);
#endif 
// input capture edge select
#if defined(ICES0)
static constexpr auto ices0 = fieldBit<ICES0>(tccr0b);
#endif


// ********************* Interrupts

// -------------interrupts - enable
// overflow
static constexpr auto toie0 = fieldBit<TOIE0>(timsk0);

// output compare A
#if defined(OCIE0A)
static constexpr auto ocie0a = fieldBit<OCIE0A>(timsk0);
#elif defined(OCIE0)
static constexpr auto ocie0a = fieldBit<OCIE0>(timsk0);
#endif

// output compare B
#if defined(OCIE0B)
static constexpr auto ocie0b = fieldBit<OCIE0B>(timsk0);
#endif

// output compare C
#if defined(OCIE0C)
static constexpr auto ocie0c = fieldBit<OCIE0C>(timsk0);
#endif

// input cature
#if defined(ICIE0)
static constexpr auto icie0 = fieldBit<ICIE0>(timsk0);
#endif

// ----------interrupts - flags
// overflow
static constexpr auto tov0 = fieldBit<TOV0>(tifr0);

// output compare A
#if defined(OCF0A)
static constexpr auto ocf0a = fieldBit<OCF0A>(tifr0);
#elif defined(OCF0)
static constexpr auto ocf0a = fieldBit<OCF0>(tifr0);
#endif

// output compare B
#if defined(OCF0B)
static constexpr auto ocf0b = fieldBit<OCF0B>(tifr0);
#endif

// output compare C
#if defined(OCF0C)
static constexpr auto ocf0c = fieldBit<OCF0C>(tifr0);
#endif

// input cature
#if defined(ICF0)
static constexpr auto icf0 = fieldBit<ICF0>(tifr0);
#endif

// -------------interrupts - numbers
// overflow
#ifdef TIMER0_OVF_vect_num
#define FH_IRQ_TOV0 TIMER0_OVF_vect_num
static constexpr auto irq_tov0 = interrupt<FH_IRQ_TOV0>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_tov0)>>: 
        func_fieldbit_enable<decltype(toie0)>,
        func_fieldbit_ready_reset<decltype(tov0)> {};
}

#endif

// output compare A
#ifdef TIMER0_COMPA_vect_num
#define FH_IRQ_TOC0A TIMER0_COMPA_vect_num
#elif defined(TIMER0_COMP_vect_num)
#define FH_IRQ_TOC0A TIMER0_COMP_vect_num
#endif

#ifdef FH_IRQ_TOC0A
static constexpr auto irq_toc0a = interrupt<FH_IRQ_TOC0A>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_toc0a)>>: 
        func_fieldbit_enable<decltype(ocie0a)>,
        func_fieldbit_ready_reset<decltype(ocf0a)> {};
}
#endif


// output compare B
#ifdef TIMER0_COMPB_vect_num
#define FH_IRQ_TOC0B TIMER0_COMPB_vect_num
static constexpr auto irq_toc0b = interrupt<FH_IRQ_TOC0B>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_toc0b)>>: 
        func_fieldbit_enable<decltype(ocie0b)>,
        func_fieldbit_ready_reset<decltype(ocf0b)> {};
}
#endif

// output compare B
#ifdef TIMER0_COMPC_vect_num
#define FH_IRQ_TOC0C TIMER0_COMPC_vect_num
static constexpr auto irq_toc0c = interrupt<FH_IRQ_TOC0C>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_toc0c)>>: 
        func_fieldbit_enable<decltype(ocie0c)>,
        func_fieldbit_ready_reset<decltype(ocf0c)> {};
}
#endif

// input capture
#ifdef TIMER0_CAPT_vect_num
#define FH_IRQ_TIC0 TIMER0_CAPT_vect_num
static constexpr auto irq_tic0 = interrupt<FH_IRQ_TIC0>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_tic0)>>: 
        func_fieldbit_enable<decltype(icie0)>,
        func_fieldbit_ready_reset<decltype(icf0)> {};
}
#endif


// ******************************** ENUMS
// Clock Source
#if (0 == 0 && !defined(__AVR_ATmega128__)) || (0 == 1) || (0 == 2 && defined(__AVR_ATmega128__))
using timer0_cs = details::timer_csext;
#else
using timer0_cs = details::timer_cs;
#endif

template<timer0_cs V>
static constexpr auto timer0_cs_v = integral_constant<timer0_cs, V>{};

// Waveform generation mode
#if defined(WGM00) && defined(WGM01)
#define FH_HAS_TIMER0_WGM
#if defined(WGM03)
using timer0_wgm = details::timer_wgm3;
#elif defined(WGM02)
using timer0_wgm = details::timer_wgm2;
#else
using timer0_wgm = details::timer_wgm1;
#endif

template<timer0_wgm V>
static constexpr auto timer0_wgm_v = integral_constant<timer0_wgm, V>{};

#endif



// ******************************* Sepcialization
namespace details{
	namespace priv {	
		// COM regs
		#if defined(TCCR0A)
		static constexpr auto tccr0_oc = tccr0a;
		#elif defined(TCCR0)
		static constexpr auto tccr0_oc = tccr0;
		#endif		

		// FOC regs
		#if defined(TCCR0C)
		static constexpr auto tccr0_foc = tccr0c;
		#elif (0 == 0) || (0 == 2)
		// TCCRB or TCCR
		#if defined(TCCR0B)
		static constexpr auto tccr0_foc = tccr0b;
		#elif defined(TCCR0)
		static constexpr auto tccr0_foc = tccr0;
		#endif	
		#elif (0 == 1)	
		// Mega8/32 have it TCCR1A
		static constexpr auto tccr0_foc = tccr0a;
		#endif
	}

	#ifdef FH_HAS_TIMER0_OCA
	template<>
	struct timer_oc_impl<0, 0>{
		static constexpr auto available = true;	

		static constexpr auto comr = vField<timer_oc>(
		#if defined(COM00) && defined(COM01)
			fieldBit<COM00>(priv::tccr0_oc),
			fieldBit<COM01>(priv::tccr0_oc)
		#else
			fieldBit<COM0A0>(priv::tccr0_oc),
			fieldBit<COM0A1>(priv::tccr0_oc)
		#endif
		);

		static constexpr auto ocr = ocr0a;
		static constexpr auto irq = irq_toc0a;

		#if defined(FOC0A)
		static constexpr auto foc = fieldBit<FOC0A>(priv::tccr0_foc);
		#elif defined(FOC0)
		static constexpr auto foc = fieldBit<FOC0>(priv::tccr0_foc);
		#endif
	};

	#endif

	#ifdef FH_HAS_TIMER0_OCB
	template<>
	struct timer_oc_impl<0, 1>{
		static constexpr auto available = true;	

		static constexpr auto comr = vField<timer_oc>(
			fieldBit<COM0B0>(priv::tccr0_oc),
			fieldBit<COM0B1>(priv::tccr0_oc)
		);

		static constexpr auto ocr = ocr0b;
		static constexpr auto irq = irq_toc0b;

		#if defined(FOC0B)
		static constexpr auto foc = fieldBit<FOC0B>(priv::tccr0_foc);
		#endif
	};
	#endif

	#ifdef FH_HAS_TIMER0_OCC
	template<>
	struct timer_oc_impl<0, 2>{
		static constexpr auto available = true;	

		static constexpr auto comr = vField<timer_oc>(
			fieldBit<COM0C0>(priv::tccr0_oc),
			fieldBit<COM0C1>(priv::tccr0_oc)
		);

		static constexpr auto ocr = ocr0c;
		static constexpr auto irq = irq_toc0c;

		#if defined(FOC0C)
		static constexpr auto foc = fieldBit<FOC0C>(priv::tccr0_foc);
		#endif
	};
	#endif

	template<>
	struct timer_impl<0>{
		static constexpr auto available = true;
		
		static constexpr auto tcnt = tcnt0;
		static constexpr auto irq_tov = irq_tov0;

		// clock source
		using cs_t = timer0_cs;

		// clock source register
		static constexpr auto cs = mField<(1 << CS02) | (1 << CS01) | (1 << CS00) ,cs_t>(
		#if defined(TCCR0B)
		tccr0b
		#else
		tccr0
		#endif
		);

		// waveform generation
		#ifdef FH_HAS_TIMER0_WGM
		using wgm_t = timer0_wgm;
		
		static constexpr auto wgm = vField<wgm_t>(
		#if defined(WGM02)
			fieldBit<WGM00>(tccr0a)
			, fieldBit<WGM01>(tccr0a)
			, fieldBit<WGM02>(tccr0b)
			#if defined(WGM03)
			, fieldBit<WGM03>(tccr0b)
			#endif
		#else
			fieldBit<WGM00>(tccr0)
			, fieldBit<WGM01>(tccr0)
		#endif
		);

		#endif

		// input capture
		#ifdef FH_HAS_TIMER0_IC
		static constexpr auto icr = icr0;
		static constexpr auto icnc = icnc0;
		static constexpr auto ices = ices0;
		
		static constexpr auto irq_tic = irq_tic0;		
		#endif

		// output capture
		template<unsigned VSub>
		static constexpr auto oc = timer_oc_impl<0, VSub>{};

		#ifdef FH_HAS_TIMER0_OCA
		static constexpr auto ocA = oc<0>;
		#endif
		#ifdef FH_HAS_TIMER0_OCB
		static constexpr auto ocB = oc<1>;
		#endif
		#ifdef FH_HAS_TIMER0_OCC
		static constexpr auto ocC = oc<2>;
		#endif
	};
}
// static constexpr auto timer0 = details::timer_impl<0>{};


#endif // defined TCCR

#endif /* TIMER0_H_ */
