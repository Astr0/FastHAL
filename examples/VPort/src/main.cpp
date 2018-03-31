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
int main(){
    #if (actions_ex == 2)
    auto v = read(testPort1);
    v = write_a(testPort1, 123).execute(v);
    v = set_a(testPort1, 1).execute(v);
    v = clear_a(testPort1, 54).execute(v);
    v = toggle_a(testPort1, 0xF).execute(v);
    v = write_a(testPort1, PORTB).execute(v);
    write(testPort1, v);
    #elif (actions_ex == 1)
    //apply(write_a<123>(testPort1));
    apply(
            //mp::make_const_list(
            mp::make_const_list(
                write_a<123>(testPort1),
                clear_a<77>(testPort2),
                set_a<1>(testPort1)),
            write_a(testPort2, PORTC),
            mp::make_const_list(
                clear_a<54>(testPort1),
                toggle_a<0xF>(testPort1)),
            toggle_a(testPort2, read(testPort1)),
            write_a(testPort1, PORTB)
            //)
    );
    #else
    write(testPort1, 123);
    // clear(testPort2, 77);
    // set(testPort1, 1);
    // write(testPort2, PORTC);
    // clear(testPort1, 54);
    // toggle(testPort1, 0xF);
    // toggle(testPort2, read(testPort1));
    // write(testPort1, PORTB);
    #endif

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