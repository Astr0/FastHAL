#include "fasthal.h"

using namespace fasthal;

typedef VPort<PinB4, PinB5, PinB6, PinB7> TestPort;

template<class DataPort, class ControlPin, class SckPin>
class LCD{
public:
    static void write(uint8_t value){
        {
            NoInterrupts no;      
            ControlPin::clear();
            DataPort::write(value);
        }
        SckPin::set();
        SckPin::clear();
    }
};

typedef LCD<PortD, PinB0, PinB1> MyLcd;


int main(){
    MyLcd::write('a');
    return 0;
}