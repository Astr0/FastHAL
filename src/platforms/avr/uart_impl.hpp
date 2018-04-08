#define FH_DECLARE_AVR_UART(NUM)\
namespace details{\
    template<>\
    struct uart<NUM>{\
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
        static constexpr auto ucsrc = avr::ucsr ## NUM ## c;\
        static constexpr auto irq_txr = irq_txr ## NUM;\
        static constexpr auto irq_txc = irq_txc ## NUM;\
        static constexpr auto irq_rxc = irq_rxc ## NUM;\
    };\
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(avr::u2x ## NUM)>>: func_fieldbit_enable<decltype(avr::u2x ## NUM)>{};\
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(avr::txen ## NUM)>>: func_fieldbit_enable<decltype(avr::txen ## NUM)>{};\
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(avr::rxen ## NUM)>>: func_fieldbit_enable<decltype(avr::rxen ## NUM)>{};\
}\
static constexpr auto uart##NUM = details::uart<NUM>{};\

#define FH_DECLARE_AVR_UART_BUF(NUM, TYPE, SIZE)\
namespace details{\
    template<>\
    struct uart_buf<uart<NUM>, TYPE>{ static ring_buffer<SIZE> buffer; };\
}\
ring_buffer<SIZE> details::uart_buf<details::uart<NUM>, TYPE>::buffer = {};\

#define FH_DECLARE_AVR_UART_TXR(NUM)\
namespace details{\
    template<>\
    struct default_isr<uart<NUM>::irq_txr.number>{ static inline void handle() { uart_txr_irq<uart<NUM>>(); } };\
}\
FH_ISR(FH_IRQ_TXR ## NUM);\



#ifdef FH_HAS_UART0
FH_DECLARE_AVR_UART(0);

#if defined(FH_UART0_TX) && (FH_UART0_TX) > 0
FH_DECLARE_AVR_UART_BUF(0, true, FH_UART0_TX);
FH_DECLARE_AVR_UART_TXR(0);
#endif

#if defined(FH_UART0_RX) && (FH_UART0_RX) > 0
FH_DECLARE_AVR_UART_BUF(0, false, FH_UART0_RX);
#endif
#endif

#ifdef FH_HAS_UART1
FH_DECLARE_AVR_UART(1);

#if defined(FH_UART1_TX) && (FH_UART1_TX) > 0
FH_DECLARE_AVR_UART_BUF(1, true, FH_UART1_TX);
FH_DECLARE_AVR_UART_TXR(1);
#endif

#if defined(FH_UART1_RX) && (FH_UART1_RX) > 0
FH_DECLARE_AVR_UART_BUF(1, false, FH_UART1_RX);
#endif
#endif

#ifdef FH_HAS_UART2
FH_DECLARE_AVR_UART(2);

#if defined(FH_UART2_TX) && (FH_UART2_TX) > 0
FH_DECLARE_AVR_UART_BUF(2, true, FH_UART2_TX);
FH_DECLARE_AVR_UART_TXR(2);
#endif

#if defined(FH_UART2_RX) && (FH_UART2_RX) > 0
FH_DECLARE_AVR_UART_BUF(2, false, FH_UART2_RX);
#endif
#endif

#ifdef FH_HAS_UART3
FH_DECLARE_AVR_UART(3);

#if defined(FH_UART3_TX) && (FH_UART3_TX) > 0
FH_DECLARE_AVR_UART_BUF(3, true, FH_UART3_TX);
FH_DECLARE_AVR_UART_TXR(3);
#endif

#if defined(FH_UART3_RX) && (FH_UART3_RX) > 0
FH_DECLARE_AVR_UART_BUF(3, false, FH_UART3_RX);
#endif
#endif

#undef FH_DECLARE_AVR_UART_BUF
#undef FH_DECLARE_AVR_UART