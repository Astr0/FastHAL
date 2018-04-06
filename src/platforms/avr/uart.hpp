#ifndef FH_AVR_UART_H_
#define FH_AVR_UART_H_

#include "registers.hpp"
#include "../../fields/actions.hpp"
#include "../../std/type_traits.hpp"
#include "../../std/std_fake.hpp"
#include "../../mp/const_list.hpp"

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
    constexpr auto baud_def = baud_v<115000>;

    namespace details{
        template<unsigned VNum>
        struct avr_uart_impl{
            static constexpr bool available = false;
        };

        template<unsigned VNum>
        struct avr_uart: avr_uart_impl<VNum>{
            using impl_t = avr_uart_impl<VNum>;
            static_assert(impl_t::available, "UART not available");
        };

        template<class T>
        struct is_avr_uart_impl: std::false_type {};

        template<unsigned VNum>
        struct is_avr_uart_impl<avr_uart<VNum>>: std::true_type {};

        template<class T>
        using enable_if_avr_uart = std::enable_if_c<is_avr_uart_impl<std::base_type_t<T>>::value>;
    }
    #include "uart_impl.hpp"

    namespace details{
        template<typename TBaud>
        constexpr auto calc_uart_baud(TBaud baud){
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
        constexpr auto calc_uart_baud(integral_constant<TBaud, VBaud> baud){
            constexpr auto result = calc_uart_baud(VBaud);
            return mp::make_const_list(
                integral_constant<std::uint16_t, mp::get<0>(result)>{}, 
                integral_constant<bool, mp::get<1>(result)>{}
                );
        }

        template<typename TConfig>
        constexpr auto calc_uart_config(TConfig config){
            auto config_ = static_cast<std::uint8_t>(config);
            #if defined(__AVR_ATmega8__)
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

    template<class T, 
        typename TBaud = decltype(baud_def),
        typename TConfig = decltype(serial_config_v<serial_config::def>),
        details::enable_if_avr_uart<T> dummy = nullptr>
    constexpr auto begin(T uart, TBaud baud = baud_def, TConfig config = serial_config_v<serial_config::def>){
        using uart_t = typename T::impl_t;

        auto calc_ = details::calc_uart_baud(baud);
        auto baud_ = mp::get<0>(calc_);
        auto u2x_ = mp::get<1>(calc_);
        auto config_ = details::calc_uart_config(config);
        
        return combine(
            // enable u2x
            enable(uart_t::u2x, u2x_),
            // assign the baud_, a.k.a. ubrr (USART Baud Rate Register)
            write(uart_t::ubrr, baud_),
            //set the data bits, parity, and stop bits
            write(uart_t::ucsrc, config_),

            // enable tx
            enable(uart_t::txen), // todo: use if for written?
            // disable tx ready irq
            disable(uart_t::irq_txr),

            // enable rx
            enable(uart_t::rxen),
            // enable rx ready irq
            enable(uart_t::irq_rxc)
        );
    }
}
#endif