// Timer template
// Replace 4 to timer num

#ifndef AVR_TIMER4_H_
#define AVR_TIMER4_H_

#if defined(TCNT4) && (4 != 1 || (defined(TCCR1A) && defined(TCCR1B)))

#define FH_HAS_TIMER4

// *************** Registers
// counter
FH_DECLARE_REGISTER_ONLY(tcnt4, TCNT4)

// control - general
#if defined(TCCR4)
FH_DECLARE_REGISTER_ONLY(tccr4, TCCR4)		
#endif

// control A
#if defined(TCCR4A)
FH_DECLARE_REGISTER_ONLY(tccr4a, TCCR4A)
#endif

// control B
#if defined(TCCR4B)
FH_DECLARE_REGISTER_ONLY(tccr4b, TCCR4B)
#endif

// control C
#if defined(TCCR4C)
FH_DECLARE_REGISTER_ONLY(tccr4c, TCCR4C)
#endif

// interrupt mask
#if defined(TIMSK4)
FH_DECLARE_REGISTER_ONLY(timsk4, TIMSK4)
#else
FH_DECLARE_REGISTER_ONLY(timsk4, TIMSK)
#endif

// interrupt flags
#if defined(TIFR4)
FH_DECLARE_REGISTER_ONLY(tifr4, TIFR4)
#else
FH_DECLARE_REGISTER_ONLY(tifr4, TIFR)
#endif

// output compare A
#if defined(OCR4A)
#define FH_HAS_TIMER4_OCA
FH_DECLARE_REGISTER_ONLY(ocr4a, OCR4A)
#elif defined(OCR4)
#define FH_HAS_TIMER4_OCA
FH_DECLARE_REGISTER_ONLY(ocr4a, OCR4)		
#endif

// output compare B
#if defined(OCR4B)
#define FH_HAS_TIMER4_OCB
FH_DECLARE_REGISTER_ONLY(ocr4b, OCR4B)
#endif

// output compare C
#if defined(OCR4C)
#define FH_HAS_TIMER4_OCC
FH_DECLARE_REGISTER_ONLY(ocr4c, OCR4C)
#endif

// input capture
#if defined(ICR4)
#define FH_HAS_TIMER4_IC
FH_DECLARE_REGISTER_ONLY(icr4, ICR4)
#endif

// ********************* bits
// input capture noise canceling
#if defined(ICNC4)
static constexpr auto icnc4 = fieldBit<ICNC4>(tccr4b);
#endif 
// input capture edge select
#if defined(ICES4)
static constexpr auto ices4 = fieldBit<ICES4>(tccr4b);
#endif


// ********************* Interrupts

// -------------interrupts - enable
// overflow
static constexpr auto toie4 = fieldBit<TOIE4>(timsk4);

// output compare A
#if defined(OCIE4A)
static constexpr auto ocie4a = fieldBit<OCIE4A>(timsk4);
#elif defined(OCIE4)
static constexpr auto ocie4a = fieldBit<OCIE4>(timsk4);
#endif

// output compare B
#if defined(OCIE4B)
static constexpr auto ocie4b = fieldBit<OCIE4B>(timsk4);
#endif

// output compare C
#if defined(OCIE4C)
static constexpr auto ocie4c = fieldBit<OCIE4C>(timsk4);
#endif

// input cature
#if defined(ICIE4)
static constexpr auto icie4 = fieldBit<ICIE4>(timsk4);
#endif

// ----------interrupts - flags
// overflow
static constexpr auto tov4 = fieldBit<TOV4>(tifr4);

// output compare A
#if defined(OCF4A)
static constexpr auto ocf4a = fieldBit<OCF4A>(tifr4);
#elif defined(OCF4)
static constexpr auto ocf4a = fieldBit<OCF4>(tifr4);
#endif

// output compare B
#if defined(OCF4B)
static constexpr auto ocf4b = fieldBit<OCF4B>(tifr4);
#endif

// output compare C
#if defined(OCF4C)
static constexpr auto ocf4c = fieldBit<OCF4C>(tifr4);
#endif

