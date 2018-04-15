//#define FH_TIME_ARDUINO
#define FH_TIME 1
#define FH_TIME_WGM pwm_pcdef

#ifdef FH_TIME_ARDUINO
#include <Arduino.h>
#endif

#include "fastduino.hpp"

using namespace fasthal;
using namespace fasthal::duino;

static constexpr auto led = ino<LED_BUILTIN>;
static constexpr auto uart0 = uart<0>{};
static constexpr auto uart0tx = uart_sync_tx<0>{};
//auto time = avr_time<timer<0>>{};

void setup(){
    apply(
        enable(irq)
        ,makeOutput(led)
        ,uart0.begin()
        #ifdef FH_TIME
        ,begin(time)
        #endif
    );
}

void loop(){    
    auto ms = time_ms();
    auto us = time_us();
    if (us > ms)
        toggle_(led);
    print(uart0tx, "My time: ");
    print(uart0tx, ms);
    print(uart0tx, ' ');
    println(uart0tx, us);
    // delay_us approximation (doesn't count for loop cycles)
    if (read_(led)) {
        for (auto i = 0; i < 1000; ++i)
            delay_us(1000);
    } else {
        delay_ms(1000);
    }
}

#ifndef FH_TIME_ARDUINO

int main(){
    setup();
    while (1){
        loop();
    }
}

#endif