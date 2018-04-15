// Timer template
// Replace 3 to timer num

#ifndef AVR_TIMER3_H_
#define AVR_TIMER3_H_

#if defined(TCNT3) && (3 != 1 || (defined(TCCR1A) && defined(TCCR1B)))

#define FH_HAS_TIMER3

// *************** Registers
// counter
FH_DECLARE_REGISTER_ONLY(tcnt3, TCNT3)

// control - general
#if defined(TCCR3)
FH_DECLARE_REGISTER_ONLY(tccr3, TCCR3)		
#endif

// control A
#if defined(TCCR3A)
FH_DECLARE_REGISTER_ONLY(tccr3a, TCCR3A)
#endif

// control B
#if defined(TCCR3B)
FH_DECLARE_REGISTER_ONLY(tccr3b, TCCR3B)
#endif

// control C
#if defined(TCCR3C)
FH_DECLARE_REGISTER_ONLY(tccr3c, TCCR3C)
#endif

// interrupt mask
#if defined(TIMSK3)
FH_DECLARE_REGISTER_ONLY(timsk3, TIMSK3)
#else
FH_DECLARE_REGISTER_ONLY(timsk3, TIMSK)
#endif

// interrupt flags
#if defined(TIFR3)
FH_DECLARE_REGISTER_ONLY(tifr3, TIFR3)
#else
FH_DECLARE_REGISTER_ONLY(tifr3, TIFR)
#endif

// output compare A
#if defined(OCR3A)
#define FH_HAS_TIMER3_OCA
FH_DECLARE_REGISTER_ONLY(ocr3a, OCR3A)
#elif defined(OCR3)
#define FH_HAS_TIMER3_OCA
FH_DECLARE_REGISTER_ONLY(ocr3a, OCR3)		
#endif

// output compare B
#if defined(OCR3B)
#define FH_HAS_TIMER3_OCB
FH_DECLARE_REGISTER_ONLY(ocr3b, OCR3B)
#endif

// output compare C
#if defined(OCR3C)
#define FH_HAS_TIMER3_OCC
FH_DECLARE_REGISTER_ONLY(ocr3c, OCR3C)
#endif

// input capture
#if defined(ICR3)
#define FH_HAS_TIMER3_IC
FH_DECLARE_REGISTER_ONLY(icr3, ICR3)
#endif

// ********************* bits
// input capture noise canceling
#if defined(ICNC3)
static constexpr auto icnc3 = fieldBit<ICNC3>(tccr3b);
#endif 
// input capture edge select
#if defined(ICES3)
static constexpr auto ices3 = fieldBit<ICES3>(tccr3b);
#endif


// ********************* Interrupts

// -------------interrupts - enable
// overflow
static constexpr auto toie3 = fieldBit<TOIE3>(timsk3);

// output compare A
#if defined(OCIE3A)
static constexpr auto ocie3a = fieldBit<OCIE3A>(timsk3);
#elif defined(OCIE3)
static constexpr auto ocie3a = fieldBit<OCIE3>(timsk3);
#endif

// output compare B
#if defined(OCIE3B)
static constexpr auto ocie3b = fieldBit<OCIE3B>(timsk3);
#endif

// output compare C
#if defined(OCIE3C)
static constexpr auto ocie3c = fieldBit<OCIE3C>(timsk3);
#endif

// input cature
#if defined(ICIE3)
static constexpr auto icie3 = fieldBit<ICIE3>(timsk3);
#endif

// ----------interrupts - flags
// overflow
static constexpr auto tov3 = fieldBit<TOV3>(tifr3);

// output compare A
#if defined(OCF3A)
static constexpr auto ocf3a = fieldBit<OCF3A>(tifr3);
#elif defined(OCF3)
static constexpr auto ocf3a = fieldBit<OCF3>(tifr3);
#endif

// output compare B
#if defined(OCF3B)
static constexpr auto ocf3b = fieldBit<OCF3B>(tifr3);
#endif

// output compare C
#if defined(OCF3C)
static constexpr auto ocf3c = fieldBit<OCF3C>(tifr3);
#endif

