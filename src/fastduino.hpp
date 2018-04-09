#ifndef FH_AVR_DUINO_H_
#define FH_AVR_DUINO_H_

namespace fasthal::duino{
    #include "pins_fastduino.hpp"

    namespace details{

    }

    template<unsigned VNum>
    constexpr auto ino = ::fasthal::details::avr_pin<ino_port_num[VNum], ino_pin_num[VNum]>{};

}

#endif