namespace avr{
    #include "uart_reg.hpp"
}
#include "uart_irq.hpp"

#define FH_DECLARE_AVR_UART(NUM)\
namespace details{\
    template<>\
    struct uart_impl<NUM>{\
        static constexpr auto available = true;\
        static constexpr auto number = NUM;\
        static constexpr auto u2x = avr::u2x ## NUM;\
        static constexpr auto txen = avr::txen ## NUM;\
        static constexpr auto rxen = avr::rxen ## NUM;\
        static constexpr auto rxc = avr::rxc ## NUM;\
        static constexpr auto txc = avr::txc ## NUM;\
        static constexpr auto udre = avr::udre ## NUM;\
        static constexpr auto upe = avr::upe ## NUM;\
        static constexpr auto udr = avr::udr ## NUM;\
        static constexpr auto ubrr = avr::ubrr ## NUM;\
        static constexpr auto ucsra = avr::ucsr ## NUM ## a;\
        static constexpr auto ucsrb = avr::ucsr ## NUM ## b;\
        static constexpr auto ucsrc = avr::ucsr ## NUM ## c;\
        static constexpr auto irq_txr = irq_txr ## NUM;\
        static constexpr auto irq_txc = irq_txc ## NUM;\
        static constexpr auto irq_rxc = irq_rxc ## NUM;\
    };\
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(avr::u2x ## NUM)>>: func_fieldbit_enable<decltype(avr::u2x ## NUM)>{};\
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(avr::txen ## NUM)>>: func_fieldbit_enable<decltype(avr::txen ## NUM)>{};\
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(avr::rxen ## NUM)>>: func_fieldbit_enable<decltype(avr::rxen ## NUM)>{};\
}
//static constexpr auto uart##NUM = uart<NUM>{};


#ifdef FH_HAS_UART0
FH_DECLARE_AVR_UART(0);
#endif

#ifdef FH_HAS_UART1
FH_DECLARE_AVR_UART(1);
#endif

#ifdef FH_HAS_UART2
FH_DECLARE_AVR_UART(2);
#endif

#ifdef FH_HAS_UART3
FH_DECLARE_AVR_UART(3);
#endif

#undef FH_DECLARE_AVR_UART