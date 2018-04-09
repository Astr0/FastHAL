#ifndef FH_AVR_GPIO_H_
#define FH_AVR_GPIO_H_

#include "../../fields/gpio.hpp"
#include "../../fields/fieldbit.hpp"

namespace fasthal{
    namespace details{
        template<unsigned VReg, unsigned VNumber> 
        struct avr_pin: 
            gpio_input_bit<decltype(fieldBit<VNumber>(::fasthal::avr::gpio_registers<VReg>::pin))>,
            gpio_output_bit<decltype(fieldBit<VNumber>(::fasthal::avr::gpio_registers<VReg>::port))>
        {
            using ddr_t = decltype(fieldBit<VNumber>(::fasthal::avr::gpio_registers<VReg>::ddr));
        };

        template<unsigned VReg, unsigned VNumber> 
        struct func_fieldbit_impl<avr_pin<VReg, VNumber>>: avr_pin<VReg, VNumber>{};
    }

    template<class T, 
        class ddr_t = typename T::ddr_t>
    constexpr auto inline makeOutput(T pin){
        return set(ddr_t{});
    }

    template<class T, 
        class ddr_t = typename T::ddr_t>
    inline void makeOutput_(T pin){
        return set_(ddr_t{});
    }
    
    template<class T, 
        class ddr_t = typename T::ddr_t,
        class gpout_t = typename T::gpout_t>
    constexpr auto inline makeInput(T pin){
        return combine(clear(ddr_t{}), clear(gpout_t{}));
    }

    template<class T, 
        class ddr_t = typename T::ddr_t,
        class gpout_t = typename T::gpout_t>
    inline void makeInput_(T pin){
        clear_(ddr_t{});
        clear_(gpout_t{});
    }

    template<class T, 
        class ddr_t = typename T::ddr_t,
        class gpout_t = typename T::gpout_t>
    constexpr auto inline makeInputPullup(T pin){
        return combine(clear(ddr_t{}), set(gpout_t{}));
    }

    template<class T, 
        class ddr_t = typename T::ddr_t,
        class gpout_t = typename T::gpout_t>
    inline void makeInputPullup_(T pin){
       clear_(ddr_t{});
       set_(gpout_t{});
    }
    
    #include "gpio_impl.hpp"
}

#endif