#pragma once

#ifndef AVR_SERIAL_H_
#define AVR_SERIAL_H_

#include "maskutils.h"
#include <inttypes.h>

#define FASTHAL_WRAPREG(REGNAME, CODE)\
struct REGNAME ## Reg ## CODE \
{\
static decltype(REGNAME ## CODE)& value(){return REGNAME ## CODE;}\
};

#define FASTHAL_DECLAREUART(CODE)\
namespace priv\
{\
	FASTHAL_WRAPREG(UBRRH, CODE)\
	FASTHAL_WRAPREG(UBRRL, CODE)\
	FASTHAL_WRAPREG(UCSRA, CODE)\
	FASTHAL_WRAPREG(UCSRB, CODE)\
	FASTHAL_WRAPREG(UCSRC, CODE)\
	FASTHAL_WRAPREG(UDR, CODE)\
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
    > Uart ## CODE;

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

            // enable RX, TX and RX interrupt
            enableRx();
            enableTx();
            enableRxIrq();
            disableTxReadyIrq();
        }

        static void end(){
            // disable UART
            disableRx();
            disableTx();
            disableRxIrq();
            disableTxReadyIrq();
        }

        static void enableRx() { sbi(ucsrb::value(), RXENbit); }
        static void disableRx() { cbi(ucsrb::value(), RXENbit); }

        static void enableTx() { sbi(ucsrb::value(), TXENbit); }
        static void disableTx() { cbi(ucsrb::value(), TXENbit); }
        
        static void enableRxIrq() { sbi(ucsrb::value(), RXCIEbit); }
        static void disableRxIrq() { cbi(ucsrb::value(), RXCIEbit); }
        
        static bool enabledTxReadyIrq() { return bit_is_set(ucsrb::value(), UDRIEbit); }
        static void enableTxReadyIrq(){ sbi(ucsrb::value(), UDRIEbit); }
        static void disableTxReadyIrq(){ sbi(ucsrb::value(), UDRIEbit); }

        static bool txReady() { return bit_is_set(ucsra::value(), UDREbit); }
        static bool txDone() { return bit_is_clear(ucsra::value(), TXC0); }
        static void tx(uint8_t c) { udr::value() = c; clearTxReady(); }
        static void clearTxReady(){ sbi(ucsra::value(), TXCbit); }
    };

    template<
        unsigned int TxBufferSize, 
        unsigned int RxBufferSize,
        class Uart
        >
    class AvrSerial{
    private:
        typedef typename common::NumberType<RxBufferSize>::Result RxBufferIndex;
        typedef typename common::NumberType<RxBufferSize * 2>::Result RxBufferIndex2;
        typedef typename common::NumberType<TxBufferSize>::Result TxBufferIndex;
    
        volatile RxBufferIndex _rx_head;
        volatile RxBufferIndex _rx_tail;
        volatile TxBufferIndex _tx_head;
        volatile TxBufferIndex _tx_tail;
        bool _written;

        unsigned char _rx_buffer[RxBufferSize];
        unsigned char _tx_buffer[TxBufferSize];
    public:
        AvrSerial():
            _rx_head(0), _rx_tail(0),
            _tx_head(0), _tx_tail(0),
            _written(false)
        {
        }

        void begin(uint32_t baud){
            begin(baud, SerialConfig::D8N1);
        }

        void begin(uint32_t baud, uint8_t config){
            _written = false;
            Uart::begin(baud, config);
        }

        void end(){
            // wait for transmission of outgoing data
            flush();

            Uart::end();

            // clear any received data
            _rx_head = _rx_tail;
        }

        RxBufferIndex available()
        {
          return ((RxBufferIndex2)(RxBufferSize + _rx_head - _rx_tail)) % RxBufferSize;
        }

        bool availableAny(){
            return _rx_head != _rx_tail;
        }

        uint8_t peek(){
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

        TxBufferIndex availableForWrite()
        {
            TxBufferIndex head;
            RxBufferIndex tail;
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
                // interrupts are disabled
                if (NoInterrupts::enabled()){
                    // check if tx ready manually
                    if (Uart::txReady())
                        _tx_udr_empty_irq();
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
                if (NoInterrupts::enabled()) {
                    // Interrupts are disabled, so we'll have to poll the data
                    // register empty flag ourselves. If it is set, pretend an
                    // interrupt has happened and call the handler to free up
                    // space for us.
                    if(Uart::txReady())
                        _tx_udr_empty_irq();
                } 
            }

            _tx_buffer[_tx_head] = c;
            _tx_head = i;

            Uart::enableTxReadyIrq();

            return true;
        } 

        void _tx_udr_empty_irq()
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


}

#endif