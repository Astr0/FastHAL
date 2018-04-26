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
            return transport().send_route(msg);
        }

        void presentNode() const{
            present(node_sensor_id, my_sensor::node);
            // TODO: call presentation
        }

        bool present(const uint8_t sensor_id, const my_sensor sensor_type, const char *description = nullptr,
                    const bool ack = false) const {
            auto msg = mymessage{};

            return send_route(msg.build(gateway_address, gateway_address, sensor_id, sensor_type,
                                    ack).set(sensor_id == node_sensor_id ? mysensors_library_version : description));
        }

        bool send_heartbeat(mymessage& tmp, const bool ack) const{
            // TODO
    	    // const uint32_t heartbeat = transportGetHeartbeat();
            // auto msg = mymessage{};
	        // return send_route(msg.build(gateway_address, gateway_address, node_sensor_id, my_internal::heartbeat_response, ack)
            //     .set(heartbeat));
            return true;
        }

        bool process_internal_from_gateway(mymessage& msg) const{
            switch (msg.itype()){
                case my_internal::reboot:
                    // TODO
                    // hwReboot();
                    break;
                case my_internal::registration_response:
                    // TODO
                    // _coreConfig.nodeRegistered = _msg.getBool();
                    break;
                case my_internal::config:
                    // TODO: maybe we don't need it at all
                    break;
                case my_internal::presentation:
                    // Re-send node presentation to controller
                    // we can reuse msg here
                    presentNode();
                    break;
                case my_internal::heartbeat_request:
                    send_heartbeat(msg, false);
                    break;
                case my_internal::time:
                    // TODO
                    // // Deliver time to callback
                    // if (receiveTime) {
                    //     receiveTime(_msg.getULong());
                    // }
                    break;
                case my_internal::children:
                    // TODO
                    // #if defined(MY_REPEATER_FEATURE) && defined(MY_SENSOR_NETWORK)
                    // if (_msg.data[0] == 'C') {
                    //     // Clears child relay data for this node
                    //     setIndication(INDICATION_CLEAR_ROUTING);
                    //     transportClearRoutingTable();
                    //     (void)_sendRoute(build(_msgTmp, GATEWAY_ADDRESS, NODE_SENSOR_ID, C_INTERNAL, I_CHILDREN).set("OK"));
                    // }
                    // #endif
                    break;
                case my_internal::debug:
                    // TODO?
                    // #if defined(MY_SPECIAL_DEBUG)
                    //             const char debug_msg = _msg.data[0];
                    //             if (debug_msg == 'R') {		// routing table
                    // #if defined(MY_REPEATER_FEATURE) && defined(MY_SENSOR_NETWORK)
                    //                 transportReportRoutingTable();
                    // #endif
                    //             } else if (debug_msg == 'V') {	// CPU voltage
                    //                 (void)_sendRoute(build(_msgTmp, GATEWAY_ADDRESS, NODE_SENSOR_ID, C_INTERNAL,
                    //                                     I_DEBUG).set(hwCPUVoltage()));
                    //             } else if (debug_msg == 'F') {	// CPU frequency in 1/10Mhz
                    //                 (void)_sendRoute(build(_msgTmp, GATEWAY_ADDRESS, NODE_SENSOR_ID, C_INTERNAL,
                    //                                     I_DEBUG).set(hwCPUFrequency()));
                    //             } else if (debug_msg == 'M') {	// free memory
                    //                 (void)_sendRoute(build(_msgTmp, GATEWAY_ADDRESS, NODE_SENSOR_ID, C_INTERNAL,
                    //                                     I_DEBUG).set(hwFreeMem()));
                    //             } else if (debug_msg == 'E') {	// clear MySensors eeprom area and reboot
                    //                 (void)_sendRoute(build(_msgTmp, GATEWAY_ADDRESS, NODE_SENSOR_ID, C_INTERNAL, I_DEBUG).set("OK"));
                    //                 for (uint16_t i = EEPROM_START; i<EEPROM_LOCAL_CONFIG_ADDRESS; i++) {
                    //                     hwWriteConfig(i, 0xFF);
                    //                 }
                    //                 setIndication(INDICATION_REBOOT);
                    //                 hwReboot();
                    //             }
                    // #endif
                    break;
                default:
                    return false;
            }
            return true;
        }

        bool process_internal_from_node(mymessage& msg) const {
            if (msg.itype() == my_internal::registration_request){
                
                if constexpr (F_CPU > 16000000){
                    // delay for fast GW and slow nodes
                    delay_ms(5);
                }
                // always approve (TODO: controller/version check/etc)
                send_route(msg.build(gateway_address, msg.sender, node_sensor_id, my_internal::registration_response, false).set(true));

                return true;
            }
            return false;
        }

        // Message delivered through _msg
        bool process_internal(mymessage& msg) const {
            if (msg.sender == gateway_address) 
                return process_internal_from_gateway(msg);
            return process_internal_from_node(msg);        
        }

        void process_gtransport() const {
            mymessage msg;
            if (!gtransport().try_receive(msg))
                return;
            if (msg.destination == gateway_address) {
                // Check if sender requests an ack back.
                if (msg.request_ack()) {
                    // Reply without ack flag (otherwise we would end up in an eternal loop)
                    msg.request_ack(false);
                    // it's ack
                    msg.ack(true);
                    // reverse destination
                    msg.destination = msg.sender;
                    // sender is us
                    msg.sender = gateway_address;
                    gtransport().send(msg);
                }
                if (msg.command() == my_command::internal) {
                    if (msg.itype() == my_internal::version) {
                        // Request for version. Create the response
                        gtransport().send(msg.build_gw(my_internal::version).set(mysensors_library_version));
                    } else {
                        process_internal(msg);
                    }
                } else {
                    // Call incoming message callback if available
                    // TODO
                    // if (receive) {
                    //     receive(_msg);
                    // }
                }
            } else {
                transport().send_route(msg);
            }
        }

        void process_transport() const {

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

        void update() const{
            process_gtransport();

        }
    };
}

#endif