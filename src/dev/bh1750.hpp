#ifndef FH_DEV_BH1750
#define FH_DEV_BH1750

#include "../std/std_types.hpp"
#include "../mp/holder.hpp"
#include "../hal/i2c.hpp"

namespace fasthal::dev{
    // i2c light sensor
    enum class bh1750_mode: std::uint8_t{
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

    template<typename TI2cPtr, typename TAddress = decltype(i2c_address_v<0x23>)>
    class bh1750: 
        mp::holder<TI2cPtr>,
        mp::holder<TAddress>
    {        
        auto& i2c() { return *(this->mp::holder<TI2cPtr>::get());}
        TAddress address() { return this->mp::holder<TAddress>::get();}
    public:
        // TODO: think how to pass i2c, drivers and other stuff
        constexpr bh1750(TI2cPtr i2c, TAddress address = i2c_address_v<0x23>): 
            mp::holder<TI2cPtr>(i2c),
            mp::holder<TAddress>(address) {}
        
        // at least 2 bytes buffer
        template<class TArgs>
        void set_mode(bh1750_mode m, TArgs& args){
            args[0] = i2c_build_sla(i2c_mt, address());
            args[1] = static_cast<std::uint8_t>(m);
            i2c().start(args); 
            // if (!i2c().start(args))
            //     args();
        }

        template<class TArgs>
        bool set_mode_end(TArgs& args)
        {
            i2c().stop();
            return args.status_any(i2c_result::done);    
        }

        template<class TArgs>
        void read(TArgs& args){
            args[0] = i2c_build_sla(i2c_mr, address());
            // address doesn't count as received byte and gets overwritten
            args.count(2);          
            i2c().start(args); 
            // if (!i2c().start(args))
            //     args();
        }

        template<class TArgs>
        bool read_end(TArgs& args, std::uint16_t& value){
            i2c().stop();

            if (!args.status_any(i2c_result::done))
                return false;

            auto result = static_cast<uint16_t>(args[0]);
            result <<= 8;
            result |= args[1];

            value = (result * 10U) / 12U;

            return true;
        }
    };
}

#endif