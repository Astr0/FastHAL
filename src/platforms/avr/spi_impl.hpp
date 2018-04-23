//******************************* REGISTERS
namespace avr{

#if defined(SPCR0)
#define FH_HAS_SPI0

// registers
// control
FH_DECLARE_REGISTER_ONLY(spcr0, SPCR0)
// status
FH_DECLARE_REGISTER_ONLY(spsr0, SPSR0)
// data
FH_DECLARE_REGISTER_ONLY(spdr0, SPDR0)

// enable
constexpr auto spe0 = fieldBit<SPE0>(spcr0);
// interrupt enable
constexpr auto spie0 = fieldBit<SPIE0>(spcr0);
// data order (LSB first?)
constexpr auto dord0 = fieldBit<DORD0>(spcr0);
// master select (master?)
constexpr auto mstr0 = fieldBit<MSTR0>(spcr0);

// polarity and phase
constexpr auto spimode0 = vField<spi_mode>(fieldBit<CPHA0>(spcr0), fieldBit<CPOL0>(spcr0));

// prescaler
constexpr auto spips0 = vField<spi_ps>(fieldBit<SPR00>(spcr0), fieldBit<SPR01>(spcr0), fieldBit<SPI2X0>(spsr0));

// interrupt flag
constexpr auto spif0 = fieldBit<SPIF0>(spsr0);

// write collision
constexpr auto wcol0 = fieldBit<WCOL0>(spsr0);

#elif defined (SPCR)
#define FH_HAS_SPI0

// registers
// control
FH_DECLARE_REGISTER_ONLY(spcr0, SPCR)
// status
FH_DECLARE_REGISTER_ONLY(spsr0, SPSR)
// data
FH_DECLARE_REGISTER_ONLY(spdr0, SPDR)

// enable
constexpr auto spe0 = fieldBit<SPE>(spcr0);
// interrupt enable
constexpr auto spie0 = fieldBit<SPIE>(spcr0);
// data order (LSB first?)
constexpr auto dord0 = fieldBit<DORD>(spcr0);
// master select (master?)
constexpr auto mstr0 = fieldBit<MSTR>(spcr0);

// polarity and phase
constexpr auto spimode0 = vField<spi_mode>(fieldBit<CPHA>(spcr0), fieldBit<CPOL>(spcr0));

// prescaler
constexpr auto spips0 = vField<spi_ps>(fieldBit<SPR0>(spcr0), fieldBit<SPR1>(spcr0), fieldBit<SPI2X>(spsr0));

// interrupt flag
constexpr auto spif0 = fieldBit<SPIF>(spsr0);

// write collision
constexpr auto wcol0 = fieldBit<WCOL>(spsr0);

#endif


#ifdef SPCR1
#define FH_HAS_SPI1

// registers
// control
FH_DECLARE_REGISTER_ONLY(spcr1, SPCR1)
// status
FH_DECLARE_REGISTER_ONLY(spsr1, SPSR1)
// data
FH_DECLARE_REGISTER_ONLY(spdr1, SPDR1)

// enable
constexpr auto spe1 = fieldBit<SPE1>(spcr1);
// interrupt enable
constexpr auto spie1 = fieldBit<SPIE1>(spcr1);
// data order (LSB first?)
constexpr auto dord1 = fieldBit<DORD1>(spcr1);
// master select (master?)
constexpr auto mstr1 = fieldBit<MSTR1>(spcr1);

// polarity and phase
constexpr auto spmode1 = vField<spi_mode>(fieldBit<CPHA1>(spcr1), fieldBit<CPOL1>(spcr1));

// prescaler
constexpr auto spps1 = vField<spi_ps>(fieldBit<SPR11>(spcr1), fieldBit<SPR11>(spcr1), fieldBit<SPI2X1>(spsr1));

// interrupt flag
constexpr auto spif1 = fieldBit<SPIF1>(spsr1);

// write collision
constexpr auto wcol1 = fieldBit<WCOL1>(spsr1);
#endif

}

// ***************************** INTERRUPTS

#ifdef FH_HAS_SPI0

#if defined(SPI0_STC_vect_num)

#define FH_IRQ_SPI0 SPI0_STC_vect_num
#else
#define FH_IRQ_SPI0 SPI_STC_vect_num
#endif

constexpr auto irq_spi0 = interrupt<FH_IRQ_SPI0>{};
namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_spi0)>>: 
        func_fieldbit_enable<decltype(avr::spie0)>,
        func_fieldbit_ready_reset<decltype(avr::spif0)>
        {};
}
#endif


#ifdef FH_HAS_SPI1

#define FH_IRQ_SPI1 SPI1_STC_vect_num

constexpr auto irq_spi1 = interrupt<FH_IRQ_SPI1>{};
namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_spi1)>>: 
        func_fieldbit_enable<decltype(avr::spie1)>,
        func_fieldbit_ready_reset<decltype(avr::spif1)>
        {};
}
#endif

// ***************************** IMPL
namespace details{
#ifdef FH_HAS_SPI0
template<>
struct spi_impl<0>{
    static constexpr bool available = true;
    
    static constexpr auto spe = avr::spe0;
    static constexpr auto spie = avr::spie0;
    static constexpr auto dord = avr::dord0;
    static constexpr auto mstr = avr::mstr0;
    static constexpr auto spimode = avr::spimode0;
    static constexpr auto spips = avr::spips0;
    static constexpr auto spif = avr::spif0;
    static constexpr auto wcol = avr::wcol0;

    static constexpr auto spdr = avr::spdr0;

    static constexpr auto irq = irq_spi0;
};
#endif

#ifdef FH_HAS_SPI1
template<>
struct spi_impl<1>{
    static constexpr bool available = true;
    
    static constexpr auto spe = avr::spe1;
    static constexpr auto spie = avr::spie1;
    static constexpr auto dord = avr::dord1;
    static constexpr auto mstr = avr::mstr1;
    static constexpr auto spimode = avr::spimode1;
    static constexpr auto spips = avr::spips1;
    static constexpr auto spif = avr::spif1;
    static constexpr auto wcol = avr::wcol1;

    static constexpr auto spdr = avr::spdr1;

    static constexpr auto irq = irq_spi1;
};
#endif
}




