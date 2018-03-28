#include "fasthal.h"

using namespace fasthal;

//typedef VPort<PinB4, PinB5, PinB6, PinB7> TestPort;

constexpr auto nullPin0 = fieldBit<0>(nullField);
constexpr auto nullPin1 = fieldBit<0>(nullField);
constexpr auto nullPin0Inv = inverted(nullPin0);
constexpr auto nullPin0normal = inverted(nullPin0Inv);
constexpr auto testField = bitField(nullPin0Inv, nullPin1);
constexpr auto testFieldPin0 = fieldBit<0>(testField);

int main(){
    //TestPort::write(123);
    return 0;
}