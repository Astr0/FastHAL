// Timer template
// Replace 5 to timer num

#ifndef AVR_TIMER5_H_
#define AVR_TIMER5_H_

#if defined(TCNT5) && (5 != 1 || (defined(TCCR1A) && defined(TCCR1B)))

#define FH_HAS_TIMER5

// *************** Registers
// counter
FH_DECLARE_REGISTER_ONLY(tcnt5, TCNT5)

// control - general
#if defined(TCCR5)
FH_DECLARE_REGISTER_ONLY(tccr5, TCCR5)		
#endif

// control A
#if defined(TCCR5A)
FH_DECLARE_REGISTER_ONLY(tccr5a, TCCR5A)
#endif

// control B
#if defined(TCCR5B)
FH_DECLARE_REGISTER_ONLY(tccr5b, TCCR5B)
#endif

// control C
#if defined(TCCR5C)
FH_DECLARE_REGISTER_ONLY(tccr5c, TCCR5C)
#endif

// interrupt mask
#if defined(TIMSK5)
FH_DECLARE_REGISTER_ONLY(timsk5, TIMSK5)
#else
FH_DECLARE_REGISTER_ONLY(timsk5, TIMSK)
#endif

// interrupt flags
#if defined(TIFR5)
FH_DECLARE_REGISTER_ONLY(tifr5, TIFR5)
#else
FH_DECLARE_REGISTER_ONLY(tifr5, TIFR)
#endif

// output compare A
#if defined(OCR5A)
#define FH_HAS_TIMER5_OCA
FH_DECLARE_REGISTER_ONLY(ocr5a, OCR5A)
#elif defined(OCR5)
#define FH_HAS_TIMER5_OCA
FH_DECLARE_REGISTER_ONLY(ocr5a, OCR5)		
#endif

// output compare B
#if defined(OCR5B)
#define FH_HAS_TIMER5_OCB
FH_DECLARE_REGISTER_ONLY(ocr5b, OCR5B)
#endif

// output compare C
#if defined(OCR5C)
#define FH_HAS_TIMER5_OCC
FH_DECLARE_REGISTER_ONLY(ocr5b, OCR5C)
#endif

// input capture
#if defined(ICR5)
#define FH_HAS_TIMER5_IC
FH_DECLARE_REGISTER_ONLY(icr5, ICR5)
#endif

// ********************* bits
// input capture noise canceling
#if defined(ICNC5)
static constexpr auto icnc5 = fieldBit<ICNC5>(tccr5b);
#endif 
// input capture edge select
#if defined(ICES5)
static constexpr auto ices5 = fieldBit<ICES5>(tccr5b);
#endif


// ********************* Interrupts

// -------------interrupts - enable
// overflow
static constexpr auto toie5 = fieldBit<TOIE5>(timsk5);

// output compare A
#if defined(OCIE5A)
static constexpr auto ocie5a = fieldBit<OCIE5A>(timsk5);
#elif defined(OCIE5)
static constexpr auto ocie5a = fieldBit<OCIE5>(timsk5);
#endif

// output compare B
#if defined(OCIE5B)
static constexpr auto ocie5b = fieldBit<OCIE5B>(timsk5);
#endif

// output compare C
#if defined(OCIE5C)
static constexpr auto ocie5c = fieldBit<OCIE5C>(timsk5);
#endif

// input cature
#if defined(ICIE5)
static constexpr auto icie5 = fieldBit<ICIE5>(timsk5);
#endif

// ----------interrupts - flags
// overflow
static constexpr auto tov5 = fieldBit<TOV5>(tifr5);

// output compare A
#if defined(OCF5A)
static constexpr auto ocf5a = fieldBit<OCF5A>(tifr5);
#elif defined(OCF5)
static constexpr auto ocf5a = fieldBit<OCF5>(tifr5);
#endif

// output compare B
#if defined(OCF5B)
static constexpr auto ocf5b = fieldBit<OCF5B>(tifr5);
#endif

// output compare C
#if defined(OCF5C)
static constexpr auto ocf5c = fieldBit<OCF5C>(tifr5);
#endif

