#ifndef FH_LIBS_MYSENSORS_CORE_H_
#define FH_LIBS_MYSENSORS_CORE_H_

#include "../../std/std_types.hpp"

namespace fasthal::mysensors{
    static constexpr std::uint8_t gateway_address = 0;
    static constexpr std::uint8_t node_sensor_id = 255;
    static constexpr auto mysensors_library_version = "2.2.0";
}

// this my sensors implementation is purely network oriented, but highly customizable
// based on chain-of-responsibility / piping principle
// basic "pipe" interface: bool begin(), bool update(msg) - receive message, bool send(msg) - to send 
// update may possibly process message internally and return true/false depending on processing

#endif