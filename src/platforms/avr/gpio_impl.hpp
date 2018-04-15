#define FH_DECLARE_AVR_GPIO_PIN(CODE, PORTN, PINN) \
static constexpr auto pin ## CODE ## PINN = details::avr_pin<PORTN, PINN>{};

#define FH_DECLARE_AVR_GPIO_PORT(CODE, PORTN)\
namespace avr{\
    FH_DECLARE_REGISTER(port ## CODE, PORT ## CODE)\
    FH_DECLARE_REGISTER(pin ## CODE, PIN ## CODE)\
    FH_DECLARE_REGISTER(ddr ## CODE, DDR ## CODE)\
    template<>\
    struct gpio_registers<PORTN>{\
        static constexpr auto port = port ## CODE;\
        static constexpr auto pin = pin ## CODE;\
        static constexpr auto ddr = ddr ## CODE;\
    };\
}\
FH_DECLARE_AVR_GPIO_PIN(CODE, PORTN, 0);\
FH_DECLARE_AVR_GPIO_PIN(CODE, PORTN, 1);\
FH_DECLARE_AVR_GPIO_PIN(CODE, PORTN, 2);\
FH_DECLARE_AVR_GPIO_PIN(CODE, PORTN, 3);\
FH_DECLARE_AVR_GPIO_PIN(CODE, PORTN, 4);\
FH_DECLARE_AVR_GPIO_PIN(CODE, PORTN, 5);\
FH_DECLARE_AVR_GPIO_PIN(CODE, PORTN, 6);\
FH_DECLARE_AVR_GPIO_PIN(CODE, PORTN, 7);\


#ifdef PORTA
FH_DECLARE_AVR_GPIO_PORT(A, 1)
#endif
#ifdef PORTB
FH_DECLARE_AVR_GPIO_PORT(B, 2)
#endif
#ifdef PORTC
FH_DECLARE_AVR_GPIO_PORT(C, 3)
#endif
#ifdef PORTD
FH_DECLARE_AVR_GPIO_PORT(D, 4)
#endif
#ifdef PORTE
FH_DECLARE_AVR_GPIO_PORT(E, 5)
#endif
#ifdef PORTF
FH_DECLARE_AVR_GPIO_PORT(F, 6)
#endif
#ifdef PORTG
FH_DECLARE_AVR_GPIO_PORT(G, 7)
#endif
#ifdef PORTH
FH_DECLARE_AVR_GPIO_PORT(H, 8)
#endif
#ifdef PORTJ
FH_DECLARE_AVR_GPIO_PORT(J, 9)
#endif
#ifdef PORTK
FH_DECLARE_AVR_GPIO_PORT(K, 10)
#endif
#ifdef PORTL
FH_DECLARE_AVR_GPIO_PORT(L, 11)
#endif

#undef FH_DECLARE_AVR_GPIO_PORT
