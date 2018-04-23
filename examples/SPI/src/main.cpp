#define RAW
#define MODE 0
#include "fastduino.hpp"

using namespace fasthal;
using namespace fasthal::duino;

#ifndef RAW
static constexpr auto uart0 = uart<0>{};
static auto uart0tx = uart_buf_tx<uart<0>, 32>{};
FH_UART_TX(0, uart0tx);
#endif

int main(){    
    // cheat a little
    apply(        
        // activate internal pull ups for i2c
        set(ino<SDA>)
        , set(ino<SCL>)
        //init uart
        #ifndef RAW
        , uart0.begin()
        #endif
        , enable(irq)
    );

    while (1){

    }
}