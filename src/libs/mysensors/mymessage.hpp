#ifndef FH_LIBS_MYSENSORS_MYMESSAGE_H_
#define FH_LIBS_MYSENSORS_MYMESSAGE_H_

#include "../../std/std_types.hpp"
#include "core.hpp"
#include <string.h>

namespace fasthal::mysensors{


    /// @brief The command field (message-type) defines the overall properties of a message
    enum class my_command: std::uint8_t {
        presentation		= 0,	//!< Sent by a node when they present attached sensors. This is usually done in presentation() at startup.
        set					= 1,	//!< This message is sent from or to a sensor when a sensor value should be updated.
        req					= 2,	//!< Requests a variable value (usually from an actuator destined for controller).
        internal			= 3,	//!< Internal MySensors messages (also include common messages provided/generated by the library).
        stream				= 4		//!< For firmware and other larger chunks of data that need to be divided into pieces.
    };

    /// @brief Type of sensor (used when presenting sensors)
    enum class my_sensor: std::uint8_t{
        door				= 0,	//!< Door sensor, V_TRIPPED, V_ARMED
        motion				= 1,	//!< Motion sensor, V_TRIPPED, V_ARMED
        smoke				= 2,	//!< Smoke sensor, V_TRIPPED, V_ARMED
        binary				= 3,	//!< Binary light or relay, V_STATUS, V_WATT
        light				= 3,	//!< \deprecated Same as S_BINARY
        dimmer				= 4,	//!< Dimmable light or fan device, V_STATUS (on/off), V_PERCENTAGE (dimmer level 0-100), V_WATT
        cover				= 5,	//!< Blinds or window cover, V_UP, V_DOWN, V_STOP, V_PERCENTAGE (open/close to a percentage)
        temp				= 6,	//!< Temperature sensor, V_TEMP
        hum					= 7,	//!< Humidity sensor, V_HUM
        baro				= 8,	//!< Barometer sensor, V_PRESSURE, V_FORECAST
        wind				= 9,	//!< Wind sensor, V_WIND, V_GUST
        rain				= 10,	//!< Rain sensor, V_RAIN, V_RAINRATE
        uv					= 11,	//!< Uv sensor, V_UV
        weight				= 12,	//!< Personal scale sensor, V_WEIGHT, V_IMPEDANCE
        power				= 13,	//!< Power meter, V_WATT, V_KWH, V_VAR, V_VA, V_POWER_FACTOR
        heater				= 14,	//!< Header device, V_HVAC_SETPOINT_HEAT, V_HVAC_FLOW_STATE, V_TEMP
        distance			= 15,	//!< Distance sensor, V_DISTANCE
        light_level			= 16,	//!< Light level sensor, V_LIGHT_LEVEL (uncalibrated in percentage),  V_LEVEL (light level in lux)
        node				= 17,	//!< Used (internally) for presenting a non-repeating Arduino node
        repeater_node		= 18,	//!< Used (internally) for presenting a repeating Arduino node
        lock				= 19,	//!< Lock device, V_LOCK_STATUS
        ir					= 20,	//!< IR device, V_IR_SEND, V_IR_RECEIVE
        water				= 21,	//!< Water meter, V_FLOW, V_VOLUME
        air_quality			= 22,	//!< Air quality sensor, V_LEVEL
        custom				= 23,	//!< Custom sensor
        dust				= 24,	//!< Dust sensor, V_LEVEL
        scene_controller	= 25,	//!< Scene controller device, V_SCENE_ON, V_SCENE_OFF.
        rgb_light			= 26,	//!< RGB light. Send color component data using V_RGB. Also supports V_WATT
        rgbw_light			= 27,	//!< RGB light with an additional White component. Send data using V_RGBW. Also supports V_WATT
        color_sensor		= 28,	//!< Color sensor, send color information using V_RGB
        hvac				= 29,	//!< Thermostat/HVAC device. V_HVAC_SETPOINT_HEAT, V_HVAC_SETPOINT_COLD, V_HVAC_FLOW_STATE, V_HVAC_FLOW_MODE, V_TEMP
        multimeter			= 30,	//!< Multimeter device, V_VOLTAGE, V_CURRENT, V_IMPEDANCE
        sprinkler			= 31,	//!< Sprinkler, V_STATUS (turn on/off), V_TRIPPED (if fire detecting device)
        water_leak			= 32,	//!< Water leak sensor, V_TRIPPED, V_ARMED
        sound				= 33,	//!< Sound sensor, V_TRIPPED, V_ARMED, V_LEVEL (sound level in dB)
        vibration			= 34,	//!< Vibration sensor, V_TRIPPED, V_ARMED, V_LEVEL (vibration in Hz)
        moisture			= 35,	//!< Moisture sensor, V_TRIPPED, V_ARMED, V_LEVEL (water content or moisture in percentage?)
        info				= 36,	//!< LCD text device / Simple information device on controller, V_TEXT
        gas					= 37,	//!< Gas meter, V_FLOW, V_VOLUME
        gps					= 38,	//!< GPS Sensor, V_POSITION
        water_quality		= 39	//!< V_TEMP, V_PH, V_ORP, V_EC, V_STATUS
    };

