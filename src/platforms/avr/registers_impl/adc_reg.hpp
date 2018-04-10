// Registers 7..0
// ATTINY24/44/84: ADMUX: REFS1 REFS0 MUX5  MUX4  MUX3 MUX2 MUX1 MUX0.   ADCSRA: ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0.   ADCSRB: BIN ACME X   ADLAR X    ADTS2 ADTS1 ADTS0
// ATTINY25/45/85: ADMUX: REFS1 REFS0 ADLAR REFS2 MUX3 MUX2 MUX1 MUX0.   ADCSRA: ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0.   ADCSRB: BIN ACME IPR X     X    ADTS2 ADTS1 ADTS0 
// Mega:           ADMUX: REFS1 REFS0 ADLAR MUX4  MUX3 MUX2 MUX1 MUX0.   ADCSRA: ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0    ADCSRB: X   ACME X   X     MUX5 ADTS2 ADTS1 ADTS0
// Others:         ADMUX: REFS1 REFS0 ADLAR X     MUX3 MUX2 MUX1 MUX0.   ADCSRA: ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0.   ADCSRB: X   ACME X   X     X    ADTS2 ADTS1 ADTS0


#ifdef ADMUX
FH_DECLARE_REGISTER_ONLY(admux, ADMUX)
#endif
#ifdef ADCSRA
FH_DECLARE_REGISTER_ONLY(adcsra, ADCSRA)
#endif
#ifdef ADCSRB
FH_DECLARE_REGISTER_ONLY(adcsrb, ADCSRB)
#endif
#ifdef ADC
FH_DECLARE_REGISTER_ONLY(adcr, ADC)
#endif
#ifdef ADCH
FH_DECLARE_REGISTER_ONLY(adch, ADCH)
#endif
#ifdef ADCL
FH_DECLARE_REGISTER_ONLY(adcl, ADCL)
#endif

// select channel - MUX different on ATTINY*4 and others
#ifdef MUX0
using ad_mux = std::uint8_t;

template<ad_mux V>
static constexpr auto mux_v = integral_constant<ad_mux, V>{};

#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__) || !defined(MUX5)
constexpr auto mux = mField<
        (1 << MUX0)
        #ifdef MUX1
        | (1 << MUX1)
        #endif
        #ifdef MUX2
        | (1 << MUX2)
        #endif
        #ifdef MUX3
        | (1 << MUX3)
        #endif
        #ifdef MUX4
        | (1 << MUX4)
        #endif
        #ifdef MUX5
        | (1 << MUX5)
        #endif
, ad_mux>(admux);
#else
constexpr auto mux = vField<ad_mux>(
    fieldBit<MUX0>(admux),
    fieldBit<MUX1>(admux),
    fieldBit<MUX2>(admux),
    fieldBit<MUX3>(admux),
    fieldBit<MUX4>(admux),
    fieldBit<MUX5>(adcsrb)
);
#endif
#endif

// select reference volatage - constant on all supported platforms
#ifdef REFS0
	enum class ad_ref{
		#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
			def                 = (0 << REFS1) | (0 << REFS0), // VCC
			external            = (0 << REFS1) | (1 << REFS0),
			internal1v1         = (1 << REFS1) | (0 << REFS0)
		#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
			def                 = (0 << REFS2) | (0 << REFS1) | (0 << REFS0), // VCC
			external            = (0 << REFS2) | (1 << REFS1) | (1 << REFS0),
			internal1v1         = (0 << REFS2) | (1 << REFS1) | (0 << REFS0),
			internal2v56        = (1 << REFS2) | (1 << REFS1) | (0 << REFS0),
			internal2v56_ExtCap = (1 << REFS2) | (1 << REFS1) | (1 << REFS0)
		#else  
			external     = (0 << REFS1) | (0 << REFS0),			
			def          = (0 << REFS1) | (1 << REFS0), // AVCC ext cap
		#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
			internal1v1  = (1 << REFS1) | (0 << REFS0),
			internal2v56 = (1 << REFS1) | (1 << REFS0)
		#else			
			internal1v1  = (1 << REFS1) | (1 << REFS0) // was just "internal"
		#endif
		#endif 
	};

template<ad_ref V>
static constexpr auto refs_v = integral_constant<ad_ref, V>{};

constexpr auto refs = mField<
        (1 << REFS0)
        #ifdef REFS1
        | (1 << REFS1)
        #endif
        #ifdef REFS2
        | (1 << REFS2)
        #endif
, ad_ref>(admux);
#endif

// Prescaler (ADC speed) - constant on all supported platforms
#ifdef ADPS0
enum class ad_ps: std::uint8_t{
    _2   = (0 << ADPS2) | (0 << ADPS1) | (1 << ADPS0),
    _4   = (0 << ADPS2) | (1 << ADPS1) | (0 << ADPS0),
    _8   = (0 << ADPS2) | (1 << ADPS1) | (1 << ADPS0),
    _16  = (1 << ADPS2) | (0 << ADPS1) | (0 << ADPS0),
    _32  = (1 << ADPS2) | (0 << ADPS1) | (1 << ADPS0),
    _64  = (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0),
    _128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0),
    // default a2d prescaler should be inside the desired 50-200 KHz range.
    #if F_CPU >= 16000000 // 16 MHz / 128 = 125 KHz
    def = _128
    #elif F_CPU >= 8000000 // 8 MHz / 64 = 125 KHz
    def = _64
    #elif F_CPU >= 4000000 // 4 MHz / 32 = 125 KHz
    def = _32
    #elif F_CPU >= 2000000 // 2 MHz / 16 = 125 KHz
    def = _16
    #elif F_CPU >= 1000000 // 1 MHz / 8 = 125 KHz
    def = _8
    #elif F_CPU >= 500000 // 0.5 MHz / 4 = 125 KHz
    def = _4
    #else // 128 kHz / 2 = 64 KHz 
    def = _2
    #endif			
};

template<ad_ps V>
static constexpr auto adps_v = integral_constant<ad_ps, V>{};

constexpr auto adps = mField<
        (1 << ADPS0)
        #ifdef ADPS1
        | (1 << ADPS1)
        #endif
        #ifdef ADPS2
        | (1 << ADPS2)
        #endif
, ad_ps>(adcsra);
#endif

// ADC enabled flag - constant on all supported platforms
#ifdef ADEN 
constexpr auto aden = fieldBit<ADEN>(adcsra);
#endif

// 8 or 10 bit selector - different on ATTiny*4
#ifdef ADLAR
#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
constexpr auto adlar = fieldBit<ADLAR>(adcsrb);
#else
constexpr auto adlar = fieldBit<ADLAR>(admux);
#endif
#endif

// ADC start conversion, cleared when conversion finishes
#ifdef ADSC
constexpr auto adsc = fieldBit<ADSC>(adcsra);
#endif

#if defined(ADMUX) && defined(ADCSRA) && defined(ADEN) && defined(ADSC)
#define FH_HAS_ADC
#endif