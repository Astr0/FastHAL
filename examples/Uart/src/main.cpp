#include "fasthal.hpp"
//#include <Arduino.h>

using namespace fasthal;


int main(){
    enable_(irq);
    uart0.begin_(baud_v<9600>);
    while (true){
        if (uart0.rx_done()){
            auto error = uart0.rx_error();
            auto v = uart0.rx();
            if (!error)
                tx_sync(uart0, v);
        }
        
        // for(auto x = 'a'; x <= 'z'; ++x)
        //     uart0.tx(x);
        // uart0.tx(10);
        // uart0.tx(13);
        // uart0.flush();
    }

    // Serial.begin(9600);
    // while (true){
    //     for(auto x = 'a'; x <= 'z'; ++x)
    //         Serial.write(x);
    //     Serial.println();
    // }
}