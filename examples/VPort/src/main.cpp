#include <avr/io.h>
#include "fasthal.h"

using namespace fasthal;

using TestPort = VField<PinB4, PinB5, PinB6, PinD0, PinD2Inv, NullPin>;
using TestPin = PinB1;

int main(){
    TestPort::write(123);
    TestPort::set(1);
    TestPort::clear(54);
    TestPort::toggle(0xF);
    TestPin::set();
    TestPin::clear();
    TestPin::toggle();
    TestPin::set(false);
    if (TestPin::read())
    {
        TestPort::clearAndSet(1, 2);
    }
    PORTD = TestPort::read();
    return 0;
}