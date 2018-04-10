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

#if 1==1//def FH_HAS_I2C

#include "interrupts.hpp"
#include "../../std/std_types.hpp"

namespace fasthal{
    namespace details{
        struct i2c_impl{
            static constexpr auto ack = twea;
        };

        // enable, reset, ready for i2c
        template<>
        struct func_fieldbit_impl<i2c_impl>:
            func_fieldbit_enable<decltype(twen)>,
            func_fieldbit_ready_reset<decltype(twint)>
            { };

        // enable for i2c.ack
        template<>
        struct func_fieldbit_impl<std::base_type_t<decltype(twea)>>:
            func_fieldbit_enable<decltype(twea)>
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
        inline constexpr std::uint8_t calc_i2c_twbr(TFreq freq, TPs ps){
            auto psv = twi_ps_value(ps);
            
            return ((F_CPU / freq) - 16) / (2 * psv);
        }

        template<typename TFreq, TFreq VFreq, avr::tw_ps VPs>
        inline constexpr auto calc_i2c_twbr(integral_constant<TFreq, VFreq> freq, integral_constant<avr::tw_ps, VPs> ps) {
            auto result = calc_i2c_twbr(VFreq, VPs);
            return integral_constant<std::uint8_t, result>{};
        }
            
    }
    constexpr auto i2c = details::i2c_impl{};

    template<std::uint32_t V>
    constexpr auto i2c_freq_v = integral_constant<std::uint32_t, V>{};
    constexpr auto i2c_freq_def = i2c_freq_v<100000L>;

    // master begin
    template<typename TFreq = decltype(i2c_freq_def), typename TPs = decltype(avr::tw_ps::def)>
    inline constexpr auto begin(details::i2c_impl i2c, TFreq freq = i2c_freq_def, TPs ps = avr::tw_ps::def){
        auto _twbr = details::calc_i2c_twbr(freq, ps);
        return combine(
            // set ps
            write(twps, ps),
            // set twbr
            write(twbr, _twbr),
            // enable
            enable(i2c),
            // enable interrupt?
            enable(irq_i2c),
            // enable ack
            enable(i2c.ack)
        );
    }
    template<typename TFreq = decltype(i2c_freq_def), typename TPs = decltype(avr::tw_ps::def)>
    inline void begin_(i2c_impl i2c, TFreq freq = i2c_freq_def, TPs ps = avr::tw_ps::def){
        apply(begin(i2c, freq, ps));
    }

    // end
    inline constexpr auto end(i2c_impl i2c){
        return combine(
            // disable
            clear(twen),
            // disable interrupt
            clear(twie),
            // disable ack
            clear(twea)
        );
    }

}

#endif

#endif