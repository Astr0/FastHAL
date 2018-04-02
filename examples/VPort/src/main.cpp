#include <avr/io.h>
#include "fasthal.h"
#include "mp/const_list.hpp"

using namespace fasthal;

constexpr auto testPort1 = vField(pinB4, pinB5, pinB6, pinD0, invert(pinD2), nullBit);
//constexpr auto testPort1 = vField(nullBit, nullBit, nullBit, nullBit, pinB4, pinB5, pinB6, nullBit);
constexpr auto testPort2 = portD;
//constexpr auto testPort = vField(pinB1, pinB2);



constexpr auto testPin = fieldBit<0>(portB);
//constexpr auto testPin = portB0;
constexpr auto testVPin = fieldBit<1>(testPort1);

#define actions_ex 0

template<std::size_t V>
using value_t = brigand::uint8_t<V>;

void test(){
    #if (actions_ex == 3)
    auto v = apply(
        combine_a(
            combine_a(
                write(testPort1, value_t<123>{}),
                clear(testPort2, value_t<77>{}),
                set(testPort1, value_t<1>{})),
            write(testPort2, PORTC),
            combine_a(
                clear(testPort1, value_t<54>{}),
                toggle(testPort1, value_t<0xF>{})),
            toggle(testPort2, read_i(testPort1)),
            write(testPort1, PORTB)
        ));
    // PORTC = sizeof(decltype( write(testPort2, PORTC)));
    // PORTC = sizeof(decltype(toggle(testPort2, read_i(testPort1)));
    // PORTC = sizeof(write(testPort1, PORTB));
    // PORTC = sizeof(decltype(wr));
    PORTC = get_a(testPort1, v) | get_a(testPort2, v);
    #elif (actions_ex == 2)
    apply(write(testPort1, value_t<123>{}));
    apply(clear(testPort2, value_t<77>{}));
    apply(set(testPort1, value_t<1>{}));
    apply(write(testPort2, PORTC));
    apply(clear(testPort1, value_t<54>{}));
    apply(toggle(testPort1, value_t<0xF>{}));
    apply(toggle(testPort2, read_i(testPort1)));
    apply(write(testPort1, PORTB));
    PORTC = read_i(testPort1) | read_i(testPort2);
    #elif (actions_ex == 1)
    //apply(write<123>(testPort1));
    auto v = apply(
        write(testPort1, value_t<123>{}),
        clear(testPort2, value_t<77>{}),
        set(testPort1, value_t<1>{}),
        write(testPort2, PORTC),
        clear(testPort1, value_t<54>{}),
        toggle(testPort1, value_t<0xF>{}),
        toggle(testPort2, read_i(testPort1)),
        write(testPort1, PORTB));
    PORTC = get_a(testPort1, v) | get_a(testPort2, v);
    #else
    write_i(testPort1, value_t<123>{});
    clear_i(testPort2, value_t<77>{});
    set_i(testPort1, value_t<1>{});
    write_i(testPort2, PORTC);
    clear_i(testPort1, value_t<54>{});
    toggle_i(testPort1, value_t<0xF>{});
    toggle_i(testPort2, read_i(testPort1));
    write_i(testPort1, PORTB);
    PORTC = read_i(testPort1) | read_i(testPort2);
    #endif
}

int main(){
    test();    
    //PORTC = get_a(testPort2, apply(read(testPort2)));

    // set(testPin);
    // clear(testPin);
    // toggle(testPin);
    // set(testPin, false);
    // if (read(testPin))
    // {
    //     clear_set(testPort1, 1, 2);
    // }
    // PORTD = read(testPort1);
    
    return 0;
}