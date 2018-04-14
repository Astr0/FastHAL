#define MODE 1
// 1 - echo sync
// 2 - a...z sync
// 3 - echo async
// 4 - a...z asyc
// 5 - echo arduino
// 6 - a..z arduino


#include "fasthal.hpp"

#if (MODE == 5 || MODE == 6)
#include <Arduino.h>
#endif

using namespace fasthal;

static constexpr auto uart0 = uart<0>{};

int main(){
    enable_(irq);
    #if (MODE == 5 || MODE == 6)
    Serial.begin(9600);
    #else
    uart0.begin_(baud_v<9600>);
    #endif

    #if (MODE == 4)
    uart0.tx_start();
    #endif

    while (true){
        #if (MODE == 1)
        try_rx(uart0, [](auto c){ tx_sync(uart0, c); });
        #elif (MODE == 2)
        for(auto x = 'a'; x <= 'z'; ++x)
            tx_sync(uart0, x);
        tx_sync(uart0, 10);
        tx_sync(uart0, 13);
        #elif (MODE == 5)
        Serial.write(Serial.read());
        #elif (MODE==6)
        for(auto x = 'a'; x <= 'z'; ++x)
             Serial.write(x);
         Serial.println();        
        #endif
    }
}

#if (MODE == 3)
void uart0_rx(){
    auto ok = uart0.rx_ok();
    auto v = uart0.rx();
    if (ok)
        tx_sync(uart0, v);
}
FH_UART_RX(0, uart0_rx);
#elif (MODE == 4)
char c = 'a';
void uart0_tx(){
    switch(c){
        case 'z' + 1: 
            uart0.tx(10);
            break;
        case 'z' + 2:
            uart0.tx(13);
            c = 'a';
            return;
        default:
            uart0.tx(c);
            break;
    }
    c++;
}
FH_UART_TX(0, uart0_tx);
#endif