// input cature
#if defined(ICF4)
static constexpr auto icf4 = fieldBit<ICF4>(tifr4);
#endif

// -------------interrupts - numbers
// overflow
#ifdef TIMER4_OVF_vect_num
#define FH_IRQ_TOV4 TIMER4_OVF_vect_num
static constexpr auto irq_tov4 = interrupt<FH_IRQ_TOV4>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_tov4)>>: 
        func_fieldbit_enable<decltype(toie4)>,
        func_fieldbit_ready_reset<decltype(tov4)> {};
}

#endif

// output compare A
#ifdef TIMER4_COMPA_vect_num
#define FH_IRQ_TOC4A TIMER4_COMPA_vect_num
#elif defined(TIMER4_COMP_vect_num)
#define FH_IRQ_TOC4A TIMER4_COMP_vect_num
#endif

#ifdef FH_IRQ_TOC4A
static constexpr auto irq_toc4a = interrupt<FH_IRQ_TOC4A>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_toc4a)>>: 
        func_fieldbit_enable<decltype(ocie4a)>,
        func_fieldbit_ready_reset<decltype(ocf4a)> {};
}
#endif


// output compare B
#ifdef TIMER4_COMPB_vect_num
#define FH_IRQ_TOC4B TIMER4_COMPB_vect_num
static constexpr auto irq_toc4b = interrupt<FH_IRQ_TOC4B>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_toc4b)>>: 
        func_fieldbit_enable<decltype(ocie4b)>,
        func_fieldbit_ready_reset<decltype(ocf4b)> {};
}
#endif

// output compare B
#ifdef TIMER4_COMPC_vect_num
#define FH_IRQ_TOC4C TIMER4_COMPC_vect_num
static constexpr auto irq_toc4c = interrupt<FH_IRQ_TOC4C>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_toc4c)>>: 
        func_fieldbit_enable<decltype(ocie4c)>,
        func_fieldbit_ready_reset<decltype(ocf4c)> {};
}
#endif

// input capture
#ifdef TIMER4_CAPT_vect_num
#define FH_IRQ_TIC4 TIMER4_CAPT_vect_num
static constexpr auto irq_tic4 = interrupt<FH_IRQ_TIC4>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_tic4)>>: 
        func_fieldbit_enable<decltype(icie4)>,
        func_fieldbit_ready_reset<decltype(icf4)> {};
}
#endif


// ******************************** ENUMS
// Clock Source
#if (4 == 0 && !defined(__AVR_ATmega128__)) || (4 == 1) || (4 == 2 && defined(__AVR_ATmega128__))
using timer4_cs = details::timer_csext;
#else
using timer4_cs = details::timer_cs;
#endif

template<timer4_cs V>
static constexpr auto timer4_cs_v = integral_constant<timer4_cs, V>{};

// Waveform generation mode
#if defined(WGM40) && defined(WGM41)
#define FH_HAS_TIMER4_WGM
#if defined(WGM43)
using timer4_wgm = details::timer_wgm3;
#elif defined(WGM42)
using timer4_wgm = details::timer_wgm2;
#else
using timer4_wgm = details::timer_wgm1;
#endif

template<timer4_wgm V>
static constexpr auto timer4_wgm_v = integral_constant<timer4_wgm, V>{};

#endif



// ******************************* Sepcialization
namespace details{
	namespace priv {	
		// COM regs
		#if defined(TCCR4A)
		static constexpr auto tccr4_oc = tccr4a;
		#elif defined(TCCR4)
		static constexpr auto tccr4_oc = tccr4;
		#endif		

		// FOC regs
		#if defined(TCCR4C)
		static constexpr auto tccr4_foc = tccr4c;
		#elif (4 == 0) || (4 == 2)
		// TCCRB or TCCR
		#if defined(TCCR4B)
		static constexpr auto tccr4_foc = tccr4b;
		#elif defined(TCCR4)
		static constexpr auto tccr4_foc = tccr4;
		#endif	
		#elif (4 == 1)	
		// Mega8/32 have it TCCR1A
		static constexpr auto tccr4_foc = tccr4a;
		#endif
	}

