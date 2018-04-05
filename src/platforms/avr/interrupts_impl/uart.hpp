// USART0 RX IRQ
#if defined(USART_RX_vect_num)
    #define FH_IRQ_RX0 USART_RX_vect_num
#elif defined(USART_RXC_vect_num)
    #define FH_IRQ_RX0 USART_RXC_vect_num
#elif defined(USART0_RX_vect_num)
    #define FH_IRQ_RX0 USART0_RX_vect_num
#elif defined(UART0_RX_vect_num)
    #define FH_IRQ_RX0 UART0_RX_vect_num
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
    #define FH_IRQ_TX0 USART_TX_vect_num
#elif defined(USART_TXC_vect_num)
    #define FH_IRQ_TX0 USART_TXC_vect_num
#elif defined(USART0_TX_vect_num)
    #define FH_IRQ_TX0 USART0_TX_vect_num
#elif defined(UART0_TX_vect_num)
    #define FH_IRQ_TX0 UART0_TX_vect_num
#endif 

// USART1 RX IRQ
#if defined(USART1_RX_vect_num)
    #define FH_IRQ_RX1 USART1_RX_vect_num
#elif defined(UART1_RX_vect_num)
    #define FH_IRQ_RX1 UART1_RX_vect_num
#endif 

// USART1 TX Buffer empty (TX Ready)
#if defined(UART1_UDRE_vect_num)
    #define FH_IRQ_TXR1 UART1_UDRE_vect_num
#elif defined(USART1_UDRE_vect_num)
    #define FH_IRQ_TXR1 USART1_UDRE_vect_num
#endif 

// USART1 TX Completed
#if defined(USART1_TX_vect_num)
    #define FH_IRQ_TX1 USART1_TX_vect_num
#elif defined(UART1_TX_vect_num)
    #define FH_IRQ_TX1 UART1_TX_vect_num
#endif 


// USART2 RX IRQ
#if defined(USART2_RX_vect_num)
    #define FH_IRQ_RX2 USART2_RX_vect_num
#elif defined(UART2_RX_vect_num)
    #define FH_IRQ_RX2 UART2_RX_vect_num
#endif 

// USART2 TX Buffer empty (TX Ready)
#if defined(UART2_UDRE_vect_num)
    #define FH_IRQ_TXR2 UART2_UDRE_vect_num
#elif defined(USART2_UDRE_vect_num)
    #define FH_IRQ_TXR2 USART2_UDRE_vect_num
#endif 

// USART2 TX Completed
#if defined(USART2_TX_vect_num)
    #define FH_IRQ_TX2 USART2_TX_vect_num
#elif defined(UART2_TX_vect_num)
    #define FH_IRQ_TX2 UART2_TX_vect_num
#endif 


// USART3 RX IRQ
#if defined(USART3_RX_vect_num)
    #define FH_IRQ_RX3 USART3_RX_vect_num
#elif defined(UART3_RX_vect_num)
    #define FH_IRQ_RX3 UART3_RX_vect_num
#endif 

// USART3 TX Buffer empty (TX Ready)
#if defined(UART3_UDRE_vect_num)
    #define FH_IRQ_TXR3 UART3_UDRE_vect_num
#elif defined(USART3_UDRE_vect_num)
    #define FH_IRQ_TXR3 USART3_UDRE_vect_num
#endif 

// USART3 TX Completed
#if defined(USART3_TX_vect_num)
    #define FH_IRQ_TX3 USART3_TX_vect_num
#elif defined(UART3_TX_vect_num)
    #define FH_IRQ_TX3 UART3_TX_vect_num
#endif 