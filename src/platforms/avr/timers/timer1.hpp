// Timer template
// Replace 1 to timer num

#ifndef AVR_TIMER1_H_
#define AVR_TIMER1_H_

#if defined(TCNT1) && (1 != 1 || (defined(TCCR1A) && defined(TCCR1B)))

#define FH_HAS_TIMER1

// *************** Registers
// counter
FH_DECLARE_REGISTER_ONLY(tcnt1, TCNT1)

// control - general
#if defined(TCCR1)
FH_DECLARE_REGISTER_ONLY(tccr1, TCCR1)		
#endif

// control A
#if defined(TCCR1A)
FH_DECLARE_REGISTER_ONLY(tccr1a, TCCR1A)
#endif

// control B
#if defined(TCCR1B)
FH_DECLARE_REGISTER_ONLY(tccr1b, TCCR1B)
#endif

// control C
#if defined(TCCR1C)
FH_DECLARE_REGISTER_ONLY(tccr1c, TCCR1C)
#endif

// interrupt mask
#if defined(TIMSK1)
FH_DECLARE_REGISTER_ONLY(timsk1, TIMSK1)
#else
FH_DECLARE_REGISTER_ONLY(timsk1, TIMSK)
#endif

// interrupt flags
#if defined(TIFR1)
FH_DECLARE_REGISTER_ONLY(tifr1, TIFR1)
#else
FH_DECLARE_REGISTER_ONLY(tifr1, TIFR)
#endif

// output compare A
#if defined(OCR1A)
#define FH_HAS_TIMER1_OCA
FH_DECLARE_REGISTER_ONLY(ocr1a, OCR1A)
#elif defined(OCR1)
#define FH_HAS_TIMER1_OCA
FH_DECLARE_REGISTER_ONLY(ocr1a, OCR1)		
#endif

// output compare B
#if defined(OCR1B)
#define FH_HAS_TIMER1_OCB
FH_DECLARE_REGISTER_ONLY(ocr1b, OCR1B)
#endif

// output compare C
#if defined(OCR1C)
#define FH_HAS_TIMER1_OCC
FH_DECLARE_REGISTER_ONLY(ocr1c, OCR1C)
#endif

// input capture
#if defined(ICR1)
#define FH_HAS_TIMER1_IC
FH_DECLARE_REGISTER_ONLY(icr1, ICR1)
#endif

// ********************* bits
// input capture noise canceling
#if defined(ICNC1)
static constexpr auto icnc1 = fieldBit<ICNC1>(tccr1b);
#endif 
// input capture edge select
#if defined(ICES1)
static constexpr auto ices1 = fieldBit<ICES1>(tccr1b);
#endif


// ********************* Interrupts

// -------------interrupts - enable
// overflow
static constexpr auto toie1 = fieldBit<TOIE1>(timsk1);

// output compare A
#if defined(OCIE1A)
static constexpr auto ocie1a = fieldBit<OCIE1A>(timsk1);
#elif defined(OCIE1)
static constexpr auto ocie1a = fieldBit<OCIE1>(timsk1);
#endif

// output compare B
#if defined(OCIE1B)
static constexpr auto ocie1b = fieldBit<OCIE1B>(timsk1);
#endif

// output compare C
#if defined(OCIE1C)
static constexpr auto ocie1c = fieldBit<OCIE1C>(timsk1);
#endif

// input cature
#if defined(ICIE1)
static constexpr auto icie1 = fieldBit<ICIE1>(timsk1);
#endif

// ----------interrupts - flags
// overflow
static constexpr auto tov1 = fieldBit<TOV1>(tifr1);

// output compare A
#if defined(OCF1A)
static constexpr auto ocf1a = fieldBit<OCF1A>(tifr1);
#elif defined(OCF1)
static constexpr auto ocf1a = fieldBit<OCF1>(tifr1);
#endif

// output compare B
#if defined(OCF1B)
static constexpr auto ocf1b = fieldBit<OCF1B>(tifr1);
#endif

// output compare C
#if defined(OCF1C)
static constexpr auto ocf1c = fieldBit<OCF1C>(tifr1);
#endif