// input cature
#if defined(ICF5)
static constexpr auto icf5 = fieldBit<ICF5>(tifr5);
#endif

// -------------interrupts - numbers
// overflow
#ifdef TIMER5_OVF_vect_num
#define FH_IRQ_TOV5 TIMER5_OVF_vect_num
static constexpr auto irq_tov5 = interrupt<FH_IRQ_TOV5>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_tov5)>>: 
        func_fieldbit_enable<decltype(toie5)>,
        func_fieldbit_ready_reset<decltype(tov5)> {};
}

#endif

// output compare A
#ifdef TIMER5_COMPA_vect_num
#define FH_IRQ_TOC5A TIMER5_COMPA_vect_num
#elif defined(TIMER5_COMP_vect_num)
#define FH_IRQ_TOC5A TIMER5_COMP_vect_num
#endif

#ifdef FH_IRQ_TOC5A
static constexpr auto irq_toc5a = interrupt<FH_IRQ_TOC5A>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_toc5a)>>: 
        func_fieldbit_enable<decltype(ocie5a)>,
        func_fieldbit_ready_reset<decltype(ocf5a)> {};
}
#endif


// output compare B
#ifdef TIMER5_COMPB_vect_num
#define FH_IRQ_TOC5B TIMER5_COMPB_vect_num
static constexpr auto irq_toc5b = interrupt<FH_IRQ_TOC5B>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_toc5b)>>: 
        func_fieldbit_enable<decltype(ocie5b)>,
        func_fieldbit_ready_reset<decltype(ocf5b)> {};
}
#endif

// output compare B
#ifdef TIMER5_COMPC_vect_num
#define FH_IRQ_TOC5C TIMER5_COMPC_vect_num
static constexpr auto irq_toc5c = interrupt<FH_IRQ_TOC5C>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_toc5c)>>: 
        func_fieldbit_enable<decltype(ocie5c)>,
        func_fieldbit_ready_reset<decltype(ocf5c)> {};
}
#endif

// input capture
#ifdef TIMER5_CAPT_vect_num
#define FH_IRQ_TIC5 TIMER5_CAPT_vect_num
static constexpr auto irq_tic5 = interrupt<FH_IRQ_TIC5>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_tic5)>>: 
        func_fieldbit_enable<decltype(icie5)>,
        func_fieldbit_ready_reset<decltype(icf5)> {};
}
#endif


// ******************************** ENUMS
// Clock Source
#if (5 == 0 && !defined(__AVR_ATmega128__)) || (5 == 1) || (5 == 2 && defined(__AVR_ATmega128__))
using timer5_cs = details::timer_csext;
#else
using timer5_cs = details::timer_cs;
#endif

template<timer5_cs V>
static constexpr auto timer5_cs_v = integral_constant<timer5_cs, V>{};

// Waveform generation mode
#if defined(WGM50) && defined(WGM51)
#define FH_HAS_TIMER5_WGM
#if defined(WGM53)
using timer5_wgm = details::timer_wgm3;
#elif defined(WGM52)
using timer5_wgm = details::timer_wgm2;
#else
using timer5_wgm = details::timer_wgm1;
#endif

template<timer5_wgm V>
static constexpr auto timer5_wgm_v = integral_constant<timer5_wgm, V>{};

#endif



// ******************************* Sepcialization
namespace details{
	namespace priv {	
		// COM regs
		#if defined(TCCR5A)
		static constexpr auto tccr5_oc = tccr5a;
		#elif defined(TCCR5)
		static constexpr auto tccr5_oc = tccr5;
		#endif		

		// FOC regs
		#if defined(TCCR5C)
		static constexpr auto tccr5_foc = tccr5c;
		#elif (5 == 0) || (5 == 2)
		// TCCRB or TCCR
		#if defined(TCCR5B)
		static constexpr auto tccr5_foc = tccr5b;
		#elif defined(TCCR5)
		static constexpr auto tccr5_foc = tccr5;
		#endif	
		#elif (5 == 1)	
		// Mega8/32 have it TCCR1A
		static constexpr auto tccr5_foc = tccr5a;
		#endif
	}

