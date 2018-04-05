#ifndef FH_AVR_UART_H_
#define FH_AVR_UART_H_

#include "registers.hpp"
#include "../../fields/actions.hpp"
#include "../../std/type_traits.hpp"
#include "../../std/std_fake.hpp"

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

        template<unsigned VNum, unsigned VTxSize, unsigned VRxSize>
        struct avr_uart{
            using impl_t = avr_uart_impl<VNum>;
            static_assert(impl_t::available, "UART not available");

            static constexpr auto u2x = impl_t::u2x;
            static constexpr auto ubrr = impl_t::ubrr;
        };

        template<class T>
        struct is_avr_uart_impl: std::false_type {};

        template<unsigned VNum, unsigned VTxSize, unsigned VRxSize>
        struct is_avr_uart_impl<avr_uart<VNum, VTxSize, VRxSize>>: std::true_type {};

        template<class T>
        using enable_if_avr_uart = std::enable_if_c<is_avr_uart_impl<std::base_type_t<T>>::value>;
    }

template<unsigned VNum, unsigned VTxSize, unsigned VRxSize>
constexpr auto makeUart(){
    static_assert(details::avr_uart_impl<VNum>::available, "UART not available");
    return details::avr_uart<VNum, VTxSize, VRxSize>{};
}

template<class T, 
    typename TBaud = decltype(baud_def),
    typename TConfig = decltype(serial_config_v<serial_config::def>),
    details::enable_if_avr_uart<T> dummy = nullptr>
constexpr auto begin(T uart, TBaud baud = baud_def, serial_config config = serial_config_v<serial_config::def>){
    auto config_ = static_cast<std::uint8_t>(config);
    #if defined(__AVR_ATmega8__)
    config_ |= 0x80; // select UCSRC register (shared with UBRRH)
    #endif

    // Try u2x mode first
    auto baud_ = static_cast<std::uint16_t>((F_CPU / 4 / baud - 1) / 2);

    // hardcoded exception for 57600 for compatibility with the bootloader
    // shipped with the Duemilanove and previous boards and the firmware
    // on the 8U2 on the Uno and Mega 2560. Also, The baud_ cannot
    // be > 4095, so switch back to non-u2x mode if the baud rate is too
    // low.
    constexpr auto no_u2x = ((F_CPU == 16000000UL) && (baud == 57600)) || (baud_ > 4095);
    
    if (no_u2x)
    {
        baud_ = (F_CPU / 8 / baud - 1) / 2;
    }

    return 0;
    // return combine(
    //     // enable u2x
    //     set(uart::u2x, !no_u2x),
    //     // assign the baud_, a.k.a. ubrr (USART Baud Rate Register)
    //     write(uart::ubrr, baud_),
    //     //set the data bits, parity, and stop bits
    //     write(uart::ucsrc, config_),

    //     // enable tx
    //     enable(uart::tx), // todo: use if for written?
    //     // disable tx ready irq
    //     disable(uart::irq_txr),
    //     // enable rx
    //     enable(uart::rx),
    //     // enable rx ready irq
    //     enable(uart::irq_rx)
    // );
}


}
#endif