// input cature
#if defined(ICF1)
static constexpr auto icf1 = fieldBit<ICF1>(tifr1);
#endif

// -------------interrupts - numbers
// overflow
#ifdef TIMER1_OVF_vect_num
#define FH_IRQ_TOV1 TIMER1_OVF_vect_num
static constexpr auto irq_tov1 = interrupt<FH_IRQ_TOV1>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_tov1)>>: 
        func_fieldbit_enable<decltype(toie1)>,
        func_fieldbit_ready_reset<decltype(tov1)> {};
}

#endif

// output compare A
#ifdef TIMER1_COMPA_vect_num
#define FH_IRQ_TOC1A TIMER1_COMPA_vect_num
#elif defined(TIMER1_COMP_vect_num)
#define FH_IRQ_TOC1A TIMER1_COMP_vect_num
#endif

#ifdef FH_IRQ_TOC1A
static constexpr auto irq_toc1a = interrupt<FH_IRQ_TOC1A>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_toc1a)>>: 
        func_fieldbit_enable<decltype(ocie1a)>,
        func_fieldbit_ready_reset<decltype(ocf1a)> {};
}
#endif


// output compare B
#ifdef TIMER1_COMPB_vect_num
#define FH_IRQ_TOC1B TIMER1_COMPB_vect_num
static constexpr auto irq_toc1b = interrupt<FH_IRQ_TOC1B>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_toc1b)>>: 
        func_fieldbit_enable<decltype(ocie1b)>,
        func_fieldbit_ready_reset<decltype(ocf1b)> {};
}
#endif

// output compare B
#ifdef TIMER1_COMPC_vect_num
#define FH_IRQ_TOC1C TIMER1_COMPC_vect_num
static constexpr auto irq_toc1c = interrupt<FH_IRQ_TOC1C>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_toc1c)>>: 
        func_fieldbit_enable<decltype(ocie1c)>,
        func_fieldbit_ready_reset<decltype(ocf1c)> {};
}
#endif

// input capture
#ifdef TIMER1_CAPT_vect_num
#define FH_IRQ_TIC1 TIMER1_CAPT_vect_num
static constexpr auto irq_tic1 = interrupt<FH_IRQ_TIC1>{};

namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_tic1)>>: 
        func_fieldbit_enable<decltype(icie1)>,
        func_fieldbit_ready_reset<decltype(icf1)> {};
}
#endif


// ******************************** ENUMS
// Clock Source
#if (1 == 0 && !defined(__AVR_ATmega128__)) || (1 == 1) || (1 == 2 && defined(__AVR_ATmega128__))
using timer1_cs = details::timer_csext;
#else
using timer1_cs = details::timer_cs;
#endif

template<timer1_cs V>
static constexpr auto timer1_cs_v = integral_constant<timer1_cs, V>{};

// Waveform generation mode
#if defined(WGM10) && defined(WGM11)
#define FH_HAS_TIMER1_WGM
#if defined(WGM13)
using timer1_wgm = details::timer_wgm3;
#elif defined(WGM12)
using timer1_wgm = details::timer_wgm2;
#else
using timer1_wgm = details::timer_wgm1;
#endif

template<timer1_wgm V>
static constexpr auto timer1_wgm_v = integral_constant<timer1_wgm, V>{};

#endif



// ******************************* Sepcialization
namespace details{
	namespace priv {	
		// COM regs
		#if defined(TCCR1A)
		static constexpr auto tccr1_oc = tccr1a;
		#elif defined(TCCR1)
		static constexpr auto tccr1_oc = tccr1;
		#endif		

		// FOC regs
		#if defined(TCCR1C)
		static constexpr auto tccr1_foc = tccr1c;
		#elif (1 == 0) || (1 == 2)
		// TCCRB or TCCR
		#if defined(TCCR1B)
		static constexpr auto tccr1_foc = tccr1b;
		#elif defined(TCCR1)
		static constexpr auto tccr1_foc = tccr1;
		#endif	
		#elif (1 == 1)	
		// Mega8/32 have it TCCR1A
		static constexpr auto tccr1_foc = tccr1a;
		#endif
	}

