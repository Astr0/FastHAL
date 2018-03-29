#include <avr/io.h>
#include "fasthal.h"

using namespace fasthal;

typedef VPort<PinB4, PinB5, PinB6, PinD0> TestPort;

int main(){
    TestPort::write(123);
    TestPort::set(1);
    TestPort::clear(54);
    TestPort::toggle(0xF);
    PORTD = TestPort::read();
    return 0;
}