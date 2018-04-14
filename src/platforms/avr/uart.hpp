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
#include "../../streams/sync_streams.hpp"

#define FH_UART_TX(NUM, HANDLER) FH_ISR(FH_IRQ_TXR ## NUM, HANDLER);
#define FH_UART_RX(NUM, HANDLER) FH_ISR(FH_IRQ_RXC ## NUM, HANDLER);

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
                FH_CONST(mp::get<0>(result)), 
                FH_CONST(mp::get<1>(result))
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
            return FH_CONST(calc_uart_config(VConfig));
        }

        template<unsigned VNum>
        struct uart_impl{
            static constexpr auto available = false;
            static constexpr auto number = VNum;
        };
    }

    template<unsigned VNum>
    class uart: details::uart_impl<VNum>
    {
        using uart_t = details::uart_impl<VNum>;
        static_assert(uart_t::available, "UART not available");

        struct lazy{
            static constexpr auto async_tx = details::has_isr<uart_t::irq_txr.number>;
            static constexpr auto async_rx = details::has_isr<uart_t::irq_txc.number>;
        };

    public:
        static constexpr auto number = VNum;

        // -------------------- begin
        template<typename TBaud = decltype(baud_def), typename TConfig = decltype(serial_config_v<serial_config::def>)>
        static constexpr auto inline begin(TBaud baud = baud_def, TConfig config = serial_config_v<serial_config::def>){            
            auto calc_ = details::calc_uart_baud(baud);
            auto baud_ = mp::get<0>(calc_);
            auto u2x_ = mp::get<1>(calc_);
            auto config_ = details::calc_uart_config(config);
            
            return combine(
                // assign the baud_, a.k.a. ubrr (USART Baud Rate Register)
                write(uart_t::ubrr, baud_),
                //set the data bits, parity, and stop bits
                write(uart_t::ucsrc, config_),

                // enable u2x
                enable(uart_t::u2x, u2x_),

                // enable tx
                enable(uart_t::txen), 

                // disable tx ready irq
                disable(uart_t::irq_txr),

                // enable rx
                enable(uart_t::rxen),
                // enable rx ready irq
                enable(uart_t::irq_rxc, integral_constant<bool, lazy::async_rx>{})
            );
        }

        template<typename TBaud = decltype(baud_def), typename TConfig = decltype(serial_config_v<serial_config::def>)>
        static inline void begin_(TBaud baud = baud_def, TConfig config = serial_config_v<serial_config::def>){
            apply(begin(baud, config));
        }        

        // ---------------------- end
        static inline constexpr auto end(){
            // TODO: Flush?
            return combine(
                disable(uart_t::rxen),
                disable(uart_t::irq_rxc),
                disable(uart_t::txen),
                disable(uart_t::irq_txr)
            );
        }

        static inline void end_(){ apply(end()); }

        // ---------------------- tx
        // does tx, does not checks ready
        static inline void tx(std::uint8_t v){
            // write udr
            write_(uart_t::udr, v);
            if constexpr(!lazy::async_tx){
                // clear txc by setting. This is automatically done by interrupt in async mode
                set_(uart_t::txc);
            }
        }
        static inline bool tx_ready(){ return read_(uart_t::udre); }        
        static inline bool tx_done(){ return read_(uart_t::txc); }
        static inline void tx_start() { 
            static_assert(lazy::async_tx, "Not async TX");
            enable_(uart_t::irq_txr);             
        }
        static inline void tx_stop() { 
            static_assert(lazy::async_tx, "Not async TX");
            disable_(uart_t::irq_txr);
        }
        static inline bool tx_started() { 
            static_assert(lazy::async_tx, "Not async TX");
            return enabled_(uart_t::irq_txr); 
        }
        // ---------------------- rx
        static inline bool rx_done(){ return read_(uart_t::rxc); }
        static inline bool rx_ok() { return !read_(uart_t::upe); }
        static inline std::uint8_t rx() { return read_(uart_t::udre); }
    };

    // some "extension methods"
    template<unsigned VNum>
    // blocking TX
    static inline void tx_sync(uart<VNum> uart, std::uint8_t v) { 
        while (!uart.tx_ready());
        uart.tx(v);
    }

    // template<unsigned VNum>
    // static inline void tx_done_wait(uart<VNum> uart){
    //     while (!uart.tx_done()) ;
    // }

    template<unsigned VNum, typename Tfunc>
    // if rx can be done - do rx
    static inline bool try_rx(uart<VNum> uart, Tfunc callback){
        if (!uart.rx_done())
            return false;
        auto ok = uart.rx_ok();
        auto v = uart.rx();
        if (ok) 
            callback(v);
        return ok;
    }

    // sync tx ostream
    template<unsigned VNum>
    struct uart_sync_tx{};
    namespace details{
        template<unsigned VNum> struct is_ostream_impl<uart_sync_tx<VNum>>: std::true_type{};
    }
    template<unsigned VNum>
    void write(uart_sync_tx<VNum> uartw, std::uint8_t c){ tx_sync(uart<VNum>{}, c); }
    
    #include "uart_impl.hpp"
}
#endif