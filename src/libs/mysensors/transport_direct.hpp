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

    template<typename TContextPtr, typename TNTransportPtr>
    class direct_transport: 
        mp::holder<TContextPtr>,
        mp::holder<TNTransportPtr>
    {
        auto& ntransport() const{return *(this->mp::holder<TNTransportPtr>::get()); }
        auto& context()const {return *(this->mp::holder<TContextPtr>::get()); }
    public:
        constexpr direct_transport(TContextPtr context, TNTransportPtr ntransport)
            : mp::holder<TContextPtr>(context)
            , mp::holder<TNTransportPtr>(ntransport)
            {}

        bool send(mymessage& msg) const{
            return ntransport().send(msg.destination, reinterpret_cast<const std::uint8_t*>(&msg), msg.total_length(), msg.request_ack());
        }

        bool update(mymessage& msg) const{
            auto ok = ntransport().update(reinterpret_cast<std::uint8_t*>(&msg));
            if (ok && msg.destination == context().address())
                mytransport::handle_ack(*this, msg, context().address());
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