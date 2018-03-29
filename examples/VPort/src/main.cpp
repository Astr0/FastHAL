#include <avr/io.h>
#include "fasthal.h"

using namespace fasthal;

//typedef VPort<PinB4, PinB5, PinB6, PinB7> TestPort;

struct PortBField{
    static void write(uint8_t v) { PORTB = v; }
    static uint8_t read() { return PORTB;}
};

constexpr auto portB = PortBField{};

constexpr auto nullPin0 = fieldBit<0>(nullField);
constexpr auto nullPin1 = fieldBit<0>(nullField);
constexpr auto nullPin0Inv = inverted(nullPin0);
constexpr auto nullPin0normal = inverted(nullPin0Inv);
constexpr auto testField = bitField(nullPin0Inv, nullPin1);
constexpr auto testFieldPin0 = fieldBit<0>(testField);
//constexpr auto testFieldPin3NC = fieldBit<3>(testField);
constexpr auto pinB0 = fieldBit<0>(portB);
constexpr auto pinB1 = fieldBit<1>(portB);
constexpr auto vPortB = bitField(pinB0, pinB1);



int main(){
    constexpr auto toggleTest = toggle(vPortB);
    apply(toggleTest);
    //constexpr auto wNC = set(nullPin0);
    //TestPort::write(123);
    return 0;
}