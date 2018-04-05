#ifdef SREG
// Interrupts constrol register
FASTHAL_DECLAREREGISTER_ONLY(sreg, SREG)

// interrupts enabled flag
#ifdef SREG_I
constexpr auto sreg_i = fieldBit<SREG_I>(sreg);
#endif

#endif

#if defined(SREG) && defined(SREG_I)
#define FH_HAS_IRQ
#endif
