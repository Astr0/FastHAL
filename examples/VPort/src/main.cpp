//#define VFIELD_OLD
#include <avr/io.h>
#include "fasthal.h"

using namespace fasthal;

//constexpr auto testPort = vField(pinB4, pinB5, pinB6, pinD0, invert(pinD2), nullBit);
// using TestPort = VField<PinB4, PinB5, PinB6, PinD0, PinD2Inv, NullPin>;
constexpr auto testPort = vField(pinB1, pinB2);

//constexpr auto testPin = fieldBit<0>(portB);
constexpr auto testPin = portB0;
constexpr auto testVPin = fieldBit<1>(testPort);

int main(){
    // write(testPort, 123);
    // set(testPort, 1);
    // clear(testPort, 54);
    // toggle(testPort, 0xF);

    // set(testPin);
    // clear(testPin);
    // toggle(testPin);
    // set(testPin, false);
    // if (read(testPin))
    // {
    //     clearAndSet(testPort, 1, 2);
    // }
    PORTD = read(testPort);
    //write(testPort, PORTB);
    return 0;
}