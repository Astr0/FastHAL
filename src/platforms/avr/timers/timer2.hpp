// Timer template
// Replace 2 to timer num

#ifndef AVR_TIMER2_H_
#define AVR_TIMER2_H_

#if defined(TCNT2) && (2 != 1 || (defined(TCCR1A) && defined(TCCR1B)))

#define FH_HAS_TIMER2

// *************** Registers
// counter
FH_DECLARE_REGISTER_ONLY(tcnt2, TCNT2)

// control - general
#if defined(TCCR2)
FH_DECLARE_REGISTER_ONLY(tccr2, TCCR2)		
#endif

// control A
#if defined(TCCR2A)
FH_DECLARE_REGISTER_ONLY(tccr2a, TCCR2A)
#endif

// control B
#if defined(TCCR2B)
FH_DECLARE_REGISTER_ONLY(tccr2b, TCCR2B)
#endif

// control C
#if defined(TCCR2C)
FH_DECLARE_REGISTER_ONLY(tccr2c, TCCR2C)
#endif

// interrupt mask
#if defined(TIMSK2)
FH_DECLARE_REGISTER_ONLY(timsk2, TIMSK2)
#else
FH_DECLARE_REGISTER_ONLY(timsk2, TIMSK)
#endif

// interrupt flags
#if defined(TIFR2)
FH_DECLARE_REGISTER_ONLY(tifr2, TIFR2)
#else
FH_DECLARE_REGISTER_ONLY(tifr2, TIFR)
#endif

// output compare A
#if defined(OCR2A)
#define FH_HAS_TIMER2_OCA
FH_DECLARE_REGISTER_ONLY(ocr2a, OCR2A)
#elif defined(OCR2)
#define FH_HAS_TIMER2_OCA
FH_DECLARE_REGISTER_ONLY(ocr2a, OCR2)		
#endif

// output compare B
#if defined(OCR2B)
#define FH_HAS_TIMER2_OCB
FH_DECLARE_REGISTER_ONLY(ocr2b, OCR2B)
#endif

// output compare C
#if defined(OCR2C)
#define FH_HAS_TIMER2_OCC
FH_DECLARE_REGISTER_ONLY(ocr2c, OCR2C)
#endif

// input capture
#if defined(ICR2)
#define FH_HAS_TIMER2_IC
FH_DECLARE_REGISTER_ONLY(icr2, ICR2)
#endif

// ********************* bits
// input capture noise canceling
#if defined(ICNC2)
static constexpr auto icnc2 = fieldBit<ICNC2>(tccr2b);
#endif 
// input capture edge select
#if defined(ICES2)
static constexpr auto ices2 = fieldBit<ICES2>(tccr2b);
#endif


// ********************* Interrupts

// -------------interrupts - enable
// overflow
static constexpr auto toie2 = fieldBit<TOIE2>(timsk2);

// output compare A
#if defined(OCIE2A)
static constexpr auto ocie2a = fieldBit<OCIE2A>(timsk2);
#elif defined(OCIE2)
static constexpr auto ocie2a = fieldBit<OCIE2>(timsk2);
#endif

// output compare B
#if defined(OCIE2B)
static constexpr auto ocie2b = fieldBit<OCIE2B>(timsk2);
#endif

// output compare C
#if defined(OCIE2C)
static constexpr auto ocie2c = fieldBit<OCIE2C>(timsk2);
#endif

// input cature
#if defined(ICIE2)
static constexpr auto icie2 = fieldBit<ICIE2>(timsk2);
#endif

// ----------interrupts - flags
// overflow
static constexpr auto tov2 = fieldBit<TOV2>(tifr2);

// output compare A
#if defined(OCF2A)
static constexpr auto ocf2a = fieldBit<OCF2A>(tifr2);
#elif defined(OCF2)
static constexpr auto ocf2a = fieldBit<OCF2>(tifr2);
#endif

// output compare B
#if defined(OCF2B)
static constexpr auto ocf2b = fieldBit<OCF2B>(tifr2);
#endif

// output compare C
#if defined(OCF2C)
static constexpr auto ocf2c = fieldBit<OCF2C>(tifr2);
#endif

