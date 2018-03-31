#include <avr/io.h>
#include "fasthal.h"

using namespace fasthal;

//constexpr auto testPort = vField(pinB4, pinB5, pinB6, pinD0, invert(pinD2), nullBit);
constexpr auto testPort = portD;
//constexpr auto testPort = vField(pinB1, pinB2);



constexpr auto testPin = fieldBit<0>(portB);
//constexpr auto testPin = portB0;
constexpr auto testVPin = fieldBit<1>(testPort);

#define actions_ex

int main(){
    #ifdef actions_ex
    apply(
        //write_ex(testPort, 123),
        //set_ex(testPort, 1),
        //clear_ex(testPort, 54),
        toggle_ex(testPort, 0xF)
        //write_ex(testPort, PORTB)
    );
    #else
    write(testPort, 123);
    // set(testPort, 1);
    // clear(testPort, 54);
    // toggle(testPort, 0xF);
     write(testPort, PORTB);
    #endif

    // set(testPin);
    // clear(testPin);
    // toggle(testPin);
    // set(testPin, false);
    // if (read(testPin))
    // {
    //     clearAndSet(testPort, 1, 2);
    // }
    // PORTD = read(testPort);
    
    return 0;
}