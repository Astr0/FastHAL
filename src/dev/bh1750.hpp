#ifndef FH_DEV_BH1750
#define FH_DEV_BH1750

#include "../../std/std_types.hpp"

namespace fasthal::dev{
    // i2c light sensor
    class bh1750{
        i2c_address_t _address;
    public:
        enum class mode: std::uint8_t{
            // power down
            power_down = 0,
            // measurement at 1lx resolution. measurement time is approx 120ms.
            continuous_high_res_mode  = 0x10,
            // measurement at 0.5lx resolution. measurement time is approx 120ms.
            continuous_high_res_mode_2 = 0x11,
            // measurement at 4lx resolution. measurement time is approx 16ms.
            continuous_low_res_mode = 0x13,
            // measurement at 1lx resolution. measurement time is approx 120ms.
            one_time_high_res_mode = 0x20,
            // measurement at 0.5lx resolution. measurement time is approx 120ms.
            one_time_high_res_mode_2 = 0x21,
            // measurement at 1lx resolution. measurement time is approx 120ms.
            one_time_low_res_mode = 0x23            
        };

        // TODO: think how to pass i2c, drivers and other stuff
        template<class TI2c>
        constexpr bh1750(i2c_address_t address = 0x23): _address(address) {}
        
        // at least 2 bytes buffer
        bool set_mode(mode m, TI2c& i2c, buffer_t buf, typename TI2c::callback_t callback){
            buf[0] = i2c_build_sla(_address);
            buf[1] = static_cast<std::uint8_t>(m);
            return i2c.start(buf, 2, callback);
        }

        std::uint16_t set_mode_end(TI2c& i2c, buffer_t buf, i2c_result r)
        {
            if (r != i2c_result::done)
                return 0xFFFF;
            
        }
    };
}

#endif