// input cature
#if defined(ICF3)
static constexpr auto icf3 = fieldBit<ICF3>(tifr3);
#endif

// -------------interrupts - numbers
// overflow
#ifdef TIMER3_OVF_vect_num
#define FH_IRQ_TOV3 TIMER3_OVF_vect_num
static constexpr auto irq_tov3 = interrupt<FH_IRQ_TOV3>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_tov3)>>: 
        func_fieldbit_enable<decltype(toie3)>,
        func_fieldbit_ready_reset<decltype(tov3)> {};
}

#endif

// output compare A
#ifdef TIMER3_COMPA_vect_num
#define FH_IRQ_TOC3A TIMER3_COMPA_vect_num
#elif defined(TIMER3_COMP_vect_num)
#define FH_IRQ_TOC3A TIMER3_COMP_vect_num
#endif

#ifdef FH_IRQ_TOC3A
static constexpr auto irq_toc3a = interrupt<FH_IRQ_TOC3A>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_toc3a)>>: 
        func_fieldbit_enable<decltype(ocie3a)>,
        func_fieldbit_ready_reset<decltype(ocf3a)> {};
}
#endif


// output compare B
#ifdef TIMER3_COMPB_vect_num
#define FH_IRQ_TOC3B TIMER3_COMPB_vect_num
static constexpr auto irq_toc3b = interrupt<FH_IRQ_TOC3B>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_toc3b)>>: 
        func_fieldbit_enable<decltype(ocie3b)>,
        func_fieldbit_ready_reset<decltype(ocf3b)> {};
}
#endif

// output compare B
#ifdef TIMER3_COMPC_vect_num
#define FH_IRQ_TOC3C TIMER3_COMPC_vect_num
static constexpr auto irq_toc3c = interrupt<FH_IRQ_TOC3C>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_toc3c)>>: 
        func_fieldbit_enable<decltype(ocie3c)>,
        func_fieldbit_ready_reset<decltype(ocf3c)> {};
}
#endif

// input capture
#ifdef TIMER3_CAPT_vect_num
#define FH_IRQ_TIC3 TIMER3_CAPT_vect_num
static constexpr auto irq_tic3 = interrupt<FH_IRQ_TIC3>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_tic3)>>: 
        func_fieldbit_enable<decltype(icie3)>,
        func_fieldbit_ready_reset<decltype(icf3)> {};
}
#endif


// ******************************** ENUMS
// Clock Source
#if (3 == 0 && !defined(__AVR_ATmega128__)) || (3 == 1) || (3 == 2 && defined(__AVR_ATmega128__))
using timer3_cs = details::timer_csext;
#else
using timer3_cs = details::timer_cs;
#endif

template<timer3_cs V>
static constexpr auto timer3_cs_v = integral_constant<timer3_cs, V>{};

// Waveform generation mode
#if defined(WGM30) && defined(WGM31)
#define FH_HAS_TIMER3_WGM
#if defined(WGM33)
using timer3_wgm = details::timer_wgm3;
#elif defined(WGM32)
using timer3_wgm = details::timer_wgm2;
#else
using timer3_wgm = details::timer_wgm1;
#endif

template<timer3_wgm V>
static constexpr auto timer3_wgm_v = integral_constant<timer3_wgm, V>{};

#endif



// ******************************* Sepcialization
namespace details{
	namespace priv {	
		// COM regs
		#if defined(TCCR3A)
		static constexpr auto tccr3_oc = tccr3a;
		#elif defined(TCCR3)
		static constexpr auto tccr3_oc = tccr3;
		#endif		

		// FOC regs
		#if defined(TCCR3C)
		static constexpr auto tccr3_foc = tccr3c;
		#elif (3 == 0) || (3 == 2)
		// TCCRB or TCCR
		#if defined(TCCR3B)
		static constexpr auto tccr3_foc = tccr3b;
		#elif defined(TCCR3)
		static constexpr auto tccr3_foc = tccr3;
		#endif	
		#elif (3 == 1)	
		// Mega8/32 have it TCCR1A
		static constexpr auto tccr3_foc = tccr3a;
		#endif
	}

