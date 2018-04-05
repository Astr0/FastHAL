#include "fasthal.hpp"

using namespace fasthal;

// comment and it won't compile
template<>
void fasthal::isr<FH_IRQ_RX0>(){
    PORTB = 12;
}

int main(){
    if (!enabled_(irq))
        enable_(irq);

    while (1){

    }
}
FH_ISR(FH_IRQ_RX0);