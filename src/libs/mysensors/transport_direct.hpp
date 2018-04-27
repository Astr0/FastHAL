#ifndef FH_LIBS_MYSENSORS_TRANSPORT_DIRECT_H_
#define FH_LIBS_MYSENSORS_TRANSPORT_DIRECT_H_

#include "../../mp/holder.hpp"
#include "mymessage.hpp"
#include "mytransport.hpp"

namespace fasthal::mysensors{
    // transport interface
    // bool send(msg&)
    // bool update(msg&)
    // bool begin()

    template<typename TNTransport>
    class transport_direct
    {
        TNTransport _ntransport;
        auto& ntransport() {return _ntransport; }
        auto& context() {return ntransport().context(); }
    public:
        constexpr transport_direct(TNTransport ntransport)
            : _ntransport(ntransport)
            {}

        bool send(mymessage& msg) {
            return ntransport().send(msg.destination, reinterpret_cast<const std::uint8_t*>(&msg), msg.total_length(), msg.request_ack());
        }

        bool update(mymessage& msg) {
            auto ok = ntransport().update(reinterpret_cast<std::uint8_t*>(&msg), mymessage::max_message_size);
            if (ok && (msg.destination == context().address()))
                mytransport::handle_ack(*this, msg, context().address());
            return ok;
        }

        bool begin() {
            auto ok = ntransport().begin();
            if (ok)
                ntransport().address_set();
            return ok;
        }
        
    };
}

#endif