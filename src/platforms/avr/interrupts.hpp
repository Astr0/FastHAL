#ifndef FH_AVR_INTERRUPTS_H_
#define FH_AVR_INTERRUPTS_H_

#include "../../fields/info.hpp"
#include "../../fields/actions.hpp"
#include "registers.hpp"

namespace fasthal{
    class no_irq{
        field_data_type<avr::sreg> _sreg;
    public:
        inline no_irq()
        {
            _sreg = read_(avr::sreg);
            cli();
        }

        inline ~no_irq(){
            write_(avr::sreg, _sreg);
        }
    };
}
#endif