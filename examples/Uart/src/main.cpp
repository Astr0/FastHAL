//#define FH_UART0_RX 1
//#define FH_UART0_TX 1

#include "fasthal.hpp"
//#include <Arduino.h>

using namespace fasthal;

static constexpr auto uart0 = uart<0>{};
// FH_UART_TX(0, uart0);

int main(){
    enable_(irq);
    uart0.begin_(baud_v<9600>);
    while (true){
        // if (available(uart0)){
        //     auto v = read_dirty(uart0);
        //     write(uart0, v);
        // }
        
        for(auto x = 'a'; x <= 'z'; ++x)
            uart0.tx(x);
        uart0.tx(10);
        uart0.tx(13);
        uart0.flush();
    }

    // Serial.begin(9600);
    // while (true){
    //     for(auto x = 'a'; x <= 'z'; ++x)
    //         Serial.write(x);
    //     Serial.println();
    // }
}