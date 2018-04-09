//#define FH_TIME_ARDUINO
#define FH_TIME 1
#define FH_TIME_CS _64
#define FH_UART0_TX 64

#ifdef FH_TIME_ARDUINO
#include <Arduino.h>
#endif

#include "fastduino.hpp"

using namespace fasthal;
using namespace fasthal::duino;

static constexpr auto led = ino<LED_BUILTIN>;

void setup(){
    apply(
        enable(irq)
        ,makeOutput(led)
        ,begin(uart0)
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
    print(uart0, "My time: ");
    print(uart0, ms);
    printc(uart0, ' ');
    println(uart0, us);
    delay_ms(1000);
}

#ifndef FH_TIME_ARDUINO

int main(){
    // PORTB = time.ms_inc / 256;
    // PORTB = time.ms_inc % 256; // 262, ok
    //PORTB = time.frac_inc / 256;
    //PORTB = time.frac_inc % 256; //262, ok
    setup();
    while (1){
        loop();
    }
}

#endif