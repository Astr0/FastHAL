#include <Arduino.h>
#define FH_TIME_ARDUINO
#define FH_UART0_TX 64

#include "fastduino.hpp"

using namespace fasthal;
using namespace fasthal::duino;

static constexpr auto led = ino<LED_BUILTIN>;

#ifdef FH_TIME_ARDUINO
void setup(){
    apply(
        makeOutput(led),
        begin(uart0)
    );
}

void loop(){
    toggle_(led);
    auto ms = time_ms();
    auto us = time_us();
    print(uart0, "My time: ");
    print(uart0, ms);
    printc(uart0, ' ');
    println(uart0, us);
    delay_ms(1000);
}

#else

int main(){
    while (1){}
}

#endif