	#ifdef FH_HAS_TIMER5_OCA
	template<>
	struct timer_oc_impl<5, 0>{
		static constexpr auto available = true;	

		static constexpr auto comr = vField<timer_oc>(
		#if defined(COM50) && defined(COM51)
			fieldBit<COM50>(priv::tccr5_oc),
			fieldBit<COM51>(priv::tccr5_oc)
		#else
			fieldBit<COM5A0>(priv::tccr5_oc),
			fieldBit<COM5A1>(priv::tccr5_oc)
		#endif
		);

		static constexpr auto ocr = ocr5a;
		static constexpr auto irq = irq_toc5a;

		#if defined(FOC5A)
		static constexpr auto foc = fieldBit<FOC5A>(priv::tccr5_foc);
		#elif defined(FOC5)
		static constexpr auto foc = fieldBit<FOC5>(priv::tccr5_foc);
		#endif
	};

	#endif

	#ifdef FH_HAS_TIMER5_OCB
	template<>
	struct timer_oc_impl<5, 1>{
		static constexpr auto available = true;	

		static constexpr auto comr = vField<timer_oc>(
			fieldBit<COM5B0>(priv::tccr5_oc),
			fieldBit<COM5B1>(priv::tccr5_oc)
		);

		static constexpr auto ocr = ocr5b;
		static constexpr auto irq = irq_toc5b;

		#if defined(FOC5B)
		static constexpr auto foc = fieldBit<FOC5B>(priv::tccr5_foc);
		#endif
	};
	#endif

	#ifdef FH_HAS_TIMER5_OCC
	template<>
	struct timer_oc_impl<5, 2>{
		static constexpr auto available = true;	

		static constexpr auto comr = vField<timer_oc>(
			fieldBit<COM5C0>(priv::tccr5_oc),
			fieldBit<COM5C1>(priv::tccr5_oc)
		);

		static constexpr auto ocr = ocr5c;
		static constexpr auto irq = irq_toc5c;

		#if defined(FOC5C)
		static constexpr auto foc = fieldBit<FOC5C>(priv::tccr5_foc);
		#endif
	};
	#endif

	template<>
	struct timer_impl<5>{
		static constexpr auto available = true;
		
		static constexpr auto tcnt = tcnt5;
		static constexpr auto irq_tov = irq_tov5;

		// clock source
		using cs_t = timer5_cs;

		// clock source register
		static constexpr auto cs = mField<(1 << CS52) | (1 << CS51) | (1 << CS50) ,cs_t>(
		#if defined(TCCR5B)
		tccr5b
		#else
		tccr5
		#endif
		);

		// waveform generation
		#ifdef FH_HAS_TIMER5_WGM
		using wgm_t = timer5_wgm;
		
		static constexpr auto wgm = vField<wgm_t>(
		#if defined(WGM52)
			fieldBit<WGM50>(tccr5a)
			, fieldBit<WGM51>(tccr5a)
			, fieldBit<WGM52>(tccr5b)
			#if defined(WGM53)
			, fieldBit<WGM53>(tccr5b)
			#endif
		#else
			fieldBit<WGM50>(tccr5)
			, fieldBit<WGM51>(tccr5)
		#endif
		);

		#endif

		// input capture
		#ifdef FH_HAS_TIMER5_IC
		static constexpr auto icr = icr5;
		static constexpr auto icnc = icnc5;
		static constexpr auto ices = ices5;
		
		static constexpr auto irq_tic = irq_tic5;		
		#endif

		// output capture
		template<unsigned VSub>
		static constexpr auto oc = timer_oc_impl<5, VSub>{};

		#ifdef FH_HAS_TIMER5_OCA
		static constexpr auto ocA = oc<0>;
		#endif
		#ifdef FH_HAS_TIMER5_OCB
		static constexpr auto ocB = oc<1>;
		#endif
		#ifdef FH_HAS_TIMER5_OCC
		static constexpr auto ocC = oc<2>;
		#endif
	};
}
static constexpr auto timer5 = details::timer_impl<5>{};


#endif // defined TCCR

#endif /* TIMER0_H_ */
