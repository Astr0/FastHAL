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

    template<typename TNodePtr, typename TNTransportPtr>
    class direct_transport: 
        mp::holder<TNodePtr>,
        mp::holder<TNTransportPtr>
    {
        auto& ntransport() const{return *(this->mp::holder<TNTransportPtr>::get()); }
        auto& node()const {return *(this->mp::holder<TNodePtr>::get()); }
    public:
        constexpr direct_transport(TNodePtr node, TNTransportPtr ntransport)
            : mp::holder<TNodePtr>(node)
            , mp::holder<TNTransportPtr>(ntransport)
            {}

        bool send(mymessage& msg) const{
            return ntransport().send(msg.destination, reinterpret_cast<const std::uint8_t*>(&msg), msg.total_length(), msg.request_ack());
        }

        bool update(mymessage& msg) const{
            auto ok = ntransport().update(reinterpret_cast<std::uint8_t*>(&msg), mymessage::max_message_size);
            if (ok && (msg.destination == node().address()))
                mytransport::handle_ack(*this, msg, node().address());
            return ok;
        }

        bool begin() const{
            auto ok = ntransport().begin();
            if (ok)
                ntransport().address_set();
            return ok;
        }
        
    };
}

#endif