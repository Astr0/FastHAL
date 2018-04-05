#include "fasthal.hpp"

using namespace fasthal;

#define MY_ISR USART_RX_vect_num

// comment and it won't compile
template<>
void fasthal::isr<MY_ISR>(){
    PORTB = 12;
}

int main(){
    if (!enabled_(irq))
        enable_(irq);

    while (1){

    }
}
FH_ISR(MY_ISR);