	#ifdef FH_HAS_TIMER4_OCA
	template<>
	struct timer_oc_impl<4, 0>{
		static constexpr auto available = true;	

		static constexpr auto comr = vField<timer_oc>(
		#if defined(COM40) && defined(COM41)
			fieldBit<COM40>(priv::tccr4_oc),
			fieldBit<COM41>(priv::tccr4_oc)
		#else
			fieldBit<COM4A0>(priv::tccr4_oc),
			fieldBit<COM4A1>(priv::tccr4_oc)
		#endif
		);

		static constexpr auto ocr = ocr4a;
		static constexpr auto irq = irq_toc4a;

		#if defined(FOC4A)
		static constexpr auto foc = fieldBit<FOC4A>(priv::tccr4_foc);
		#elif defined(FOC4)
		static constexpr auto foc = fieldBit<FOC4>(priv::tccr4_foc);
		#endif
	};

	#endif

	#ifdef FH_HAS_TIMER4_OCB
	template<>
	struct timer_oc_impl<4, 1>{
		static constexpr auto available = true;	

		static constexpr auto comr = vField<timer_oc>(
			fieldBit<COM4B0>(priv::tccr4_oc),
			fieldBit<COM4B1>(priv::tccr4_oc)
		);

		static constexpr auto ocr = ocr4b;
		static constexpr auto irq = irq_toc4b;

		#if defined(FOC4B)
		static constexpr auto foc = fieldBit<FOC4B>(priv::tccr4_foc);
		#endif
	};
	#endif

	#ifdef FH_HAS_TIMER4_OCC
	template<>
	struct timer_oc_impl<4, 2>{
		static constexpr auto available = true;	

		static constexpr auto comr = vField<timer_oc>(
			fieldBit<COM4C0>(priv::tccr4_oc),
			fieldBit<COM4C1>(priv::tccr4_oc)
		);

		static constexpr auto ocr = ocr4c;
		static constexpr auto irq = irq_toc4c;

		#if defined(FOC4C)
		static constexpr auto foc = fieldBit<FOC4C>(priv::tccr4_foc);
		#endif
	};
	#endif

	template<>
	struct timer_impl<4>{
		static constexpr auto available = true;
		
		static constexpr auto tcnt = tcnt4;
		static constexpr auto irq_tov = irq_tov4;

		// clock source
		using cs_t = timer4_cs;

		// clock source register
		static constexpr auto cs = mField<(1 << CS42) | (1 << CS41) | (1 << CS40) ,cs_t>(
		#if defined(TCCR4B)
		tccr4b
		#else
		tccr4
		#endif
		);

		// waveform generation
		#ifdef FH_HAS_TIMER4_WGM
		using wgm_t = timer4_wgm;
		
		static constexpr auto wgm = vField<wgm_t>(
		#if defined(WGM42)
			fieldBit<WGM40>(tccr4a)
			, fieldBit<WGM41>(tccr4a)
			, fieldBit<WGM42>(tccr4b)
			#if defined(WGM43)
			, fieldBit<WGM43>(tccr4b)
			#endif
		#else
			fieldBit<WGM40>(tccr4)
			, fieldBit<WGM41>(tccr4)
		#endif
		);

		#endif

		// input capture
		#ifdef FH_HAS_TIMER4_IC
		static constexpr auto icr = icr4;
		static constexpr auto icnc = icnc4;
		static constexpr auto ices = ices4;
		
		static constexpr auto irq_tic = irq_tic4;		
		#endif

		// output capture
		template<unsigned VSub>
		static constexpr auto oc = timer_oc_impl<4, VSub>{};

		#ifdef FH_HAS_TIMER4_OCA
		static constexpr auto ocA = oc<0>;
		#endif
		#ifdef FH_HAS_TIMER4_OCB
		static constexpr auto ocB = oc<1>;
		#endif
		#ifdef FH_HAS_TIMER4_OCC
		static constexpr auto ocC = oc<2>;
		#endif
	};
}
// static constexpr auto timer4 = details::timer_impl<4>{};


#endif // defined TCCR

#endif /* TIMER0_H_ */
