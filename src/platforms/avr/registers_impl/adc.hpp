// Registers 7..0
// ATTINY24/44/84: ADMUX: REFS1 REFS0 MUX5  MUX4  MUX3 MUX2 MUX1 MUX0.   ADCSRA: ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0.   ADCSRB: BIN ACME X   ADLAR X    ADTS2 ADTS1 ADTS0
// ATTINY25/45/85: ADMUX: REFS1 REFS0 ADLAR REFS2 MUX3 MUX2 MUX1 MUX0.   ADCSRA: ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0.   ADCSRB: BIN ACME IPR X     X    ADTS2 ADTS1 ADTS0 
// Mega:           ADMUX: REFS1 REFS0 ADLAR MUX4  MUX3 MUX2 MUX1 MUX0.   ADCSRA: ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0    ADCSRB: X   ACME X   X     MUX5 ADTS2 ADTS1 ADTS0
// Others:         ADMUX: REFS1 REFS0 ADLAR X     MUX3 MUX2 MUX1 MUX0.   ADCSRA: ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0.   ADCSRB: X   ACME X   X     X    ADTS2 ADTS1 ADTS0


#ifdef ADMUX
FASTHAL_DECLAREREGISTER_ONLY(admux, ADMUX)
#endif
#ifdef ADCSRA
FASTHAL_DECLAREREGISTER_ONLY(adcsra, ADCSRA)
#endif
#ifdef ADCSRB
FASTHAL_DECLAREREGISTER_ONLY(adcsrb, ADCSRB)
#endif
#ifdef ADC
FASTHAL_DECLAREREGISTER_ONLY(adc, ADC)
#endif
#ifdef ADCH
FASTHAL_DECLAREREGISTER_ONLY(adch, ADCH)
#endif
#ifdef ADCL
FASTHAL_DECLAREREGISTER_ONLY(adcl, ADCL)
#endif

// select channel - MUX different on ATTINY*4 and others
#ifdef MUX0
using MUX = std::uint8_t;

template<MUX V>
static constexpr auto mux_v = integral_constant<MUX, V>{};

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
, MUX>(admux);
#else
// TODO: typed vfield
constexpr auto mux = vField<MUX>(
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
enum class REFS: std::uint8_t{
    _ = 0
    , _0 = 1 << REFS0
    #ifdef REFS1
    , _1 = 1 << REFS1
    #endif
};

FH_BITENUM_OPS(REFS, std::uint8_t);

template<REFS V>
static constexpr auto refs_v = integral_constant<REFS, V>{};

constexpr auto refs = mField<
        (1 << REFS0)
        #ifdef REFS1
        | (1 << REFS1)
        #endif
, REFS>(admux);
#endif

// Prescaler (ADC speed) - constant on all supported platforms
#ifdef ADPS0

enum class ADPS: std::uint8_t{
    _0 = 1 << ADPS0
    #ifdef ADPS1
    , _1 = 1 << ADPS1
    #endif
    #ifdef ADPS2
    , _2 = 1 << ADPS2
    #endif    
};

FH_BITENUM_OPS(ADPS, std::uint8_t);

template<ADPS V>
static constexpr auto adps_v = integral_constant<ADPS, V>{};

constexpr auto adps = mField<
        (1 << ADPS0)
        #ifdef ADPS1
        | (1 << ADPS1)
        #endif
        #ifdef ADPS2
        | (1 << ADPS2)
        #endif
, ADPS>(adcsra);
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