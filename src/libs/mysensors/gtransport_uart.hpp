#ifndef FH_LIBS_MYSENSORS_GTRANSPORT_UART_H_
#define FH_LIBS_MYSENSORS_GTRANSPORT_UART_H_

#include "../../mp/holder.hpp"
#include "mymessage.hpp"
#include "myprotocol_text.hpp"

namespace fasthal::mysensors{
    template <typename TUartPtr>
    class gtransport_uart: mp::holder<TUartPtr>{
        auto& uart() const{ return *(this->mp::holder<TUartPtr>::get()); }
        using protocol_t = myprotocol_text;
    public:
        constexpr gtransport_uart(TUartPtr uart)
            : mp::holder<TUartPtr>(uart) {}

        bool send(mymessage& msg) const{
            protocol_t::write(uart(), msg);
            return true;
        }

        bool begin() const{
            auto msg = mymessage{};
            send(msg.build_gw(my_internal::gateway_ready).set("Gateway startup complete."));
            // TODO
            // Send presentation of locally attached sensors (and node if applicable)
	        // presentNode();
            return true;
        }
    };
}

#endif