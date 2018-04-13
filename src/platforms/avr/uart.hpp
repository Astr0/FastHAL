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


        template<unsigned VNum>
        struct uart_impl{
            static constexpr auto available = false;
            static constexpr auto number = VNum;
        };

        // transmitter target, don't depend on transmitter!
        template<unsigned VNum, template<typename> typename TTrans>
        struct uart_tx_impl{
            using uart_t = uart_impl<VNum>;

            struct lazy{
                using trans_t = TTrans<uart_tx_impl<VNum, TTrans>>;
                static constexpr auto async_tx = trans_t::async;
            };
            
            static inline void tx(std::uint8_t c) {
                // write udr
                write_(uart_t::udr, c);
                // clear txc by setting
                set_(uart_t::txc);
            }

            // write 1 byte, for transmitter communication
            static inline bool try_write(std::uint8_t c){
                #ifdef FH_UART_FLUSH_SAFE
                // enable TX
                enable_(uart_t::txen);
                #endif

                if (read_(uart_t::udre)){
                    tx(c);
                    return true;
                }
                return false;
            }

             // tries to write something from transmitter
            static inline void try_write_sync(){
                static_assert(lazy::async_tx, "Not async uart, shouldn't be here");
                try_irq_force(uart_t::irq_txr);
            }

            // called by transmitter if it has some data
            static inline void write_async(){
                static_assert(lazy::async_tx, "Not async uart, shouldn't be here");
                enable_(uart_t::irq_txr);
            }

            

            static inline void txr_irq()
            {
                static_assert(lazy::async_tx, "Not async UART Transmitter");
                auto c = lazy::trans_t::next();

                tx(c.byte);

                // not ok, disable async transmit
                if (c.last)
                    disable_(uart_t::irq_txr);
            }
        };
    }

    struct uart_handler{
        // controls tx irq, if true - tx will be async and call tx_done on handler
        static constexpr bool async_tx = false;
        // controls rx irq, if true - rx will be async and call rx_done on handler
        static constexpr bool async_rx = false;
        // check if something was written so flush won't hang =/
        static constexpr bool safe_flush = false;
    };

    template<unsigned VNum, class THandler = uart_handler>
    class uart: details::uart_impl<VNum> {
        using uart_t = details::uart_impl<VNum>;
        static_assert(uart_t::available, "UART not available");

        using handler_t = THandler;
        static constexpr auto async_tx = handler_t::async_tx;
        static constexpr auto async_rx = handler_t::async_rx;
        static constexpr auto safe_flush = handler_t::safe_flush;

        static inline void do_tx(std::uint8_t c) {
            // write udr
            write_(uart_t::udr, c);
            // clear txc by setting
            set_(uart_t::txc);
        }        
    public:
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

                // disable tx for safe flush - we will use it to check if anything written
                enable(uart_t::txen, integral_constant<bool, !safe_flush>{}), 

                // disable tx ready irq
                disable(uart_t::irq_txr),

                // enable rx
                enable(uart_t::rxen),
                // enable rx ready irq
                // TODO: If enabled
                enable(uart_t::irq_rxc, integral_constant<bool, async_rx>{})
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
        static inline void tx(std::uint8_t b){
            static_assert(!async_tx, "only for sync tx");

            if constexpr(safe_flush){
                // enable TX
                enable_(uart_t::txen);
            }
            
            while (!read_(uart_t::udre));
            
            do_tx(b);
        }

        static bool tx(buffer_view buf){
            if constexpr(async_tx){
                // TODO: do async stuff
            } else{
                while (!buf.empty()) 
                    tx(buf.next());
                return true;
            }
        }

        static inline void flush() {        
            if constexpr(safe_flush){
                // If we have never written a byte, no need to flush. This special
                // case is needed since there is no way to force the TXC (transmit
                // complete) bit to 1 during initialization
                if (!enabled_(uart_t::txen))
                    return;
            }

            // wait for TX completed
            if constexpr(async_tx) {
                // buffered mode
                // write data buffer empty interrupt enabled (we have data in write buffer)
                // or transmission not complete (no data in write buffer, but it's not actually transmitted)
                while (enabled_(uart_t::irq_txr) || read_(uart_t::txc))
                    try_irq_force(uart_t::irq_txr);
            } else{
                // wait for TX completed
                while (!read_(uart_t::txc));
            }
        }

        // --------------------------------- RX
        static constexpr bool available(){
            static_assert(!async_rx, "only for sync rx");

            // check if we have something
            if (!read_(uart_t::rxc))
                return false;
            // check if it's ok. upe 0 == ok
            if (!read_(uart_t::upe))
                return true;
            // discard error data and return false
            read_(uart_t::udr);
            return false;   
        }

        template<bool dirty = false>
        static auto rx(){
            static_assert(!async_rx, "only for sync rx");

            if constexpr(!dirty){
                while (!available());
            }
            return read_(uart_t::udr);
        }
    };
    
    #include "uart_impl.hpp"

    // ************************* RX **************************

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