    /// @brief Type of sensor data (for set/req/ack messages)
    enum class my_data: std::uint8_t {
        temp				= 0,	//!< S_TEMP. Temperature S_TEMP, S_HEATER, S_HVAC
        hum					= 1,	//!< S_HUM. Humidity
        status				= 2,	//!< S_BINARY, S_DIMMER, S_SPRINKLER, S_HVAC, S_HEATER. Used for setting/reporting binary (on/off) status. 1=on, 0=off
        light				= 2,	//!< \deprecated Same as V_STATUS
        percentage			= 3,	//!< S_DIMMER. Used for sending a percentage value 0-100 (%).
        dimmer				= 3,	//!< \deprecated Same as V_PERCENTAGE
        pressure			= 4,	//!< S_BARO. Atmospheric Pressure
        forecast			= 5,	//!< S_BARO. Whether forecast. string of "stable", "sunny", "cloudy", "unstable", "thunderstorm" or "unknown"
        rain				= 6,	//!< S_RAIN. Amount of rain
        rainrate			= 7,	//!< S_RAIN. Rate of rain
        wind				= 8,	//!< S_WIND. Wind speed
        gust				= 9,	//!< S_WIND. Gust
        direction			= 10,	//!< S_WIND. Wind direction 0-360 (degrees)
        uv					= 11,	//!< S_UV. UV light level
        weight				= 12,	//!< S_WEIGHT. Weight(for scales etc)
        distance			= 13,	//!< S_DISTANCE. Distance
        impedance			= 14,	//!< S_MULTIMETER, S_WEIGHT. Impedance value
        armed				= 15,	//!< S_DOOR, S_MOTION, S_SMOKE, S_SPRINKLER. Armed status of a security sensor. 1 = Armed, 0 = Bypassed
        tripped				= 16,	//!< S_DOOR, S_MOTION, S_SMOKE, S_SPRINKLER, S_WATER_LEAK, S_SOUND, S_VIBRATION, S_MOISTURE. Tripped status of a security sensor. 1 = Tripped, 0
        watt				= 17,	//!< S_POWER, S_BINARY, S_DIMMER, S_RGB_LIGHT, S_RGBW_LIGHT. Watt value for power meters
        kwh					= 18,	//!< S_POWER. Accumulated number of KWH for a power meter
        scene_on			= 19,	//!< S_SCENE_CONTROLLER. Turn on a scene
        scene_off			= 20,	//!< S_SCENE_CONTROLLER. Turn of a scene
        hvac_flow_state		= 21,	//!< S_HEATER, S_HVAC. HVAC flow state ("Off", "HeatOn", "CoolOn", or "AutoChangeOver")
        heater				= 21,	//!< \deprecated Same as V_HVAC_FLOW_STATE
        hvac_speed			= 22,	//!< S_HVAC, S_HEATER. HVAC/Heater fan speed ("Min", "Normal", "Max", "Auto")
        light_level			= 23,	//!< S_LIGHT_LEVEL. Uncalibrated light level. 0-100%. Use V_LEVEL for light level in lux
        var1				= 24,	//!< VAR1
        var2				= 25,	//!< VAR2
        var3				= 26,	//!< VAR3
        var4				= 27,	//!< VAR4
        var5				= 28,	//!< VAR5
        up					= 29,	//!< S_COVER. Window covering. Up
        down				= 30,	//!< S_COVER. Window covering. Down
        stop				= 31,	//!< S_COVER. Window covering. Stop
        ir_send				= 32,	//!< S_IR. Send out an IR-command
        ir_receive			= 33,	//!< S_IR. This message contains a received IR-command
        flow				= 34,	//!< S_WATER. Flow of water (in meter)
        volume				= 35,	//!< S_WATER. Water volume
        lock_status			= 36,	//!< S_LOCK. Set or get lock status. 1=Locked, 0=Unlocked
        level				= 37,	//!< S_DUST, S_AIR_QUALITY, S_SOUND (dB), S_VIBRATION (hz), S_LIGHT_LEVEL (lux)
        voltage				= 38,	//!< S_MULTIMETER
        current				= 39,	//!< S_MULTIMETER
        rgb					= 40,	//!< S_RGB_LIGHT, S_COLOR_SENSOR. Sent as ASCII hex: RRGGBB (RR=red, GG=green, BB=blue component)
        rgbw				= 41,	//!< S_RGBW_LIGHT. Sent as ASCII hex: RRGGBBWW (WW=white component)
        id					= 42,	//!< Used for sending in sensors hardware ids (i.e. OneWire DS1820b).
        unit_prefix			= 43,	//!< Allows sensors to send in a string representing the unit prefix to be displayed in GUI, not parsed by controller! E.g. cm, m, km, inch.
        hvac_setpoint_cool	= 44,	//!< S_HVAC. HVAC cool setpoint (Integer between 0-100)
        hvac_setpoint_heat	= 45,	//!< S_HEATER, S_HVAC. HVAC/Heater setpoint (Integer between 0-100)
        hvac_flow_mode		= 46,	//!< S_HVAC. Flow mode for HVAC ("Auto", "ContinuousOn", "PeriodicOn")
        text				= 47,	//!< S_INFO. Text message to display on LCD or controller device
        custom				= 48,	//!< Custom messages used for controller/inter node specific commands, preferably using S_CUSTOM device type.
        position			= 49,	//!< GPS position and altitude. Payload: latitude;longitude;altitude(m). E.g. "55.722526;13.017972;18"
        ir_record			= 50,	//!< Record IR codes S_IR for playback
        ph					= 51,	//!< S_WATER_QUALITY, water PH
        orp					= 52,	//!< S_WATER_QUALITY, water ORP : redox potential in mV
        ec					= 53,	//!< S_WATER_QUALITY, water electric conductivity μS/cm (microSiemens/cm)
        var					= 54,	//!< S_POWER, Reactive power: volt-ampere reactive (var)
        va					= 55,	//!< S_POWER, Apparent power: volt-ampere (VA)
        power_factor		= 56 	//!< S_POWER, Ratio of real power to apparent power: floating point value in the range [-1,..,1]
    };


