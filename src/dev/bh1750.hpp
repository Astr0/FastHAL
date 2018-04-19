#ifndef FH_DEV_BH1750
#define FH_DEV_BH1750

#include "../../std/std_types.hpp"
#include "../../mp/holder.hpp"

namespace fasthal::dev{
    // i2c light sensor
    template<typename TI2cPtr, typename TAddress = decltype(i2c_address_v<0x23>)>
    class bh1750: 
        mp::holder<TI2cPtr>,
        mp::holder<TAddress>
    {        
        auto& i2c() { return *(this->mp::holder<TI2cPtr>.get());}
        TAddress address() { return this->mp::holder<TAddress>.get();}
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
        constexpr bh1750(TI2cPtr i2c, TAddress address = i2c_address_v<0x23>): 
            mp::holder<TI2cPtr>(i2c),
            mp::holder<TI2cPtr>(address) {}
        
        // at least 2 bytes buffer
        template<class TArgs>
        bool set_mode(mode m, TArgs& args){
            args[0] = i2c_build_sla(address());
            args[1] = static_cast<std::uint8_t>(m);
            if (!i2c().start(args))
                args(&i2c());
        }

        std::uint16_t set_mode_end(TI2c& i2c, buffer_t buf, i2c_result r)
        {
            if (r != i2c_result::done)
                return 0xFFFF;
            
        }
    };
}

#endif