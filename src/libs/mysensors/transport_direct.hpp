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
    public:
        constexpr transport_direct(TNTransport ntransport)
            : _ntransport(ntransport)
            {}

        template<class TNode>
        bool send(TNode& node, mymessage& msg) {
            return ntransport().send(node, msg.destination, reinterpret_cast<const std::uint8_t*>(&msg), msg.total_length(), msg.request_ack());
        }

        template<class TNode>
        bool update(TNode& node, mymessage& msg) {
            auto ok = ntransport().update(node, reinterpret_cast<std::uint8_t*>(&msg), mymessage::max_message_size);
            if (ok && (msg.destination == node.address()))
                mytransport::handle_ack(node, *this, msg);
            return ok;
        }

        template<class TNode>
        bool begin(TNode& node) {
            auto ok = ntransport().begin(node);
            if (ok)
                ntransport().address_set(node);
            return ok;
        }
        
    };
}

#endif