    /// @brief Type of internal messages (for internal messages)
    enum class my_internal: std::uint8_t {
        battery_level			= 0,	//!< Battery level
        time					= 1,	//!< Time (request/response)
        version					= 2,	//!< Version
        id_request				= 3,	//!< ID request
        id_response				= 4,	//!< ID response
        inclusion_mode			= 5,	//!< Inclusion mode
        config					= 6,	//!< Config (request/response)
        find_parent_request		= 7,	//!< Find parent
        find_parent_response	= 8,	//!< Find parent response
        log_message				= 9,	//!< Log message
        children				= 10,	//!< Children
        sketch_name				= 11,	//!< Sketch name
        sketch_version			= 12,	//!< Sketch version
        reboot					= 13,	//!< Reboot request
        gateway_ready			= 14,	//!< Gateway ready
        signing_presentation	= 15,	//!< Provides signing related preferences (first byte is preference version)
        nonce_request			= 16,	//!< Request for a nonce
        nonce_response			= 17,	//!< Payload is nonce data
        heartbeat_request		= 18,	//!< Heartbeat request
        presentation			= 19,	//!< Presentation message
        discover_request		= 20,	//!< Discover request
        discover_response		= 21,	//!< Discover response
        heartbeat_response		= 22,	//!< Heartbeat response
        locked					= 23,	//!< Node is locked (reason in string-payload)
        ping					= 24,	//!< Ping sent to node, payload incremental hop counter
        pong					= 25,	//!< In return to ping, sent back to sender, payload incremental hop counter
        registration_request	= 26,	//!< Register request to GW
        registration_response	= 27,	//!< Register response from GW
        debug					= 28,	//!< Debug message
        signal_report_request	= 29,	//!< Device signal strength request
        signal_report_reverse	= 30,	//!< Internal
        signal_report_response	= 31,	//!< Device signal strength response (RSSI)
        pre_sleep_notification	= 32,	//!< Message sent before node is going to sleep
        post_sleep_notification	= 33	//!< Message sent after node woke up (if enabled)
    };


