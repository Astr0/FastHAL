#include <avr/io.h>
#include "fasthal.h"

using namespace fasthal;

typedef VPort<PinB4, PinB5, PinB6, PinB7> TestPort;

int main(){
    TestPort::write(123);
    return 0;
}