#ifndef FH_LIBS_MYSENSORS_GATEWAY_H_
#define FH_LIBS_MYSENSORS_GATEWAY_H_

#include "../../mp/holder.hpp"

namespace fasthal::mysensors{
    struct mygateway_default_config{

    };

    template<typename TTransportPtr, typename TGTransportPtr, typename TConfig = mygateway_default_config>
    class mygateway
        : mp::holder<TTransportPtr>
        , mp::holder<TGTransportPtr>{
        auto& transport() const { return *(this->mp::holder<TTransportPtr>::get()); }
        auto& gtransport() const { return *(this->mp::holder<TGTransportPtr>::get()); }
    public:
        constexpr mygateway(TTransportPtr transport, TGTransportPtr gtransport, TConfig config = mygateway_default_config{}):
            mp::holder<TTransportPtr>(transport),
            mp::holder<TGTransportPtr>(gtransport) {}

        bool begin() const {
            // skip all splash screen and EEPROM stuff, go directly to network
            if (!transport().begin())
                return false;

            // TODO: register ready callback and wait

            // init gateway transport
            if (!gtransport().begin())
                return false;


            return true;
        }

    };
}

#endif