    /// @brief Type of data stream  (for streamed message)
    enum class my_stream: std::uint8_t {
        firmware_config_request		= 0,	//!< Request new FW, payload contains current FW details
        firmware_config_response	= 1,	//!< New FW details to initiate OTA FW update
        firmware_request			= 2,	//!< Request FW block
        firmware_response			= 3,	//!< Response FW block
        sound						= 4,	//!< Sound
        image						= 5		//!< Image
    };

    /// @brief Type of payload
    enum class my_payload: std::uint8_t {
        string				= 0,	//!< Payload type is string
        byte				= 1,	//!< Payload type is byte
        int16				= 2,	//!< Payload type is INT16
        uint16				= 3,	//!< Payload type is UINT16
        int32				= 4,	//!< Payload type is INT32
        uint32				= 5,	//!< Payload type is UINT32
        custom				= 6,	//!< Payload type is binary
        float32				= 7		//!< Payload type is float32
    };

    class mymessage{
        // command_ack_payload 
        static constexpr std::uint8_t command_mask      = 0b00000111;
        static constexpr std::uint8_t request_ack_mask  = 0b00001000;
        static constexpr std::uint8_t ack_mask          = 0b00010000;
        static constexpr std::uint8_t payload_type_mask = 0b11100000;

        // version_length
        static constexpr std::uint8_t length_mask = 0b11111000;
    public:
        static constexpr auto protocol_version = 2u;
        static constexpr auto max_message_size = 32u;
        static constexpr auto header_size = 7u;
        static constexpr auto max_payload = max_message_size - header_size;

        std::uint8_t last;            	// 8 bit - Id of last node this message passed
        std::uint8_t sender;          	// 8 bit - Id of sender node (origin)
        std::uint8_t destination;     	// 8 bit - Id of destination node

        std::uint8_t version_length;    // 2 bit - Protocol version
                                        // 1 bit - Signed flag
                                        // 5 bit - Length of payload
        uint8_t command_ack_payload;    // 3 bit - Command type
                                        // 1 bit - Request an ack - Indicator that receiver should send an ack back.
                                        // 1 bit - Is ack message - Indicator that this is the actual ack message.
                                        // 3 bit - Payload data type
        uint8_t type;            	    // 8 bit - Type varies depending on command
        uint8_t sensor;          	    // 8 bit - Id of sensor that this message concerns.

        // Each message can transfer a payload. We add one extra byte for string
        // terminator \0 to be "printable" this is not transferred OTA
        // This union is used to simplify the construction of the binary data types transferred.
        union {
            uint8_t uint8v;
            uint16_t uint16v;
            int16_t int16v;
            uint32_t uint32v;
            int32_t int32v;
            struct { // Float messages
                float floatv;
                uint8_t float_precision;   // Number of decimals when serializing
            };
            struct {  // Presentation messages
                uint8_t version; 	  // Library version
                uint8_t sensor_type;   // Sensor type hint for controller, see table above
            };
            std::uint8_t data[max_payload + 1];
        } __attribute__((packed));

        // we don't clear everything!
        mymessage(): version_length(protocol_version) {}

        // metadata
        void length(std::uint8_t len){
            version_length = (version_length & ~length_mask) | (len << 3);
        }

        std::uint8_t length() const{
            return (version_length & length_mask) >> 3;
        }

