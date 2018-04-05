#ifndef FH_AVR_INTERRUPTS_H_
#define FH_AVR_INTERRUPTS_H_

#include "../../fields/info.hpp"
#include "../../fields/actions.hpp"
#include "registers.hpp"
#include "../../utils/functions.h"

//#ifdef FH_HAS_IRQ

namespace fasthal{    
    constexpr auto irq = avr::sreg;

    namespace details{
        using irq_t = decltype(irq);
    }

    class no_irq{
        field_data_type<details::irq_t> _irq;
    public:
        inline no_irq()
        {
            _irq = read_(irq);
            cli();
        }

        inline ~no_irq(){
            write_(irq, _irq);
        }
    };

    // enable/disable actions
	FH_FIELDBIT_ENABLE_ACTIONS(details::irq_t, irq, avr::sreg_i);

    // enable ISR by declaring __vector_<vector> and forwarding to handler
    #define FH_ISR(vector) ISR(_VECTOR(vector)) { isr<vector>(); }    

    // interrupts normalization...
    #include "interrupts_impl/uart.hpp"
}

//#endif
#endif