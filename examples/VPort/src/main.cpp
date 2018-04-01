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

#define actions_ex 1

void test(){
    #if (actions_ex == 3)
    write_a(testPort1, 123)();
    clear_a(testPort2, 77)();
    set_a(testPort1, 1)();
    write_a(testPort2, PORTC)();
    clear_a(testPort1, 54)();
    toggle_a(testPort1, 0xF)();
    toggle_a(testPort2, read_a(testPort1)())();
    write_a(testPort1, PORTB)();
    PORTC = read_a(testPort1)() | read_a(testPort2)();
    #elif (actions_ex == 2)
    auto v = read(testPort1);
    auto v2 = read(testPort2);
    v = write_a(testPort1, 123).execute(v);
    v2 = clear_a<77>(testPort2).execute(v2);
    v = set_a(testPort1, 1).execute(v);
    v2 = write_a(testPort2, PORTC).execute(v2);
    v = clear_a(testPort1, 54).execute(v);
    v = toggle_a(testPort1, 0xF).execute(v);
    v2 = toggle_a(testPort2, read(testPort1)).execute(v2);
    v = write_a(testPort1, PORTB).execute(v);
    write(testPort1, v);
    write(testPort2, v2);
    PORTC = v | v2;
    #elif (actions_ex == 1)
    //apply(write_a<123>(testPort1));
    auto v = apply(
            //mp::make_const_list(
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
            //)
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
    //     clearAndSet(testPort1, 1, 2);
    // }
    // PORTD = read(testPort1);
    
    return 0;
}