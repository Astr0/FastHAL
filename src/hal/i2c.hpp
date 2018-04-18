#ifndef FH_HAL_TYPES_H_
#define FH_HAL_TYPES_H_

#include "../std/std_types.hpp"
#include "../utils/types.hpp"

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
    constexpr auto i2c_address_v = V;

    static constexpr auto i2c_mt = false;
    static constexpr auto i2c_mr = true;

    inline constexpr auto i2c_build_sla(bool read, i2c_address_t address){
        address <<= 1;
        if (read)
            address |= 1;
        return address;
    }

    // i2c interface
    struct i2c_null{
        using callback_t = void(*)(i2c_result);
        static bool start(
            std::uint8_t* buf,
            bsize_t count,
            callback_t callback,
            i2c_start type = i2c_start::start
            ){ return false; }
        static void more(
            std::uint8_t* buf,
            bsize_t count,
            callback_t callback){}
        static void stop(){}
    };
}

#endif