	#ifdef FH_HAS_TIMER3_OCA
	template<>
	struct timer_oc_impl<3, 0>{
		static constexpr auto available = true;	

		static constexpr auto comr = vField<timer_oc>(
		#if defined(COM30) && defined(COM31)
			fieldBit<COM30>(priv::tccr3_oc),
			fieldBit<COM31>(priv::tccr3_oc)
		#else
			fieldBit<COM3A0>(priv::tccr3_oc),
			fieldBit<COM3A1>(priv::tccr3_oc)
		#endif
		);

		static constexpr auto ocr = ocr3a;
		static constexpr auto irq = irq_toc3a;

		#if defined(FOC3A)
		static constexpr auto foc = fieldBit<FOC3A>(priv::tccr3_foc);
		#elif defined(FOC3)
		static constexpr auto foc = fieldBit<FOC3>(priv::tccr3_foc);
		#endif
	};

	#endif

	#ifdef FH_HAS_TIMER3_OCB
	template<>
	struct timer_oc_impl<3, 1>{
		static constexpr auto available = true;	

		static constexpr auto comr = vField<timer_oc>(
			fieldBit<COM3B0>(priv::tccr3_oc),
			fieldBit<COM3B1>(priv::tccr3_oc)
		);

		static constexpr auto ocr = ocr3b;
		static constexpr auto irq = irq_toc3b;

		#if defined(FOC3B)
		static constexpr auto foc = fieldBit<FOC3B>(priv::tccr3_foc);
		#endif
	};
	#endif

	#ifdef FH_HAS_TIMER3_OCC
	template<>
	struct timer_oc_impl<3, 2>{
		static constexpr auto available = true;	

		static constexpr auto comr = vField<timer_oc>(
			fieldBit<COM3C0>(priv::tccr3_oc),
			fieldBit<COM3C1>(priv::tccr3_oc)
		);

		static constexpr auto ocr = ocr3c;
		static constexpr auto irq = irq_toc3c;

		#if defined(FOC3C)
		static constexpr auto foc = fieldBit<FOC3C>(priv::tccr3_foc);
		#endif
	};
	#endif

	template<>
	struct timer_impl<3>{
		static constexpr auto available = true;
		
		static constexpr auto tcnt = tcnt3;
		static constexpr auto irq_tov = irq_tov3;

		// clock source
		using cs_t = timer3_cs;

		// clock source register
		static constexpr auto cs = mField<(1 << CS32) | (1 << CS31) | (1 << CS30) ,cs_t>(
		#if defined(TCCR3B)
		tccr3b
		#else
		tccr3
		#endif
		);

		// waveform generation
		#ifdef FH_HAS_TIMER3_WGM
		using wgm_t = timer3_wgm;
		
		static constexpr auto wgm = vField<wgm_t>(
		#if defined(WGM32)
			fieldBit<WGM30>(tccr3a)
			, fieldBit<WGM31>(tccr3a)
			, fieldBit<WGM32>(tccr3b)
			#if defined(WGM33)
			, fieldBit<WGM33>(tccr3b)
			#endif
		#else
			fieldBit<WGM30>(tccr3)
			, fieldBit<WGM31>(tccr3)
		#endif
		);

		#endif

		// input capture
		#ifdef FH_HAS_TIMER3_IC
		static constexpr auto icr = icr3;
		static constexpr auto icnc = icnc3;
		static constexpr auto ices = ices3;
		
		static constexpr auto irq_tic = irq_tic3;		
		#endif

		// output capture
		template<unsigned VSub>
		static constexpr auto oc = timer_oc_impl<3, VSub>{};

		#ifdef FH_HAS_TIMER3_OCA
		static constexpr auto ocA = oc<0>;
		#endif
		#ifdef FH_HAS_TIMER3_OCB
		static constexpr auto ocB = oc<1>;
		#endif
		#ifdef FH_HAS_TIMER3_OCC
		static constexpr auto ocC = oc<2>;
		#endif
	};
}
// static constexpr auto timer3 = details::timer_impl<3>{};


#endif // defined TCCR

#endif /* TIMER0_H_ */
