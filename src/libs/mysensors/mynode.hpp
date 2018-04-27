#ifndef FH_LIBS_MYSENSORS_MYNODE_H_
#define FH_LIBS_MYSENSORS_MYNODE_H_

#include "core.hpp"
#include "../../std/std_types.hpp"

namespace fasthal::mysensors{
    struct mynode{
        constexpr mynode(){}

        static constexpr std::uint8_t address(){return gateway_address;}

        template<class TTransport>
        static bool present(TTransport& transport, mymessage& msg, const uint8_t sensor_id, const my_sensor sensor_type, const char *description = nullptr,
                    const bool ack = false)  {
            return transport.send(msg.build(address(), gateway_address, sensor_id, sensor_type,
                                    ack).set(sensor_id == node_sensor_id ? mysensors_library_version : description));
        }

        template<class TTransport>
        static void present(TTransport& transport, mymessage& msg){
            present(transport, msg, node_sensor_id, my_sensor::node);
        }
    };
}

#endif