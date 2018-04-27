#ifndef FH_LIBS_MY_SENSORS_MYCONTEXT_H_
#define FH_LIBS_MY_SENSORS_MYCONTEXT_H_

#include "../../std/std_types.hpp"
#include "core.hpp"

namespace fasthal::mysensors{
    struct mycontext{
        static constexpr std::uint8_t address(){return gateway_address;}
    };
}

#endif