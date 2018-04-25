#ifndef FH_LIBS_MYSENSORS_GATEWAY_H_
#define FH_LIBS_MYSENSORS_GATEWAY_H_

#include "../../mp/holder.hpp"
#include "mymessage.hpp"
#include "core.hpp"

namespace fasthal::mysensors{
    struct mygateway_default_config{

    };

    template<typename TTransportPtr, typename TGTransportPtr, typename TConfig = mygateway_default_config>
    class mygateway
        : mp::holder<TTransportPtr>
        , mp::holder<TGTransportPtr>{
        auto& transport() const { return *(this->mp::holder<TTransportPtr>::get()); }
        auto& gtransport() const { return *(this->mp::holder<TGTransportPtr>::get()); }

        bool send_route(mymessage& msg) const{
            if (msg.destination == gateway_address) {
        		// This is a message sent from a sensor attached on the gateway node.
        		// Pass it directly to the gateway transport layer.
		        return gtransport().send(msg);
	        }
            // TODO: Node sending
            return false;
        }

        void presentNode() const{
            present(node_sensor_id, my_sensor::node);
            // TODO: call presentation
        }

        bool present(const uint8_t sensor_id, const my_sensor sensor_type, const char *description = nullptr,
                    const bool ack = false) const
        {
            auto msg = mymessage{};

            return send_route(msg.build(gateway_address, gateway_address, sensor_id, my_command::presentation, sensor_type,
                                    ack).set(sensor_id == node_sensor_id ? mysensors_library_version : description));
        }

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

            // present this "node"
            presentNode();

            return true;
        }

    };
}

#endif