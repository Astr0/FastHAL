#include "fasthal.hpp"
//#include <Arduino.h>

using namespace fasthal;

// void uart0_rx(){
//     auto ok = uart0.rx_ok();
//     auto v = uart0.rx();
//     if (ok)
//         tx_sync(uart0, v);
// }

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

int main(){
    enable_(irq);
    uart0.begin_(baud_v<9600>);
    uart0.tx_start();
    while (true){
        // try_rx(uart0, [](auto c){ tx_sync(uart0, c); });

        // for(auto x = 'a'; x <= 'z'; ++x)
        //     tx_sync(uart0, x);
        // tx_sync(uart0, 10);
        // tx_sync(uart0, 13);
        // tx_done_wait(uart0);
    }

    // Serial.begin(9600);
    // while (true){
    //     for(auto x = 'a'; x <= 'z'; ++x)
    //         Serial.write(x);
    //     Serial.println();
    // }
}

//FH_UART_RX(0, uart0_rx);
FH_UART_TX(0, uart0_tx);