#define FH_DECLARE_PORT(CODE, NUM)\
FH_DECLARE_REGISTER(port ## CODE, PORT ## CODE)\
FH_DECLARE_REGISTER(pin ## CODE, PIN ## CODE)\
FH_DECLARE_REGISTER(ddr ## CODE, DDR ## CODE)\
template<>\
struct gpio_registers<NUM>{\
    static constexpr auto port = port ## CODE;\
    static constexpr auto pin = pin ## CODE;\
    static constexpr auto ddr = ddr ## CODE;\
};\

#ifdef PORTA
FH_DECLARE_PORT(A, 1)
#endif
#ifdef PORTB
FH_DECLARE_PORT(B, 2)
#endif
#ifdef PORTC
FH_DECLARE_PORT(C, 3)
#endif
#ifdef PORTD
FH_DECLARE_PORT(D, 4)
#endif
#ifdef PORTE
FH_DECLARE_PORT(E, 5)
#endif
#ifdef PORTF
FH_DECLARE_PORT(F, 6)
#endif
#ifdef PORTG
FH_DECLARE_PORT(G, 7)
#endif
#ifdef PORTH
FH_DECLARE_PORT(H, 8)
#endif
#ifdef PORTJ
FH_DECLARE_PORT(J, 9)
#endif
#ifdef PORTK
FH_DECLARE_PORT(K, 10)
#endif
#ifdef PORTL
FH_DECLARE_PORT(L, 11)
#endif

#undef FH_DECLARE_PORT