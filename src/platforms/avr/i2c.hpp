#ifndef FH_AVR_I2C_H_
#define FH_AVR_I2C_H_

// I2C should have commands
// begin as master
// master - write to slave
// master - read from slave
// slave - read write from master
// slave - respond on read to master
// possible re-starts
// implement as streaming protocol?

#include "registers.hpp"

#if 1==1//def FH_HAS_I2C0

#include "interrupts.hpp"
#include "../../std/std_types.hpp"

namespace fasthal{
    namespace details{
        template<unsigned VNum>
        struct i2c_impl{
            static constexpr bool available = false;
        };

        // enable, reset, ready for i2c
        template<unsigned VNum>
        struct func_fieldbit_impl<i2c_impl<VNum>>:
            func_fieldbit_enable<decltype(i2c_impl<VNum>::en)>,
            func_fieldbit_ready_reset<decltype(i2c_impl<VNum>::ready)>
            { };

        inline constexpr std::uint8_t twi_ps_value(avr::tw_ps ps){
            using ps_t = avr::tw_ps;
            switch (ps){
                case ps_t::_1 : return 1;
                case ps_t::_4 : return 4;
                case ps_t::_16 : return 16;
                case ps_t::_64 : return 64;
                default: return 0;
            }
        }

        template<typename TFreq, typename TPs>
        inline constexpr std::uint8_t i2c_calc_twbr(TFreq freq, TPs ps){
            auto psv = twi_ps_value(ps);
            
            return ((F_CPU / freq) - 16) / (2 * psv);
        }

        template<typename TFreq, TFreq VFreq, avr::tw_ps VPs>
        inline constexpr auto i2c_calc_twbr(integral_constant<TFreq, VFreq> freq, integral_constant<avr::tw_ps, VPs> ps) {
            auto result = i2c_calc_twbr(VFreq, VPs);
            return integral_constant<std::uint8_t, result>{};
        }

        // general case
        template<typename TRead, typename TAddress>
        inline constexpr auto i2c_build_sla(TRead read, TAddress address){
            auto result = static_cast<std::uint8_t>(address) << 1;
            if (read)
                result |= 1;
            return result;
        }
        // optimized for known mode
        template<bool VRead, typename TAddress>
        inline constexpr auto i2c_build_sla(integral_constant<bool, VRead> read, TAddress address){
            auto result = static_cast<std::uint8_t>(address) << 1;
            if constexpr (VRead)
                result |= 1;
            return result;            
        }   
        // optimized for known mode and address
        template<bool VRead, typename TAddress, TAddress VAddress>
        inline constexpr auto i2c_build_sla(integral_constant<bool, VRead> read, integral_constant<TAddress, VAddress> address){
            constexpr auto result = i2c_build_sla(VRead, VAddress);
            return integral_constant<decltype(result), result>{};
        }   
    }

    using i2c_address_t = std::uint8_t;
    template<i2c_address_t V>
    constexpr auto i2c_address_v = integral_constant<i2c_address_t, V>{};

    template<std::uint32_t V>
    constexpr auto i2c_freq_v = integral_constant<std::uint32_t, V>{};
    constexpr auto i2c_freq_def = i2c_freq_v<100000L>;

    constexpr auto i2c_write = integral_constant<bool, false>{};
    constexpr auto i2c_read = integral_constant<bool, true>{};

    // master begin
    template<unsigned VNum, typename TFreq = decltype(i2c_freq_def), typename TPs = decltype(avr::tw_ps::def)>
    inline constexpr auto begin(details::i2c_impl<VNum> i2c, TFreq freq = i2c_freq_def, TPs ps = avr::tw_ps::def){
        auto _twbr = details::i2c_calc_twbr(freq, ps);
        return combine(
            // set ps
            write(i2c.ps, ps),
            // set twbr
            write(i2c.rate, _twbr),
            // enable
            enable(i2c),
            // enable interrupt?
            enable(i2c.irq),
            // enable ack
            enable(i2c.ack)
        );
    }
    template<unsigned VNum, typename TFreq = decltype(i2c_freq_def), typename TPs = decltype(avr::tw_ps::def)>
    inline void begin_(details::i2c_impl<VNum> i2c, TFreq freq = i2c_freq_def, TPs ps = avr::tw_ps::def){
        apply(begin(i2c, freq, ps));
    }

    // end i2c
    template<unsigned VNum>
    inline constexpr auto end(details::i2c_impl<VNum> i2c){
        return combine(
            // disable
            disable(i2c),
            // disable interrupt
            disable(i2c.irq),
            // disable ack
            disable(i2c.ack)
        );
    }
    template<unsigned VNum>
    inline void end_(details::i2c_impl<VNum> i2c) { 
        apply(end(i2c)); 
    }

    // master start 
    template<unsigned VNum>
    void start(details::i2c_impl<VNum> i2c){

    }

    // master stop 
    template<unsigned VNum>
    void stop(details::i2c_impl<VNum> i2c){

    }

    // master select device and mode
    template<unsigned VNum, typename TRead, typename TAddress>
    void select(details::i2c_impl<VNum> i2c, TRead read, TAddress address){
        auto sla = details::i2c_build_sla(read, address);

    }

    // write
    template<unsigned VNum>
    void write(details::i2c_impl<VNum> i2c, std::uint8_t v){

    }

    // read
    template<unsigned VNum>
    std::uint8_t read(details::i2c_impl<VNum> i2c){
        return 0;
    }

    #ifdef FH_HAS_I2C0

    namespace details{
        template<>
        struct i2c_impl<0>{
            static constexpr bool available = true;
            static constexpr auto en = ::fasthal::avr::twen0;
            static constexpr auto ready = ::fasthal::avr::twint0;
            static constexpr auto ps = ::fasthal::avr::twps0;
            static constexpr auto rate = ::fasthal::avr::twbr0;
            static constexpr auto ack = ::fasthal::avr::twea0;
            static constexpr auto irq = irq_i2c0;
        };

        // enable for i2c.ack
        template<>
        struct func_fieldbit_impl<std::base_type_t<decltype(::fasthal::avr::twea0)>>:
            func_fieldbit_enable<decltype(::fasthal::avr::twea0)>
            { };
    }
    constexpr auto i2c0 = details::i2c_impl<0>{};
    
    #endif
}

#endif

#endif