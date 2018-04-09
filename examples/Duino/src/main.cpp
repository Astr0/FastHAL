#include "fasthal.hpp"
#include "fastduino.hpp"

using namespace fasthal;
using namespace fasthal::avr;
using namespace fasthal::duino;

static constexpr auto ledPin = ino<LED_BUILTIN>;
static constexpr auto pwmPin = ino<11>;
static constexpr auto adcPin = ino<A0>;

int main(){
    // PORTB = fasthal::details::get_ino_mux<A0>();
    // PORTC = fasthal::details::func_fieldbit<decltype(adcPin)>::mux_t{};
    apply(
        makeOutput(ledPin),
        makeOutput(pwmPin),
        begin(timer(pwmPin)),
        begin(adc),
        set_res(adc, adc_res::_8)
    );
    while(1){
        //wait loop
        for (auto i = 0; i < 1000; ++i);
        auto v = convert8_(adcPin);
        apply(
            set(ledPin, v < 128),
            pwm(pwmPin, v)
        );
    }
}