	#ifdef FH_HAS_TIMER1_OCA
	template<>
	struct timer_oc_impl<1, 0>{
		static constexpr auto available = true;	

		static constexpr auto comr = vField<timer_oc>(
		#if defined(COM10) && defined(COM11)
			fieldBit<COM10>(priv::tccr1_oc),
			fieldBit<COM11>(priv::tccr1_oc)
		#else
			fieldBit<COM1A0>(priv::tccr1_oc),
			fieldBit<COM1A1>(priv::tccr1_oc)
		#endif
		);

		static constexpr auto ocr = ocr1a;
		static constexpr auto irq = irq_toc1a;

		#if defined(FOC1A)
		static constexpr auto foc = fieldBit<FOC1A>(priv::tccr1_foc);
		#elif defined(FOC1)
		static constexpr auto foc = fieldBit<FOC1>(priv::tccr1_foc);
		#endif
	};

	#endif

	#ifdef FH_HAS_TIMER1_OCB
	template<>
	struct timer_oc_impl<1, 1>{
		static constexpr auto available = true;	

		static constexpr auto comr = vField<timer_oc>(
			fieldBit<COM1B0>(priv::tccr1_oc),
			fieldBit<COM1B1>(priv::tccr1_oc)
		);

		static constexpr auto ocr = ocr1b;
		static constexpr auto irq = irq_toc1b;

		#if defined(FOC1B)
		static constexpr auto foc = fieldBit<FOC1B>(priv::tccr1_foc);
		#endif
	};
	#endif

	#ifdef FH_HAS_TIMER1_OCC
	template<>
	struct timer_oc_impl<1, 2>{
		static constexpr auto available = true;	

		static constexpr auto comr = vField<timer_oc>(
			fieldBit<COM1C0>(priv::tccr1_oc),
			fieldBit<COM1C1>(priv::tccr1_oc)
		);

		static constexpr auto ocr = ocr1c;
		static constexpr auto irq = irq_toc1c;

		#if defined(FOC1C)
		static constexpr auto foc = fieldBit<FOC1C>(priv::tccr1_foc);
		#endif
	};
	#endif

	template<>
	struct timer_impl<1>{
		static constexpr auto available = true;
		
		static constexpr auto tcnt = tcnt1;
		static constexpr auto irq_tov = irq_tov1;

		// clock source
		using cs_t = timer1_cs;

		// clock source register
		static constexpr auto cs = mField<(1 << CS12) | (1 << CS11) | (1 << CS10) ,cs_t>(
		#if defined(TCCR1B)
		tccr1b
		#else
		tccr1
		#endif
		);

		// waveform generation
		#ifdef FH_HAS_TIMER1_WGM
		using wgm_t = timer1_wgm;
		
		static constexpr auto wgm = vField<wgm_t>(
		#if defined(WGM12)
			fieldBit<WGM10>(tccr1a)
			, fieldBit<WGM11>(tccr1a)
			, fieldBit<WGM12>(tccr1b)
			#if defined(WGM13)
			, fieldBit<WGM13>(tccr1b)
			#endif
		#else
			fieldBit<WGM10>(tccr1)
			, fieldBit<WGM11>(tccr1)
		#endif
		);

		#endif

		// input capture
		#ifdef FH_HAS_TIMER1_IC
		static constexpr auto icr = icr1;
		static constexpr auto icnc = icnc1;
		static constexpr auto ices = ices1;
		
		static constexpr auto irq_tic = irq_tic1;		
		#endif

		// output capture
		template<unsigned VSub>
		static constexpr auto oc = timer_oc_impl<1, VSub>{};

		#ifdef FH_HAS_TIMER1_OCA
		static constexpr auto ocA = oc<0>;
		#endif
		#ifdef FH_HAS_TIMER1_OCB
		static constexpr auto ocB = oc<1>;
		#endif
		#ifdef FH_HAS_TIMER1_OCC
		static constexpr auto ocC = oc<2>;
		#endif
	};
}
static constexpr auto timer1 = details::timer_impl<1>{};


#endif // defined TCCR

#endif /* TIMER0_H_ */