        void command(const my_command cmd){            
            command_ack_payload = (command_ack_payload & ~command_mask) | static_cast<std::uint8_t>(cmd);
        }

        my_command command() const {
            return static_cast<my_command>(command_ack_payload & command_mask);
        }

        void request_ack(bool ack){
            command_ack_payload = ack ? (command_ack_payload | request_ack_mask) : (command_ack_payload & ~request_ack_mask);
        }

        bool request_ack(){
            return command_ack_payload & request_ack_mask;
        }        

        void ack(bool ack){
            command_ack_payload = ack ? (command_ack_payload | ack_mask) : (command_ack_payload & ~ack_mask);
        }

        bool ack() const{
            return command_ack_payload & ack_mask;
        }

        void payload_type(const my_payload type){
            command_ack_payload = (command_ack_payload & ~payload_type_mask) | (static_cast<std::uint8_t>(type) << 5);
        }

        my_payload payload_type() const{
            return static_cast<my_payload>((command_ack_payload & payload_type_mask) >> 5);
        }        

        my_internal itype() const { return static_cast<my_internal>(type); }

        mymessage& build_gw(const my_internal gwtype){
            sender = gateway_address;
            destination = gateway_address;
            sensor = node_sensor_id;
            type = static_cast<std::uint8_t>(gwtype);
            command(my_command::internal);
            request_ack(false);
            ack(false);
            return *this;
        }

        mymessage& build(const std::uint8_t sender, const std::uint8_t destination, const std::uint8_t sensor, const my_command command, const std::uint8_t type, const bool ack){
            // TODO: For node this should be getNodeId()
            this->sender = sender;
            this->destination = destination;
            this->sensor = sensor;
            this->type = type;
            this->command(command);
            this->request_ack(ack);
            this->ack(false);
            return *this;        
        }

        // presentation
        mymessage& build(const std::uint8_t sender, const std::uint8_t destination, const std::uint8_t sensor, const my_sensor type, const bool ack){
            return build(sender, destination, sensor, my_command::presentation, static_cast<std::uint8_t>(type), ack);
        }

        // internal
        mymessage& build(const std::uint8_t sender, const std::uint8_t destination, const std::uint8_t sensor, const my_internal type, const bool ack){
            return build(sender, destination, sensor, my_command::internal, static_cast<std::uint8_t>(type), ack);
        }

        // ************************************** set
        mymessage& set(const char* v){
            std::uint8_t len;
            if (v != nullptr){
                len = strlen(v);
                if (len > max_payload)
                    len = max_payload;
                strncpy(reinterpret_cast<char*>(data), v, len);
            } else {
                len = 0;
            }
            data[len] = 0;

            length(len);
            payload_type(my_payload::string);

            return *this;
        }

        mymessage& set(const std::uint8_t v){
            length(1);
            payload_type(my_payload::byte);
            data[0] = v;
            return *this;
        }

        mymessage& set(const bool v){
            return set(static_cast<const std::uint8_t>(v));
        }

        // *************************************** in/out string
        template<class TStream>
        void print_value_to(TStream& stream) const{
            switch (payload_type()){
                case my_payload::string:
                    write(stream, data, length());
                    //write(stream, std::uint8_t{0});
                    break;
                case my_payload::byte:
                    print(stream, uint8v);
                    break;
                case my_payload::int16:
                    print(stream, int16v);
                    break;
                case my_payload::uint16:
                    print(stream, uint16v);
                    break;
                case my_payload::int32:
                    print(stream, int32v);
                    break;
                case my_payload::uint32:
                    print(stream, uint32v);
                    break;
                case my_payload::float32:
                    print(stream, floatv, float_precision);
                    break;
                case my_payload::custom:
                    print_hex(stream, data, length());
                    //write(stream, std::uint8_t{0});
                    break;
            }
        }

        template<class TStream>
        void parse_string_from(TStream& stream){
            payload_type(my_payload::string);
            length(parse_string(stream, reinterpret_cast<char*>(data)));
        }

        template<class TStream>
        void parse_custom_from(TStream& stream){
            payload_type(my_payload::custom);
            length(parse_hex(stream, data));
        }
    };
}

#endif