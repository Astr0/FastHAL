#ifndef FH_LIBS_MYSENSORS_GATEWAY_H_
#define FH_LIBS_MYSENSORS_GATEWAY_H_

#include "../../mp/holder.hpp"
#include "mymessage.hpp"
#include "core.hpp"
#include "mytransport.hpp"

namespace fasthal::mysensors{
    
    template<typename TTransportPtr, typename TGTransportPtr>
    class mygateway
        : mp::holder<TTransportPtr>
        , mp::holder<TGTransportPtr>{
        auto& transport() const { return *(this->mp::holder<TTransportPtr>::get()); }
        auto& gtransport() const { return *(this->mp::holder<TGTransportPtr>::get()); }

        // void presentNode(mymessage& msg) const{
        //     present(msg, node_sensor_id, my_sensor::node);
        //     // TODO: call presentation
        // }

        // bool present(mymessage& msg, const uint8_t sensor_id, const my_sensor sensor_type, const char *description = nullptr,
        //             const bool ack = false) const {
        //     return send_route(msg.build(gateway_address, gateway_address, sensor_id, sensor_type,
        //                             ack).set(sensor_id == node_sensor_id ? mysensors_library_version : description));
        // }

        // bool send_heartbeat(mymessage& tmp, const bool ack) const{
        //     // TODO
    	//     // const uint32_t heartbeat = transportGetHeartbeat();
        //     // auto msg = mymessage{};
	    //     // return send_route(msg.build(gateway_address, gateway_address, node_sensor_id, my_internal::heartbeat_response, ack)
        //     //     .set(heartbeat));
        //     return true;
        // }

        // bool process_internal_from_gateway(mymessage& msg) const{
        //     switch (msg.itype()){
        //         case my_internal::version:
        //             msg.build_gw(my_internal::version).set(mysensors_library_version);
        //         case my_internal::reboot:
        //             // TODO
        //             // hwReboot();
        //             break;
        //         case my_internal::registration_response:
        //             // TODO
        //             // _coreConfig.nodeRegistered = _msg.getBool();
        //             break;
        //         case my_internal::config:
        //             // TODO: maybe we don't need it at all
        //             break;
        //         case my_internal::presentation:
        //             // Re-send node presentation to controller
        //             presentNode(msg);
        //             break;
        //         case my_internal::heartbeat_request:
        //             send_heartbeat(msg, false);
        //             break;
        //         case my_internal::time:
        //             // TODO
        //             // // Deliver time to callback
        //             // if (receiveTime) {
        //             //     receiveTime(_msg.getULong());
        //             // }
        //             break;
        //         case my_internal::children:
        //             // TODO
        //             // #if defined(MY_REPEATER_FEATURE) && defined(MY_SENSOR_NETWORK)
        //             // if (_msg.data[0] == 'C') {
        //             //     // Clears child relay data for this node
        //             //     setIndication(INDICATION_CLEAR_ROUTING);
        //             //     transportClearRoutingTable();
        //             //     (void)_sendRoute(build(_msgTmp, GATEWAY_ADDRESS, NODE_SENSOR_ID, C_INTERNAL, I_CHILDREN).set("OK"));
        //             // }
        //             // #endif
        //             break;
        //         case my_internal::debug:
        //             // TODO?
        //             // #if defined(MY_SPECIAL_DEBUG)
        //             //             const char debug_msg = _msg.data[0];
        //             //             if (debug_msg == 'R') {		// routing table
        //             // #if defined(MY_REPEATER_FEATURE) && defined(MY_SENSOR_NETWORK)
        //             //                 transportReportRoutingTable();
        //             // #endif
        //             //             } else if (debug_msg == 'V') {	// CPU voltage
        //             //                 (void)_sendRoute(build(_msgTmp, GATEWAY_ADDRESS, NODE_SENSOR_ID, C_INTERNAL,
        //             //                                     I_DEBUG).set(hwCPUVoltage()));
        //             //             } else if (debug_msg == 'F') {	// CPU frequency in 1/10Mhz
        //             //                 (void)_sendRoute(build(_msgTmp, GATEWAY_ADDRESS, NODE_SENSOR_ID, C_INTERNAL,
        //             //                                     I_DEBUG).set(hwCPUFrequency()));
        //             //             } else if (debug_msg == 'M') {	// free memory
        //             //                 (void)_sendRoute(build(_msgTmp, GATEWAY_ADDRESS, NODE_SENSOR_ID, C_INTERNAL,
        //             //                                     I_DEBUG).set(hwFreeMem()));
        //             //             } else if (debug_msg == 'E') {	// clear MySensors eeprom area and reboot
        //             //                 (void)_sendRoute(build(_msgTmp, GATEWAY_ADDRESS, NODE_SENSOR_ID, C_INTERNAL, I_DEBUG).set("OK"));
        //             //                 for (uint16_t i = EEPROM_START; i<EEPROM_LOCAL_CONFIG_ADDRESS; i++) {
        //             //                     hwWriteConfig(i, 0xFF);
        //             //                 }
        //             //                 setIndication(INDICATION_REBOOT);
        //             //                 hwReboot();
        //             //             }
        //             // #endif
        //             break;
        //         default:
        //             return false;
        //     }
        //     return true;
        // }

        // bool process_internal_from_node(mymessage& msg) const {
        //     if (msg.itype() == my_internal::registration_request){
                
        //         if constexpr (F_CPU > 16000000){
        //             // delay for fast GW and slow nodes
        //             delay_ms(5);
        //         }
        //         // always approve (TODO: controller/version check/etc)
        //         send_route(msg.build(gateway_address, msg.sender, node_sensor_id, my_internal::registration_response, false).set(true));

        //         return true;
        //     }
        //     return false;
        // }

        // bool process_internal(mymessage& msg) const {
        //     if (msg.sender == gateway_address) 
        //         return process_internal_from_gateway(msg);
        //     return process_internal_from_node(msg);        
        // }

        bool update_gtransport(mymessage& msg) const {
            if (!gtransport().update(msg))
                return false;

            // we will get all messages from here, broadcasts too? shouldn't we respond to broadcasts?
            if (msg.destination != gateway_address) {
                // not our message, pass it to the network
                transport().send(msg);
                return false;
            }

            // we got message!
            return true;
        }

        bool update_transport(mymessage& msg) const {
            if (!transport().update(msg))
                return false;
            // transport also should handle ACK for us, right?
            // we should get only our messages and broadcasts from transport
            // forward them to gtransport            
            gtransport().send(msg);

            // and we got message!
            return true;
        }

    public:
        constexpr mygateway(TTransportPtr transport, TGTransportPtr gtransport):
            mp::holder<TTransportPtr>(transport),
            mp::holder<TGTransportPtr>(gtransport) {}

        // init
        bool begin() const {
            // init network and gateway transport           
            return transport().begin() 
                && gtransport().begin();
            // TODO: present this node after init
        }

        // update
        bool update(mymessage& msg) const {
            // got message from gateway transport or net transport
            return update_gtransport(msg) 
                || update_transport(msg);
        }

        // send message
        bool send(mymessage& msg) const {
            if (msg.destination == gateway_address) {
        		// This is a message sent from a sensor attached on the gateway node.
        		// Pass it directly to the gateway transport layer.
		        return gtransport().send(msg);
	        }
            // Send through normal transport
            return transport().send(msg);
        }
    };
}

#endif