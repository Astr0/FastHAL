#ifndef FH_AVR_INTERRUPTS_H_
#define FH_AVR_INTERRUPTS_H_

#include "../../fields/info.hpp"
#include "../../fields/actions.hpp"
#include "registers.hpp"
#include "../../utils/functions.h"
#include "../../std/std_fake.hpp"

//#ifdef FH_HAS_IRQ

namespace fasthal{    
    constexpr auto irq = avr::sreg;

    namespace details{
        using irq_t = std::base_type_t<decltype(irq)>;
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

    template <unsigned VNum>
    struct interrupt{
        static constexpr auto number = VNum;
    };

    // enable ISR by declaring __vector_<vector> and forwarding to handler
    #define FH_ISR(vector) ISR(_VECTOR(vector)) { isr<vector>(); }    

    // interrupts normalization...
    #include "interrupts_impl/uart_irq.hpp"

    // enable/disable IRQ
	namespace details{
		template<> struct func_fieldbit_impl<details::irq_t>: func_fieldbit_enable<decltype(avr::sreg_i)>{};        
    }

    // runs irq if it's ready and enabled, but global interrupts disabled
    template<unsigned VNum>
    inline void try_irq(interrupt<VNum> i){
        if (!enabled_(irq) && enabled_(i) && ready_(i))
            isr<VNum>();
    }

    // runs irq if it's ready, regarding of enabled state, but global interrupts disabled
    template<unsigned VNum>
    inline void try_irq_force(interrupt<VNum> i){
        if (!enabled_(irq) && ready_(i))
            isr<VNum>();
    }
}

//#endif
#endif