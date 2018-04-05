#ifndef FH_AVR_UART_H_
#define FH_AVR_UART_H_

#include "registers.hpp"
#include "../../fields/actions.hpp"
#include "../../std/type_traits.hpp"
#include "../../std/std_fake.hpp"

namespace fasthal{

// we have TX, RX separate. Join them into one?
namespace details{
    template<unsigned VNum>
    struct avr_uart_impl{
        static constexpr bool available = false;
    };

    template<unsigned VNum, unsigned VRxSize, unsigned VTxSize>
    struct avr_uart{

    };

    template<class T>
    struct is_avr_uart_impl<T>: std::false_type {};

    template<unsigned VNum, unsigned VRxSize, unsigned VTxSize>
    struct is_avr_uart_impl<avr_uart<VNum, RxSize, VTxSize>>: std::true_type {};

    template<class T>
    using enable_if_avr_uart = std::enable_if_c<is_avr_uart<std::base_type_t<T>>::value>;
}

template<unsigned VNum, unsigned VRxSize, unsigned VTxSize>
constexpr auto makeUart(){
    static_assert(details::avr_uart_impl<VNum>::available, "UART not available");
    return detail::avr_uart<VNum, VRxSize, VTxSize>{};
}

template<class T, details::enable_if_avr_uart<T> dummy = nullptr>
constexpr auto begin(T uart){
    // TODO
    return 0;
}


}
#endif