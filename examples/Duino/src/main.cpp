#include "fasthal.hpp"
#include "fastduino.hpp"

using namespace fasthal;
using namespace fasthal::avr;
using namespace fasthal::duino;

static constexpr auto ledPin = ino<LED_BUILTIN>;
static constexpr auto adcPin = ino<A0>;

int main(){
    // PORTB = fasthal::details::get_ino_mux<A0>();
    // PORTC = fasthal::details::func_fieldbit<decltype(adcPin)>::mux_t{};
    apply(
        makeOutput(ledPin),
        begin(adc)
    );
    while(1){
        for (auto i = 0; i < 1000; ++i);
        auto v = convert_(adcPin);
        set_(ledPin, v < 128);
    }
}