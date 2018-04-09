#ifndef FH_AVR_DUINO_H_
#define FH_AVR_DUINO_H_

#include "fasthal.hpp"
#include "std/std_types.hpp"
#include "std/std_fake.hpp"

namespace fasthal{

    namespace duino{
        #include "pins_fastduino.hpp"
 
        namespace details{
            template<unsigned VNum>
            static constexpr auto get_ino_port_num(){
                static_assert(VNum < std::array_size(ino_port_num), "Pin out of range");
                return ino_port_num[VNum];
            }

            template<unsigned VNum>
            static constexpr auto get_ino_pin_num(){
                static_assert(VNum < std::array_size(ino_pin_num), "Pin out of range");
                return ino_pin_num[VNum];
            }

            template<unsigned VNum>
            struct ino_pin:
                // inherit avr stuff
                ::fasthal::details::avr_pin<get_ino_port_num<VNum>(), get_ino_pin_num<VNum>()>
            {
            };
        }

        template<unsigned VNum>
        constexpr auto ino = ::fasthal::details::avr_pin<ino_port_num[VNum], ino_pin_num[VNum]>{};
    }

    namespace details{
        template<unsigned VNum>
        struct func_fieldbit_impl<::fasthal::duino::details::ino_pin<VNum>>: 
            // inherit avr stuff
            func_fieldbit_impl<avr_pin<::fasthal::duino::ino_port_num[VNum], ::fasthal::duino::ino_pin_num[VNum]>>
        {};
    }
}

namespace fasthal::duino{

    


}

#endif