#ifndef FH_UART_H_
#define FH_UART_H_

#include "registers.hpp"
#include "interrupts.hpp"
#include "../../fields/actions.hpp"
#include "../../std/type_traits.hpp"
#include "../../std/std_fake.hpp"
#include "../../std/std_types.hpp"
#include "../../mp/const_list.hpp"
#include "../../utils/ringbuffer.hpp"
#include "../../streams/stream.hpp"

namespace fasthal{
    // serial config
    enum class serial_config{
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
        D8O2 = 0x3E,
        def = D8N1
    };
    template<serial_config V>
    constexpr auto serial_config_v = integral_constant<serial_config, V>{};
    template<std::uint32_t V>
    constexpr auto baud_v = integral_constant<std::uint32_t, V>{};
    constexpr auto baud_def = baud_v<9600>;

    template<unsigned VNum>
    struct uart{
        static constexpr auto available = false;
        static constexpr auto number = VNum;
    };

    namespace details{
        template<unsigned VNum>
        struct uart_trans{
            using type = void;
        };

        template<unsigned VNum>
        static constexpr auto uart_async_tx = !std::is_same<typename uart_trans<VNum>::type, void>::value;
    }

    using uart_datatype_t = std::uint8_t;

    // ************ BEGIN ************
    namespace details{
        template<typename TBaud>
        constexpr auto inline calc_uart_baud(TBaud baud){
            // Try u2x mode first
            auto baud_ = static_cast<std::uint16_t>((F_CPU / 4 / baud - 1) / 2);

            // hardcoded exception for 57600 for compatibility with the bootloader
            // shipped with the Duemilanove and previous boards and the firmware
            // on the 8U2 on the Uno and Mega 2560. Also, The baud_ cannot
            // be > 4095, so switch back to non-u2x mode if the baud rate is too
            // low.        
            auto no_u2x = ((F_CPU == 16000000UL) && (baud == 57600)) || (baud_ > 4095);
            
            if (no_u2x)
            {
                baud_ = (F_CPU / 8 / baud - 1) / 2;
            }
            
            return mp::make_const_list(baud_, !no_u2x);
        }

        template<typename TBaud, TBaud VBaud>
        constexpr auto inline calc_uart_baud(integral_constant<TBaud, VBaud> baud){
            constexpr auto result = calc_uart_baud(VBaud);
            return mp::make_const_list(
                integral_constant<std::uint16_t, mp::get<0>(result)>{}, 
                integral_constant<bool, mp::get<1>(result)>{}
                );
        }

        template<typename TConfig>
        constexpr auto inline calc_uart_config(TConfig config){
            auto config_ = static_cast<std::uint8_t>(config);
            #if defined(__ATmega8__)
            config_ |= 0x80; // select UCSRC register (shared with UBRRH)
            #endif
            return config_;
        }

        template<typename TConfig, TConfig VConfig>
        constexpr auto calc_uart_config(integral_constant<TConfig, VConfig> config){
            constexpr auto config_ = calc_uart_config(VConfig);
            return integral_constant<std::uint8_t, config_>{};
        }
    }

    template<unsigned VNum, 
        typename TBaud = decltype(baud_def),
        typename TConfig = decltype(serial_config_v<serial_config::def>)>
    constexpr auto inline begin(uart<VNum> uart, TBaud baud = baud_def, TConfig config = serial_config_v<serial_config::def>){
        using uart_t = decltype(uart);

        static_assert(uart_t::available, "UART not available");

        auto calc_ = details::calc_uart_baud(baud);
        auto baud_ = mp::get<0>(calc_);
        auto u2x_ = mp::get<1>(calc_);
        auto config_ = details::calc_uart_config(config);
        
        return combine(
            // assign the baud_, a.k.a. ubrr (USART Baud Rate Register)
            write(uart.ubrr, baud_),
            //set the data bits, parity, and stop bits
            write(uart.ucsrc, config_),

            // enable u2x
            enable(uart.u2x, u2x_),

            #ifdef FH_UART_FLUSH_SAFE
            // disable tx - we will use it to check if anything written
            disable(uart.txen), 
            #else
            enable(uart.txen), 
            #endif
            // disable tx ready irq
            disable(uart.irq_txr),

            // enable rx
            enable(uart.rxen),
            // enable rx ready irq
            // TODO: If enabled
            disable(uart.irq_rxc)
            //enable(uart_t::irq_rxc, integral_constant<bool, details::uart_has_buf<uart_t, false>>{})
        );
    }

    template<unsigned VNum, 
        typename TBaud = decltype(baud_def),
        typename TConfig = decltype(serial_config_v<serial_config::def>)>
    inline void begin_(uart<VNum> uart, TBaud baud = baud_def, TConfig config = serial_config_v<serial_config::def>){
        apply(begin(uart, baud, config));
    }

