#include <avr/io.h>
#include "fasthal.h"

using namespace fasthal;

using TestPort = VField<PinB4, PinB5, PinB6, PinD0, PinD2Inv, NullPin>;
using TestPin = PinB1;
constexpr auto testPin = TestPin{};

int main(){
    TestPort::write(123);
    TestPort::set(1);
    TestPort::clear(54);
    TestPort::toggle(0xF);
    set(testPin);
    clear(testPin);
    toggle(testPin);
    set(testPin, false);
    if (read(testPin))
    {
        TestPort::clearAndSet(1, 2);
    }
    PORTD = TestPort::read();
    return 0;
}