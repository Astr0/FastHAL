#define actions_ex 1

#include <avr/io.h>
#include "fasthal.h"
#include "mp/const_list.hpp"

using namespace fasthal;
using namespace fasthal::avr;

constexpr auto testPort1 = vField(pinB4, pinB5, pinB6, pinD0, invert(pinD2), nullBit);
//constexpr auto testPort1 = mField<42>(portB);
//constexpr auto testPort1 = vField(nullBit, nullBit, nullBit, nullBit, pinB4, pinB5, pinB6, nullBit);
constexpr auto testPort2 = portD;
//constexpr auto testPort = vField(pinB1, pinB2);



constexpr auto testPin1 = fieldBit<0>(portB);
//constexpr auto testPin = portB0;
constexpr auto testPin2 = fieldBit<1>(testPort1);

template<std::size_t V>
using value_t = brigand::uint8_t<V>;

void test_ports(){   
    #if (actions_ex == 3)
    auto v = apply(
        combine_a(
            combine_a(
                write(testPort1, value_t<123>{}),
                clear(testPort2, value_t<77>{}),
                set(testPort1, value_t<1>{})),
            write(testPort2, PORTC),
            combine_a(
                clear(testPort1, value_t<54>{}),
                toggle(testPort1, value_t<0xF>{})),
            toggle(testPort2, read_(testPort1)),
            write(testPort1, PORTB)
        ));
    // PORTC = sizeof(decltype( write(testPort2, PORTC)));
    // PORTC = sizeof(decltype(toggle(testPort2, read_(testPort1)));
    // PORTC = sizeof(write(testPort1, PORTB));
    // PORTC = sizeof(decltype(wr));
    PORTC = get(testPort1, v) | get(testPort2, v);
    #elif (actions_ex == 2)
    apply(write(testPort1, value_t<123>{}));
    apply(clear(testPort2, value_t<77>{}));
    apply(set(testPort1, value_t<1>{}));
    apply(write(testPort2, PORTC));
    apply(clear(testPort1, value_t<54>{}));
    apply(toggle(testPort1, value_t<0xF>{}));
    apply(toggle(testPort2, read_(testPort1)));
    apply(write(testPort1, PORTB));
    PORTC = read_(testPort1) | read_(testPort2);
    #elif (actions_ex == 1)
    //apply(write<123>(testPort1));
    auto v = apply(
        write(testPort1, value_t<123>{}),
        clear(testPort2, value_t<77>{}),
        set(testPort1, value_t<1>{}),
        write(testPort2, PORTC),
        clear(testPort1, value_t<54>{}),
        toggle(testPort1, value_t<0xF>{}),
        toggle(testPort2, read_(testPort1)),
        write(testPort1, PORTB));
    PORTC = get(testPort1, v) | get(testPort2, v);
    #elif (actions_ex == 0)
    write_(testPort1, value_t<123>{});
    clear_(testPort2, value_t<77>{});
    set_(testPort1, value_t<1>{});
    write_(testPort2, PORTC);
    clear_(testPort1, value_t<54>{});
    toggle_(testPort1, value_t<0xF>{});
    toggle_(testPort2, read_(testPort1));
    write_(testPort1, PORTB);
    PORTC = read_(testPort1) | read_(testPort2);
    #endif
}

void test_field_bits(){
    #if (actions_ex >= 1)
    apply(
        set(testPin1),
        set(testPin2),
        toggle(testPin1),
        set(testPin2, false)
    );
    if (read_(testPin1))
        clear_(testPin2);
    #elif (actions_ex == 0)

    set_(testPin1);
    set_(testPin2);
    toggle_(testPin1);
    set_(testPin2, false);
    if (read_(testPin1))
        clear_(testPin2);
    
    #endif
}

void test_adc_regs(){
    #if actions_ex >= 1
    apply(
        write(mux, mux_v<MUX::_0>),
        write(refs, refs_v<REFS::_0 | REFS::_1>),
        set(adps, adps_v<ADPS::_0 | ADPS::_1>),
        set(aden)
    );
    PORTC = static_cast<std::uint8_t>(read_(mux));
    #else
    constexpr auto muxMask = (1 << MUX0) | (1 << MUX1) | (1 << MUX2) | (1 << MUX3);
    constexpr auto refsMask = (1 << REFS0) | (1 << REFS1);
    ADMUX = (ADMUX & ~muxMask & ~refsMask) | (1 << MUX0) | (1 << REFS0) | (1 << REFS1);
    constexpr auto adpsMask = (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
    ADSRA1 = (ADSRA1 & ~adpsMask) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADEN);
    #endif
}


int main(){
    test_adc_regs();
    //test_ports();    
    //test_field_bits();
    
    return 0;
}