// input cature
#if defined(ICF2)
static constexpr auto icf2 = fieldBit<ICF2>(tifr2);
#endif

// -------------interrupts - numbers
// overflow
#ifdef TIMER2_OVF_vect_num
#define FH_IRQ_TOV2 TIMER2_OVF_vect_num
static constexpr auto irq_tov2 = interrupt<FH_IRQ_TOV2>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_tov2)>>: 
        func_fieldbit_enable<decltype(toie2)>,
        func_fieldbit_ready_reset<decltype(tov2)> {};
}

#endif

// output compare A
#ifdef TIMER2_COMPA_vect_num
#define FH_IRQ_TOC2A TIMER2_COMPA_vect_num
#elif defined(TIMER2_COMP_vect_num)
#define FH_IRQ_TOC2A TIMER2_COMP_vect_num
#endif

#ifdef FH_IRQ_TOC2A
static constexpr auto irq_toc2a = interrupt<FH_IRQ_TOC2A>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_toc2a)>>: 
        func_fieldbit_enable<decltype(ocie2a)>,
        func_fieldbit_ready_reset<decltype(ocf2a)> {};
}
#endif


// output compare B
#ifdef TIMER2_COMPB_vect_num
#define FH_IRQ_TOC2B TIMER2_COMPB_vect_num
static constexpr auto irq_toc2b = interrupt<FH_IRQ_TOC2B>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_toc2b)>>: 
        func_fieldbit_enable<decltype(ocie2b)>,
        func_fieldbit_ready_reset<decltype(ocf2b)> {};
}
#endif

// output compare B
#ifdef TIMER2_COMPC_vect_num
#define FH_IRQ_TOC2C TIMER2_COMPC_vect_num
static constexpr auto irq_toc2c = interrupt<FH_IRQ_TOC2C>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_toc2c)>>: 
        func_fieldbit_enable<decltype(ocie2c)>,
        func_fieldbit_ready_reset<decltype(ocf2c)> {};
}
#endif

// input capture
#ifdef TIMER2_CAPT_vect_num
#define FH_IRQ_TIC2 TIMER2_CAPT_vect_num
static constexpr auto irq_tic2 = interrupt<FH_IRQ_TIC2>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_tic2)>>: 
        func_fieldbit_enable<decltype(icie2)>,
        func_fieldbit_ready_reset<decltype(icf2)> {};
}
#endif


// ******************************** ENUMS
// Clock Source
#if (2 == 0 && !defined(__AVR_ATmega128__)) || (2 == 1) || (2 == 2 && defined(__AVR_ATmega128__))
using timer2_cs = details::timer_csext;
#else
using timer2_cs = details::timer_cs;
#endif

template<timer2_cs V>
static constexpr auto timer2_cs_v = integral_constant<timer2_cs, V>{};

// Waveform generation mode
#if defined(WGM20) && defined(WGM21)
#define FH_HAS_TIMER2_WGM
#if defined(WGM23)
using timer2_wgm = details::timer_wgm3;
#elif defined(WGM22)
using timer2_wgm = details::timer_wgm2;
#else
using timer2_wgm = details::timer_wgm1;
#endif

template<timer2_wgm V>
static constexpr auto timer2_wgm_v = integral_constant<timer2_wgm, V>{};

#endif



// ******************************* Sepcialization
namespace details{
	namespace priv {	
		// COM regs
		#if defined(TCCR2A)
		static constexpr auto tccr2_oc = tccr2a;
		#elif defined(TCCR2)
		static constexpr auto tccr2_oc = tccr2;
		#endif		

		// FOC regs
		#if defined(TCCR2C)
		static constexpr auto tccr2_foc = tccr2c;
		#elif (2 == 0) || (2 == 2)
		// TCCRB or TCCR
		#if defined(TCCR2B)
		static constexpr auto tccr2_foc = tccr2b;
		#elif defined(TCCR2)
		static constexpr auto tccr2_foc = tccr2;
		#endif	
		#elif (2 == 1)	
		// Mega8/32 have it TCCR1A
		static constexpr auto tccr2_foc = tccr2a;
		#endif
	}

