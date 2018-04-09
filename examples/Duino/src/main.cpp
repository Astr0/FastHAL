#include "fasthal.hpp"
//#include "fastduino.hpp"
//#include "pins_arduino.h"
using namespace fasthal;

static constexpr auto ledPin = pinB5;

int main(){
    makeOutput_(ledPin);
    while(1){
        for (auto i = 0; i < 1000; ++i);
        toggle_(ledPin);
    }
}