//#define FH_TIME_ARDUINO
//#define FH_TIME 1
//#define FH_TIME_WGM pwm_pcdef

#ifdef FH_TIME_ARDUINO
#include <Arduino.h>
#endif

#include "fastduino.hpp"

using namespace fasthal;
using namespace fasthal::duino;

auto time = timer_time<timer<0>>{};
FH_TIME(0, time);
auto time2 = timer_time<timer<2>>{};
FH_TIME(2, time2);

FH_USE_TIME(time);

static constexpr auto led = ino<LED_BUILTIN>;
static constexpr auto uart0 = uart<0>{};
static constexpr auto uart0tx = uart_sync_tx<0>{};


void setup(){
    apply(
        makeOutput(led)
        ,uart0.begin()
        ,time.begin()
        ,time2.begin()
        ,enable(irq)
    );
}

template<class TTime>
void print_time(TTime& t, char num){
    auto ms = t.ms();
    auto us = t.us();
    print(uart0tx, num);
    print(uart0tx, " my time: ");
    print(uart0tx, ms);
    print(uart0tx, ' ');
    println(uart0tx, us);
}

void loop(){    
    toggle_(led);
    print_time(time, '0');
    print_time(time2, '2');
    constexpr auto global = global_time();
    print_time(global, 'G');

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