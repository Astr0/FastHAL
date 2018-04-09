#ifndef FH_AVR_DUINO_H_
#define FH_AVR_DUINO_H_

#include "fasthal.hpp"
#include "std/std_types.hpp"
#include "std/std_fake.hpp"

namespace fasthal{

    namespace duino{
        #include "pins_fastduino.hpp"
    }
 
    namespace details{
        template<unsigned VNum>
        static constexpr auto get_ino_port_num(){
            static_assert(VNum < std::array_size(fasthal::duino::ino_port_num), "Pin out of range");
            return fasthal::duino::ino_port_num[VNum];
        }

        template<unsigned VNum>
        static constexpr auto get_ino_pin_num(){
            static_assert(VNum < std::array_size(fasthal::duino::ino_pin_num), "Pin out of range");
            return fasthal::duino::ino_pin_num[VNum];
        }

        static constexpr uint8_t ino_no_mux = 255;

        template<unsigned VNum>
        static constexpr auto get_ino_mux(){
            auto pin = VNum;
            #if defined(analogPinToChannel)
                #if defined(__AVR_ATmega32U4__)
                    if (pin >= 18) pin -= 18; // allow for channel or pin numbers
                #endif
                pin = analogPinToChannel(pin);
            #elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
                if (pin >= 54) pin -= 54; // allow for channel or pin numbers
            #elif defined(__AVR_ATmega32U4__)
                if (pin >= 18) pin -= 18; // allow for channel or pin numbers
            #elif defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
                if (pin >= 24) pin -= 24; // allow for channel or pin numbers
            #else
                if (pin >= 14) pin -= 14; // allow for channel or pin numbers
            #endif
            #ifdef NUM_ANALOG_INPUTS
            if (pin >= NUM_ANALOG_INPUTS)
                pin = ino_no_mux;
            #endif
            return pin;
        }

        template<unsigned VNum>
        struct ino_pin:
            // inherit avr stuff
            avr_pin<get_ino_port_num<VNum>(), get_ino_pin_num<VNum>()>
        {
        };        

        template<unsigned VNum>
        struct func_fieldbit_impl<ino_pin<VNum>>: 
            // inherit avr stuff
            func_fieldbit_impl<avr_pin<get_ino_port_num<VNum>(), get_ino_pin_num<VNum>()>>                
            , std::conditional<get_ino_mux<VNum>() != ino_no_mux
                , func_fieldbit_adc<decltype(::fasthal::avr::mux_v<get_ino_mux<VNum>()>)>
                , std::empty_t>::type
        {};
    }

    template<unsigned VNum>
    constexpr auto ino = details::ino_pin<VNum>{};
}

#endif