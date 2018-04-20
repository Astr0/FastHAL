#ifndef FH_HAL_TYPES_H_
#define FH_HAL_TYPES_H_

#include "../std/std_types.hpp"
#include "../utils/types.hpp"
#include "net.hpp"

namespace fasthal{
    enum class i2c_start: std::uint8_t{
        start = 0,
        stop_start = 1,
        restart = 2
    };

    enum class i2c_result: std::uint8_t{
        done = 0,
        nack = 1,
        error= 2        
    };

    using i2c_address_t = std::uint8_t;
    template<i2c_address_t V>
    constexpr auto i2c_address_v = integral_constant<i2c_address_t, V>{};

    static constexpr auto i2c_mt = false;
    static constexpr auto i2c_mr = true;

    inline constexpr auto i2c_build_sla(bool read, i2c_address_t address){
        address <<= 1;
        if (read)
            address |= 1;
        return address;
    }

    // i2c interface
    template<class TArgs>
    struct i2c_null{        
        static void start(TArgs& args, i2c_start type = i2c_start::start) { args.state(i2c_result::error); args(); }
        static void more(TArgs& args) { args.state(i2c_result::error); args(); }
        static void stop(){}
    };
}

#endif