    // ************ TX ************
    namespace details{
        template<class T>
        inline void uart_tx(T uart, uart_datatype_t c){
            // write udr
            write_(T::udr, c);
            // clear txc by setting
            set_(T::txc);
        }

        template<unsigned VNum>
        inline void uart_txr_irq(uart<VNum> uart)
        {
            static_assert(uart_async_tx<VNum>, "No UART Transmitter");
            constexpr auto trans = typename uart_trans<VNum>::type{};
            static_assert(trans.async, "Not async UART Transmitter");
            auto c = next(trans);

            uart_tx(uart, c.byte);

            // not ok, disable async transmit
            if (c.last)
                disable_(uart.irq_txr);
        }
     }

    // write 1 byte, for transmitter communication
    template<unsigned VNum>
    inline bool try_write(uart<VNum> uart, uart_datatype_t c){
        #ifdef FH_UART_FLUSH_SAFE
        // enable TX
        enable_(uart.txen);
        #endif

        if (read_(uart.udre)){
            details::uart_tx(uart, c);
            return true;
        }
        return false;
    }

    // tries to write something from transmitter
    template<unsigned VNum>
    inline void try_write_sync(uart<VNum> uart){
        static_assert(details::uart_async_tx<VNum>, "Not async uart, shouldn't be here");
        try_irq_force(uart.irq_txr);
    }

    // called by transmitter if it has some data
    template<unsigned VNum>
    inline void write_async(uart<VNum> uart){
        static_assert(details::uart_async_tx<VNum>, "Not async uart, shouldn't be here");
        enable_(uart.irq_txr);
    }

    template<unsigned VNum>
    inline void flush(uart<VNum> uart) {        
        #ifdef FH_UART_FLUSH_SAFE
        // If we have never written a byte, no need to flush. This special
        // case is needed since there is no way to force the TXC (transmit
        // complete) bit to 1 during initialization
        if (!enabled_(uart_t::txen))
            return;
        #endif

        // wait for TX completed
        if constexpr(details::uart_async_tx<VNum>) {
            // buffered mode
            // write data buffer empty interrupt enabled (we have data in write buffer)
            // or transmission not complete (no data in write buffer, but it's not actually transmitted)
            while (enabled_(uart.irq_txr) || read_(uart.txc))
                try_irq_force(uart.irq_txr);
        } else{
            // wait for TX completed
            wait_hi(uart.txc);
        }
    }
   

    // ************************* END **************************
    template<unsigned VNum>
    inline constexpr auto end(uart<VNum> uart, bool doFlush = true){
        if (doFlush)
             flush(uart);
        
        return combine(
            disable(uart.rxen),
            disable(uart.irq_rxc),
            disable(uart.txen),
            disable(uart.irq_txr)
        );
    }

    template<unsigned VNum>
    inline void end_(uart<VNum> uart, bool doFlush = true){ apply(end(uart, doFlush)); }

    #include "uart_impl.hpp"

    // ************************* RX **************************
    // template<class T, details::enable_if_uart<T> dummy = nullptr>
    // constexpr bool available(T uart) {
    //     if constexpr (details::uart_has_buf<T, false>){
    //         // buffered - return if buffer has anything
    //         return !details::uart_buf<T, false>::buffer.empty();
    //     } else{
    //         // check if we have something
    //         if (!read_(T::rxc))
    //             return false;
    //         // check if it's ok. upe 0 == ok
    //         if (!read_(T::upe))
    //             return true;
    //         // discard error data and return false
    //         read_(T::udr);
    //         return false;        
    //     }
    // }

    // template<class T, details::enable_if_uart<T> dummy = nullptr>
    // constexpr uart_datatype_t read(T uart) {
    //     if constexpr (details::uart_has_buf<T, false>){
    //         // buffered - return value
    //         return details::uart_buf<T, false>::buffer.read();
    //     } else{           
    //         // TODO: direct read is meaningless - we won't keep up and aways get not available or miss bytes
    //         // direct - if we have something - read it, otherwise return 0
    //         return available(uart) ? read_(T::udr) : 0;
    //     }
    // }

    // template<class T, details::enable_if_uart<T> dummy = nullptr>
    // constexpr uart_datatype_t read_dirty(T uart) {
    //     if constexpr (details::uart_has_buf<T, false>){
    //         // buffered - return value
    //         return details::uart_buf<T, false>::buffer.read_dirty();
    //     } else{           
    //         // direct - just read udr???
    //         return read_(T::udr);
    //     }
    // }

    // namespace details{
    //     template<class T>
    //     inline void uart_rxc_irq()
    //     {
    //         static_assert(uart_has_buf<T, false>, "No UART RX buffer");

    //         if (read_(T::upe))
    //         {
    //             // we have parity error, discard value
    //             read_(T::udr);
    //         } else{
    //             // read 
    //             auto v = read_(T::udr);
    //             uart_buf<T, false>::buffer.try_write(v);
    //         }
    //     }
    // }




}
#endif