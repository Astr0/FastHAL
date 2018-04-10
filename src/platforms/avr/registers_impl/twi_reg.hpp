// clock speed
#ifdef TWBR
FH_DECLARE_REGISTER_ONLY(twbr, TWBR);
#endif

// data register
#ifdef TWDR
FH_DECLARE_REGISTER_ONLY(twdr, TWDR);
#endif

// address register
#ifdef TWAR
FH_DECLARE_REGISTER_ONLY(twar, TWAR);
#endif

// status register
#ifdef TWSR
FH_DECLARE_REGISTER_ONLY(twsr, TWSR);
// TODO: status and divider
#endif

// control register
#ifdef TWCR
FH_DECLARE_REGISTER_ONLY(twcr, TWCR);

// interrupt flag
#ifdef TWINT
constexpr auto twint = fieldBit<TWINT>(twcr);
#endif

// Enable Acknowledge
#ifdef TWEA
constexpr auto twea = fieldBit<TWEA>(twcr);
#endif

// Start condition
#ifdef TWSTA
constexpr auto twsta = fieldBit<TWSTA>(twcr);
#endif

// Stop condition
#ifdef TWSTO
constexpr auto twsto = fieldBit<TWSTO>(twcr);
#endif

// Write collision flag
#ifdef TWWC
constexpr auto twwc = fieldBit<TWWC>(twcr);
#endif

// enable bit
#ifdef TWEN
constexpr auto twen = fieldBit<TWEN>(twcr);
#endif

// interrupt enable
#ifdef TWIE
constexpr auto twie = fieldBit<TWIE>(twcr);
#endif

#endif

#if defined(TWBR) && defined(TWDR) && defined(TWCR)
#define FH_HAS_I2C
#endif