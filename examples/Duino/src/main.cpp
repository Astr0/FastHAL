#include "fasthal.hpp"
#include "fastduino.hpp"

using namespace fasthal;
using namespace fasthal::duino;

static constexpr auto ledPin = ino<LED_BUILTIN>;

int main(){
    
    makeOutput_(ledPin);
    while(1){
        for (auto i = 0; i < 1000; ++i);
        toggle_(ledPin);
    }
}