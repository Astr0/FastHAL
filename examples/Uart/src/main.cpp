#include "fasthal.hpp"
//#include <Arduino.h>

using namespace fasthal;


int main(){
    enable_(irq);
    uart0.begin_(baud_v<9600>);
    while (true){
        // try_rx(uart0, [](auto c){ tx_sync(uart0, c); });

        for(auto x = 'a'; x <= 'z'; ++x)
            tx_sync(uart0, x);
        tx_sync(uart0, 10);
        tx_sync(uart0, 13);
        tx_done_wait(uart0);
    }

    // Serial.begin(9600);
    // while (true){
    //     for(auto x = 'a'; x <= 'z'; ++x)
    //         Serial.write(x);
    //     Serial.println();
    // }
}