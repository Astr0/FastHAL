#include <avr/io.h>
#include "fasthal.h"
#include "mp/const_list.hpp"

using namespace fasthal;

constexpr auto testPort1 = vField(pinB4, pinB5, pinB6, pinD0, invert(pinD2), nullBit);
constexpr auto testPort2 = portD;
//constexpr auto testPort = vField(pinB1, pinB2);



constexpr auto testPin = fieldBit<0>(portB);
//constexpr auto testPin = portB0;
constexpr auto testVPin = fieldBit<1>(testPort1);

#define actions_ex 3

void test(){
    #if (actions_ex == 3)
    auto v = apply(combine_a(
            combine_a(
                write_a<123>(testPort1),
                clear_a<77>(testPort2),
                read_a(testPort2),
                set_a<1>(testPort1)),
            write_a(testPort2, PORTC),
            combine_a(
                clear_a<54>(testPort1),
                toggle_a<0xF>(testPort1)),
            toggle_a(testPort2, read(testPort1)),
            write_a(testPort1, PORTB),
            read_a(testPort1)));
    PORTC = get_a(testPort1, v) | get_a(testPort2, v);
    #elif (actions_ex == 2)
    apply(write_a(testPort1, 123));
    apply(clear_a(testPort2, 77));
    apply(set_a(testPort1, 1));
    apply(write_a(testPort2, PORTC));
    apply(clear_a(testPort1, 54));
    apply(toggle_a(testPort1, 0xF));
    apply(toggle_a(testPort2, read_i(testPort1)));
    apply(write_a(testPort1, PORTB));
    PORTC = read_i(testPort1) | read_i(testPort2);
    #elif (actions_ex == 1)
    //apply(write_a<123>(testPort1));
    auto v = apply(            
            combine_a(
                write_a<123>(testPort1),
                clear_a<77>(testPort2),
                read_a(testPort2),
                set_a<1>(testPort1)),
            write_a(testPort2, PORTC),
            combine_a(
                clear_a<54>(testPort1),
                toggle_a<0xF>(testPort1)),
            toggle_a(testPort2, read(testPort1)),
            write_a(testPort1, PORTB),
            read_a(testPort1)
    );
    PORTC = get_a(testPort1, v) | get_a(testPort2, v);
    #else
    write(testPort1, 123);
    clear(testPort2, 77);
    set(testPort1, 1);
    write(testPort2, PORTC);
    clear(testPort1, 54);
    toggle(testPort1, 0xF);
    toggle(testPort2, read(testPort1));
    write(testPort1, PORTB);
    PORTC = read(testPort1) | read(testPort2);
    #endif
}

int main(){
    test();    
    //PORTC = get_a(testPort2, apply(read_a(testPort2)));

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