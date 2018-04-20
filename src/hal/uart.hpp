#ifndef FH_HAL_UART_H_
#define FH_HAL_UART_H_

#include "net.hpp"

namespace fasthal{
    // uart tx interface
    template<class TArgs>
    struct uart_tx_null{        
        static void tx(TArgs& args) { args(); }
    };

    template<class TArgs>
    struct uart_rx_null{        
        static void rx(TArgs& args) { args(); }
    };
}

#endif 