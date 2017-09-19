#pragma once

#ifndef AVR_SERIAL_H_
#define AVR_SERIAL_H_

#include "maskutils.h"
#include <inttypes.h>
#include <avr/interrupt.h>
#include "functions.h"

#define FASTHAL_DECLAREUART(NAME, CODE)\
namespace priv\
{\
	FASTHAL_WRAPVARIABLE(UBRRHReg ## CODE, UBRR ## CODE ## H)\
	FASTHAL_WRAPVARIABLE(UBRRLReg ## CODE, UBRR ## CODE ## L)\
	FASTHAL_WRAPVARIABLE(UCSRAReg ## CODE, UCSR ## CODE ## A)\
	FASTHAL_WRAPVARIABLE(UCSRBReg ## CODE, UCSR ## CODE ## B)\
	FASTHAL_WRAPVARIABLE(UCSRCReg ## CODE, UCSR ## CODE ## C)\
	FASTHAL_WRAPVARIABLE(UDRReg ## CODE, UDR ## CODE)\
}\
typedef AvrUart<\
    priv::UBRRHReg ## CODE, \
    priv::UBRRLReg ## CODE, \
    priv::UCSRAReg ## CODE, \
    priv::UCSRBReg ## CODE, \
    priv::UCSRCReg ## CODE, \
    priv::UDRReg ## CODE, \
    TXC ## CODE, \
    RXEN ## CODE, \
    TXEN ## CODE, \
    RXCIE ## CODE, \
    UDRIE ## CODE, \
    U2X ## CODE, \
    UPE ## CODE, \
    UDRE ## CODE \
    > Uart ## NAME;

namespace fasthal{
    enum SerialConfig{
        D5N1 = 0x00,
        D6N1 = 0x02,
        D7N1 = 0x04,
        D8N1 = 0x06,
        D5N2 = 0x08,
        D6N2 = 0x0A,
        D7N2 = 0x0C,
        D8N2 = 0x0E,
        D5E1 = 0x20,
        D6E1 = 0x22,
        D7E1 = 0x24,
        D8E1 = 0x26,
        D5E2 = 0x28,
        D6E2 = 0x2A,
        D7E2 = 0x2C,
        D8E2 = 0x2E,
        D5O1 = 0x30,
        D6O1 = 0x32,
        D7O1 = 0x34,
        D8O1 = 0x36,
        D5O2 = 0x38,
        D6O2 = 0x3A,
        D7O2 = 0x3C,
        D8O2 = 0x3E
    };

    template<
        class ubrrh,
        class ubrrl,
        class ucsra,
        class ucsrb,
        class ucsrc,
        class udr,

        uint8_t TXCbit,
        
        uint8_t RXENbit,
        uint8_t TXENbit,
        uint8_t RXCIEbit,

        uint8_t UDRIEbit,
        uint8_t U2Xbit,
        uint8_t UPEbit,
        uint8_t UDREbit
        >
    class AvrUart{        
    public:
        static void begin(uint32_t baud){
            begin(baud, SerialConfig::D8N1);
        }

        static void begin(uint32_t baud, uint8_t config)
        {
            // Try u2x mode first
            uint16_t baud_setting = (F_CPU / 4 / baud - 1) / 2;

            // hardcoded exception for 57600 for compatibility with the bootloader
            // shipped with the Duemilanove and previous boards and the firmware
            // on the 8U2 on the Uno and Mega 2560. Also, The baud_setting cannot
            // be > 4095, so switch back to non-u2x mode if the baud rate is too
            // low.
            if (((F_CPU == 16000000UL) && (baud == 57600)) || (baud_setting > 4095))
            {
                ucsra::value() = 0;
                baud_setting = (F_CPU / 8 / baud - 1) / 2;
            } else{
                ucsra::value() = 1 << U2Xbit;            
            }

            // assign the baud_setting, a.k.a. ubrr (USART Baud Rate Register)
            ubrrh::value() = baud_setting >> 8;
            ubrrl::value() = baud_setting;

            //set the data bits, parity, and stop bits
            #if defined(__AVR_ATmega8__)
            config |= 0x80; // select UCSRC register (shared with UBRRH)
            #endif

            ucsrc::value() = config;
        }

        static inline void enableRx() { sbi(ucsrb::value(), RXENbit); }
        static inline void disableRx() { cbi(ucsrb::value(), RXENbit); }

        static inline void enableTx() { sbi(ucsrb::value(), TXENbit); }
        static inline void disableTx() { cbi(ucsrb::value(), TXENbit); }
        
        static inline void enableRxIrq() { sbi(ucsrb::value(), RXCIEbit); }
        static inline void disableRxIrq() { cbi(ucsrb::value(), RXCIEbit); }
        
        static inline bool enabledTxReadyIrq() { return bit_is_set(ucsrb::value(), UDRIEbit); }
        static inline void enableTxReadyIrq(){ sbi(ucsrb::value(), UDRIEbit); }
        static inline void disableTxReadyIrq(){ cbi(ucsrb::value(), UDRIEbit); }
        static inline bool shouldRunTxReadyIrq(){
            return NoInterrupts::enabled() && txReady();
        }

        static inline bool txReady() { return bit_is_set(ucsra::value(), UDREbit); }
        static inline bool txDone() { return bit_is_clear(ucsra::value(), TXC0); }
        static inline void tx(uint8_t c) { udr::value() = c; clearTxReady(); }
        static inline void clearTxReady(){ sbi(ucsra::value(), TXCbit); }

        static inline uint8_t rx(){ return udr::value(); }
        static inline bool rxOk(){return bit_is_clear(ucsra::value(), UPEbit); }
    };

    template<class Uart, unsigned int RxBufferSize>
    class AvrUartRx{
    private:
        typedef typename common::NumberType<RxBufferSize>::Result RxBufferIndex;
        typedef typename common::NumberType<RxBufferSize * 2>::Result RxBufferIndex2;

        volatile RxBufferIndex _rx_head;
        volatile RxBufferIndex _rx_tail;

        unsigned char _rx_buffer[RxBufferSize];
    public:
        AvrUartRx():  _rx_head(0), _rx_tail(0)
        {            
        }

        inline void begin(){
            Uart::enableRx();
            Uart::enableRxIrq();
        }

        inline void end(){
            Uart::disableRx();
            Uart::disableRxIrq();
            
            // clear any received data
            _rx_head = _rx_tail;
        }

        RxBufferIndex available()
        {
          return ((RxBufferIndex2)(RxBufferSize + _rx_head - _rx_tail)) % RxBufferSize;
        }

        inline bool availableAny(){
            return _rx_head != _rx_tail;
        }

        inline uint8_t peek(){
            return _rx_buffer[_rx_tail];
        }

        uint8_t dirtyRead(){
            unsigned char c = _rx_buffer[_rx_tail];
            _rx_tail = (RxBufferIndex)(_rx_tail + 1) % RxBufferSize;
            return c;
        }

        uint8_t read(){
            if (_rx_head == _rx_tail) {
                return 0;
            } else {
                unsigned char c = _rx_buffer[_rx_tail];
                _rx_tail = (RxBufferIndex)(_rx_tail + 1) % RxBufferSize;
                return c;
            } 
        }

        inline void _rx_ready_irq(void)
        {
          if (Uart::rxOk()) {
            // No Parity error, read byte and store it in the buffer if there is
            // room
            unsigned char c = Uart::rx();
            rx_buffer_index_t i = (unsigned int)(_rx_head + 1) % RxBufferSize;
        
            // if we should be storing the received character into the location
            // just before the tail (meaning that the head would advance to the
            // current location of the tail), we're about to overflow the buffer
            // and so we don't write the character or advance the head.
            if (i != _rx_tail) {
              _rx_buffer[_rx_head] = c;
              _rx_head = i;
            }
          } else {
            // Parity error, read byte but discard it
            Uart::rx();
          };
        }
    };

    template<
        class Uart,        
        unsigned int TxBufferSize>
    class AvrUartTx{
    private:
        typedef typename common::NumberType<TxBufferSize>::Result TxBufferIndex;
    
        volatile TxBufferIndex _tx_head;
        volatile TxBufferIndex _tx_tail;
        bool _written;

        unsigned char _tx_buffer[TxBufferSize];
    public:
        AvrUartTx():           
            _tx_head(0), _tx_tail(0)
        {
        }

        inline void begin(){
            _written = false;
            Uart::enableTx();
            Uart::disableTxReadyIrq();
        }

        inline void end(){
            // wait for transmission of outgoing data
            flush();

            Uart::disableTx();
            Uart::disableTxReadyIrq();
        }

        
        TxBufferIndex availableForWrite()
        {
            TxBufferIndex head;
            TxBufferIndex tail;
            if (TxBufferSize > 256){
                NoInterrupts noInterrupts;
                
                head = _tx_head;
                tail = _tx_tail;
            } else{
                head = _tx_head;
                tail = _tx_tail;
            }

            return (head >= tail) ? (TxBufferSize - 1 - head + tail) : (tail - head - 1);
        }

        void flush(){
            // If we have never written a byte, no need to flush. This special
            // case is needed since there is no way to force the TXC (transmit
            // complete) bit to 1 during initialization
            if (!_written)
                return;

            // write data buffer empty interrupt enabled (we have data in HW write buffer)
            // or transmission not complete (no data in write buffer, but it's not actually transmitted)
            while (Uart::enabledTxReadyIrq() || !Uart::txDone()){
                // check if tx ready manually
                if (Uart::shouldRunTxReadyIrq()){
                    _tx_ready_irq();
                }
            }

            // If we get here, nothing is queued anymore (DRIE is disabled) and
            // the hardware finished tranmission (TXC is set).
        }

        bool write(uint8_t c)
        {
            _written = true;
            // If the buffer and the data register is empty, just write the byte
            // to the data register and be done. This shortcut helps
            // significantly improve the effective datarate at high (>
            // 500kbit/s) bitrates, where interrupt overhead becomes a slowdown.
            if (_tx_head == _tx_tail && Uart::txReady()) {
                Uart::tx(c);

                return true;
            }
            TxBufferIndex i = (_tx_head + 1) % TxBufferSize;

            // If the output buffer is full, there's nothing for it other than to 
            // wait for the interrupt handler to empty it a bit
            while (i == _tx_tail) {
                if (Uart::shouldRunTxReadyIrq()) {
                    // check if tx ready manually
                    _tx_ready_irq();
                } 
            }

            _tx_buffer[_tx_head] = c;
            _tx_head = i;

            Uart::enableTxReadyIrq();

            return true;
        } 

        void _tx_ready_irq()
        {
          // If interrupts are enabled, there must be more data in the output
          // buffer. Send the next byte
          unsigned char c = _tx_buffer[_tx_tail];
          _tx_tail = (_tx_tail + 1) % TxBufferSize;
        
          Uart::tx(c);
        
          if (_tx_head == _tx_tail) {
            // Buffer empty, so disable interrupts
            Uart::disableTxReadyIrq();
          }
        }
    };

    #define FASTHAL_UARTRX(Number, RX_Vect, RxSize) \
        namespace fasthal{\
            ::fasthal::AvrUartRx<::fasthal::Uart ## Number, RxSize> Uart ## Number ## Rx;\
            ISR(RX_Vect) { Uart ## Number ## Rx._rx_ready_irq(); }\
        }

    #define FASTHAL_UARTTX(Number, UDRE_Vect, TxSize) \
        namespace fasthal{\
            ::fasthal::AvrUartTx<::fasthal::Uart ## Number, TxSize> Uart ## Number ## Tx;\
            ISR(UDRE_Vect) { Uart ## Number ## Tx._tx_ready_irq(); }\
        }
        
    #if defined(UBRRH) || defined(UBRR0H)
        #if defined(USART_RX_vect)
            #define FASTHAL_RX0_vect USART_RX_vect
        #elif defined(USART_RXC_vect)
            // ATmega8
            #define FASTHAL_RX0_vect USART_RXC_vect
        #elif defined(USART0_RX_vect)
            #define FASTHAL_RX0_vect USART0_RX_vect
        #elif defined(UART0_RX_vect)
            #define FASTHAL_RX0_vect UART0_RX_vect
        #else
            #error "Don't know what the Data Received vector is called for UART0"
        #endif 

        #if defined(UART_UDRE_vect)
            #define FASTHAL_UDRE0_vect UART_UDRE_vect
        #elif defined(USART_UDRE_vect)
            #define FASTHAL_UDRE0_vect USART_UDRE_vect
        #elif defined(UART0_UDRE_vect)
            #define FASTHAL_UDRE0_vect UART0_UDRE_vect
        #elif defined(USART0_UDRE_vect)
            #define FASTHAL_UDRE0_vect USART0_UDRE_vect
        #else
          #error "Don't know what the Data Register Empty vector is called for UART0"
        #endif 
    
        #if defined(UBRRH)
            FASTHAL_DECLAREUART(0, )
        #else
            FASTHAL_DECLAREUART(0, 0)
        #endif

        #define FASTHAL_UART0RX(RxSize) FASTHAL_UARTRX(0, FASTHAL_RX0_vect, RxSize)
        #define FASTHAL_UART0TX(TxSize) FASTHAL_UARTTX(0, FASTHAL_UDRE0_vect, TxSize)
        #define FASTHAL_UART0(RxSize, TxSize) \
            FASTHAL_UART0RX(RxSize)\
            FASTHAL_UART0TX(TxSize)
    #endif

    #if defined(UBRR1H)
        #if defined(USART1_RX_vect)
            #define FASTHAL_RX1_vect USART1_RX_vect
        #elif defined(UART1_RX_vect)
            #define FASTHAL_RX1_vect UART1_RX_vect
        #else
            #error "Don't know what the Data Received vector is called for UART1"
        #endif 

        #if defined(UART1_UDRE_vect)
            #define FASTHAL_UDRE1_vect UART1_UDRE_vect
        #elif defined(USART1_UDRE_vect)
            #define FASTHAL_UDRE1_vect USART1_UDRE_vect
        #else
          #error "Don't know what the Data Register Empty vector is called for UART1"
        #endif 

        FASTHAL_DECLAREUART(1, 1)

        #define FASTHAL_UART1RX(RxSize) FASTHAL_UARTRX(1, FASTHAL_RX1_vect, RxSize)
        #define FASTHAL_UART1TX(TxSize) FASTHAL_UARTTX(1, FASTHAL_UDRE1_vect, TxSize)
        #define FASTHAL_UART1(RxSize, TxSize) \
            FASTHAL_UART1RX(RxSize)\
            FASTHAL_UART1TX(TxSize)
  #endif

  #if defined(UBRR2H)
        #if defined(USART2_RX_vect)
            #define FASTHAL_RX2_vect USART2_RX_vect
        #elif defined(UART2_RX_vect)
            #define FASTHAL_RX2_vect UART2_RX_vect
        #else
            #error "Don't know what the Data Received vector is called for UART2"
        #endif 

        #if defined(UART2_UDRE_vect)
            #define FASTHAL_UDRE2_vect UART2_UDRE_vect
        #elif defined(USART2_UDRE_vect)
            #define FASTHAL_UDRE2_vect USART2_UDRE_vect
        #else
          #error "Don't know what the Data Register Empty vector is called for UART2"
        #endif 

        FASTHAL_DECLAREUART(2, 2)

        #define FASTHAL_UART2RX(RxSize) FASTHAL_UARTRX(2, FASTHAL_RX2_vect, RxSize)
        #define FASTHAL_UART2TX(TxSize) FASTHAL_UARTTX(2, FASTHAL_UDRE2_vect, TxSize)
        #define FASTHAL_UART2(RxSize, TxSize) \
            FASTHAL_UART2RX(RxSize)\
            FASTHAL_UART2TX(TxSize)
  #endif

  #if defined(UBRR3H)
        #if defined(USART3_RX_vect)
            #define FASTHAL_RX3_vect USART3_RX_vect
        #elif defined(UART3_RX_vect)
            #define FASTHAL_RX3_vect UART3_RX_vect
        #else
            #error "Don't know what the Data Received vector is called for UART3"
        #endif 

        #if defined(UART3_UDRE_vect)
            #define FASTHAL_UDRE3_vect UART3_UDRE_vect
        #elif defined(USART3_UDRE_vect)
            #define FASTHAL_UDRE3_vect USART3_UDRE_vect
        #else
          #error "Don't know what the Data Register Empty vector is called for UART3"
        #endif 

        FASTHAL_DECLAREUART(3, 3)

        #define FASTHAL_UART3RX(RxSize) FASTHAL_UARTRX(3, FASTHAL_RX3_vect, RxSize)
        #define FASTHAL_UART3TX(TxSize) FASTHAL_UARTTX(3, FASTHAL_UDRE3_vect, TxSize)
        #define FASTHAL_UART3(RxSize, TxSize) \
            FASTHAL_UART3RX(RxSize)\
            FASTHAL_UART3TX(TxSize)
    #endif    

  
}

#endif