	#ifdef FH_HAS_TIMER2_OCA
	template<>
	struct timer_oc_impl<2, 0>{
		static constexpr auto available = true;	

		static constexpr auto comr = vField<timer_oc>(
		#if defined(COM20) && defined(COM21)
			fieldBit<COM20>(priv::tccr2_oc),
			fieldBit<COM21>(priv::tccr2_oc)
		#else
			fieldBit<COM2A0>(priv::tccr2_oc),
			fieldBit<COM2A1>(priv::tccr2_oc)
		#endif
		);

		static constexpr auto ocr = ocr2a;
		static constexpr auto irq = irq_toc2a;

		#if defined(FOC2A)
		static constexpr auto foc = fieldBit<FOC2A>(priv::tccr2_foc);
		#elif defined(FOC2)
		static constexpr auto foc = fieldBit<FOC2>(priv::tccr2_foc);
		#endif
	};

	#endif

	#ifdef FH_HAS_TIMER2_OCB
	template<>
	struct timer_oc_impl<2, 1>{
		static constexpr auto available = true;	

		static constexpr auto comr = vField<timer_oc>(
			fieldBit<COM2B0>(priv::tccr2_oc),
			fieldBit<COM2B1>(priv::tccr2_oc)
		);

		static constexpr auto ocr = ocr2b;
		static constexpr auto irq = irq_toc2b;

		#if defined(FOC2B)
		static constexpr auto foc = fieldBit<FOC2B>(priv::tccr2_foc);
		#endif
	};
	#endif

	#ifdef FH_HAS_TIMER2_OCC
	template<>
	struct timer_oc_impl<2, 2>{
		static constexpr auto available = true;	

		static constexpr auto comr = vField<timer_oc>(
			fieldBit<COM2C0>(priv::tccr2_oc),
			fieldBit<COM2C1>(priv::tccr2_oc)
		);

		static constexpr auto ocr = ocr2c;
		static constexpr auto irq = irq_toc2c;

		#if defined(FOC2C)
		static constexpr auto foc = fieldBit<FOC2C>(priv::tccr2_foc);
		#endif
	};
	#endif

	template<>
	struct timer_impl<2>{
		static constexpr auto available = true;
		
		static constexpr auto tcnt = tcnt2;
		static constexpr auto irq_tov = irq_tov2;

		// clock source
		using cs_t = timer2_cs;

		// clock source register
		static constexpr auto cs = mField<(1 << CS22) | (1 << CS21) | (1 << CS20) ,cs_t>(
		#if defined(TCCR2B)
		tccr2b
		#else
		tccr2
		#endif
		);

		// waveform generation
		#ifdef FH_HAS_TIMER2_WGM
		using wgm_t = timer2_wgm;
		
		static constexpr auto wgm = vField<wgm_t>(
		#if defined(WGM22)
			fieldBit<WGM20>(tccr2a)
			, fieldBit<WGM21>(tccr2a)
			, fieldBit<WGM22>(tccr2b)
			#if defined(WGM23)
			, fieldBit<WGM23>(tccr2b)
			#endif
		#else
			fieldBit<WGM20>(tccr2)
			, fieldBit<WGM21>(tccr2)
		#endif
		);

		#endif

		// input capture
		#ifdef FH_HAS_TIMER2_IC
		static constexpr auto icr = icr2;
		static constexpr auto icnc = icnc2;
		static constexpr auto ices = ices2;
		
		static constexpr auto irq_tic = irq_tic2;		
		#endif

		// output capture
		template<unsigned VSub>
		static constexpr auto oc = timer_oc_impl<2, VSub>{};

		#ifdef FH_HAS_TIMER2_OCA
		static constexpr auto ocA = oc<0>;
		#endif
		#ifdef FH_HAS_TIMER2_OCB
		static constexpr auto ocB = oc<1>;
		#endif
		#ifdef FH_HAS_TIMER2_OCC
		static constexpr auto ocC = oc<2>;
		#endif
	};
}
static constexpr auto timer2 = details::timer_impl<2>{};


#endif // defined TCCR

#endif /* TIMER0_H_ */
