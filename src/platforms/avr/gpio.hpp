#ifndef FH_AVR_GPIO_H_
#define FH_AVR_GPIO_H_

#include "../../fields/gpio.hpp"
#include "../../fields/fieldbit.hpp"

namespace fasthal{
    namespace details{
        template<class Tpin, class Tport, class Tddr> 
        struct avr_pin: 
            gpio_input_bit<Tpin>,
            gpio_output_bit<Tport>
        {
            using ddr_t = Tddr;
        };

        template<class Tpin, class Tport, class Tddr> 
        struct func_fieldbit_impl<avr_pin<Tpin, Tport, Tddr>>: avr_pin<Tpin, Tport, Tddr>{};
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