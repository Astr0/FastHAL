// USART0 RX IRQ
#if defined(USART_RX_vect_num)
    #define FH_IRQ_RXC0 USART_RX_vect_num
#elif defined(USART_RXC_vect_num)
    #define FH_IRQ_RXC0 USART_RXC_vect_num
#elif defined(USART0_RX_vect_num)
    #define FH_IRQ_RXC0 USART0_RX_vect_num
#elif defined(UART0_RX_vect_num)
    #define FH_IRQ_RXC0 UART0_RX_vect_num
#endif 

// USART0 TX Buffer empty (TX Ready)
#if defined(UART_UDRE_vect_num)
    #define FH_IRQ_TXR0 UART_UDRE_vect_num
#elif defined(USART_UDRE_vect_num)
    #define FH_IRQ_TXR0 USART_UDRE_vect_num
#elif defined(UART0_UDRE_vect_num)
    #define FH_IRQ_TXR0 UART0_UDRE_vect_num
#elif defined(USART0_UDRE_vect_num)
    #define FH_IRQ_TXR0 USART0_UDRE_vect_num
#endif 

// USART0 TX Completed
#if defined(USART_TX_vect_num)
    #define FH_IRQ_TXC0 USART_TX_vect_num
#elif defined(USART_TXC_vect_num)
    #define FH_IRQ_TXC0 USART_TXC_vect_num
#elif defined(USART0_TX_vect_num)
    #define FH_IRQ_TXC0 USART0_TX_vect_num
#elif defined(UART0_TX_vect_num)
    #define FH_IRQ_TXC0 UART0_TX_vect_num
#endif 

// USART1 RX IRQ
#if defined(USART1_RX_vect_num)
    #define FH_IRQ_RXC1 USART1_RX_vect_num
#elif defined(UART1_RX_vect_num)
    #define FH_IRQ_RXC1 UART1_RX_vect_num
#endif 

// USART1 TX Buffer empty (TX Ready)
#if defined(UART1_UDRE_vect_num)
    #define FH_IRQ_TXR1 UART1_UDRE_vect_num
#elif defined(USART1_UDRE_vect_num)
    #define FH_IRQ_TXR1 USART1_UDRE_vect_num
#endif 

// USART1 TX Completed
#if defined(USART1_TX_vect_num)
    #define FH_IRQ_TXC1 USART1_TX_vect_num
#elif defined(UART1_TX_vect_num)
    #define FH_IRQ_TXC1 UART1_TX_vect_num
#endif 


// USART2 RX IRQ
#if defined(USART2_RX_vect_num)
    #define FH_IRQ_RXC2 USART2_RX_vect_num
#elif defined(UART2_RX_vect_num)
    #define FH_IRQ_RXC2 UART2_RX_vect_num
#endif 

// USART2 TX Buffer empty (TX Ready)
#if defined(UART2_UDRE_vect_num)
    #define FH_IRQ_TXR2 UART2_UDRE_vect_num
#elif defined(USART2_UDRE_vect_num)
    #define FH_IRQ_TXR2 USART2_UDRE_vect_num
#endif 

// USART2 TX Completed
#if defined(USART2_TX_vect_num)
    #define FH_IRQ_TXC2 USART2_TX_vect_num
#elif defined(UART2_TX_vect_num)
    #define FH_IRQ_TXC2 UART2_TX_vect_num
#endif 


// USART3 RX IRQ
#if defined(USART3_RX_vect_num)
    #define FH_IRQ_RXC3 USART3_RX_vect_num
#elif defined(UART3_RX_vect_num)
    #define FH_IRQ_RXC3 UART3_RX_vect_num
#endif 

// USART3 TX Buffer empty (TX Ready)
#if defined(UART3_UDRE_vect_num)
    #define FH_IRQ_TXR3 UART3_UDRE_vect_num
#elif defined(USART3_UDRE_vect_num)
    #define FH_IRQ_TXR3 USART3_UDRE_vect_num
#endif 

// USART3 TX Completed
#if defined(USART3_TX_vect_num)
    #define FH_IRQ_TXC3 USART3_TX_vect_num
#elif defined(UART3_TX_vect_num)
    #define FH_IRQ_TXC3 UART3_TX_vect_num
#endif 

// copy bits to irqs
#ifdef FH_HAS_UART0
constexpr auto irq_txr0 = interrupt<FH_IRQ_TXR0>{};
constexpr auto irq_txc0 = interrupt<FH_IRQ_TXC0>{};
constexpr auto irq_rxc0 = interrupt<FH_IRQ_RXC0>{};
namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_txr0)>>: 
        func_fieldbit_enable<decltype(avr::udrie0)>,
        func_fieldbit_ready<decltype(avr::udre0)>
        {};
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_txc0)>>: func_fieldbit_enable<decltype(avr::txcie0)>{};
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_rxc0)>>: func_fieldbit_enable<decltype(avr::rxcie0)>{};
}
#endif
#ifdef FH_HAS_UART1
constexpr auto irq_txr1 = interrupt<FH_IRQ_TXR1>{};
constexpr auto irq_txc1 = interrupt<FH_IRQ_TXC1>{};
constexpr auto irq_rxc1 = interrupt<FH_IRQ_RXC1>{};
namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_txr1)>>: 
        func_fieldbit_enable<decltype(avr::udrie1)>,
        func_fieldbit_ready<decltype(avr::udre1)>{};
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_txc1)>>: func_fieldbit_enable<decltype(avr::txcie1)>{};
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_rxc1)>>: func_fieldbit_enable<decltype(avr::rxcie1)>{};
}
#endif
#ifdef FH_HAS_UART2
constexpr auto irq_txr2 = interrupt<FH_IRQ_TXR2>{};
constexpr auto irq_txc2 = interrupt<FH_IRQ_TXC2>{};
constexpr auto irq_rxc2 = interrupt<FH_IRQ_RXC2>{};
namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_txr2)>>: 
        func_fieldbit_enable<decltype(avr::udrie2)>,
        func_fieldbit_ready<decltype(avr::udre2)>{};
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_txc2)>>: func_fieldbit_enable<decltype(avr::txcie2)>{};
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_rxc2)>>: func_fieldbit_enable<decltype(avr::rxcie2)>{};
}
#endif
#ifdef FH_HAS_UART3
constexpr auto irq_txr3 = interrupt<FH_IRQ_TXR3>{};
constexpr auto irq_txc3 = interrupt<FH_IRQ_TXC3>{};
constexpr auto irq_rxc3 = interrupt<FH_IRQ_RXC3>{};
namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_txr3)>>: 
        func_fieldbit_enable<decltype(avr::udrie3)>,
        func_fieldbit_ready<decltype(avr::udre3)>{};
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_txc3)>>: func_fieldbit_enable<decltype(avr::txcie3)>{};
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_rxc3)>>: func_fieldbit_enable<decltype(avr::rxcie3)>{};
}
#endif