#ifndef FH_AVR_INTERRUPTS_H_
#define FH_AVR_INTERRUPTS_H_

#include "../../fields/info.hpp"
#include "../../fields/actions.hpp"
#include "registers.hpp"
#include "../../utils/functions.h"
#include "../../std/std_fake.hpp"
#include <avr/interrupt.h>

namespace fasthal{    
    namespace avr{
        #ifdef SREG
        // Interrupts constrol register
        FH_DECLARE_REGISTER_ONLY(sreg, SREG);

        // interrupts enabled flag
        #ifdef SREG_I
        constexpr auto sreg_i = fieldBit<SREG_I>(sreg);
        #endif

        #endif

        #if defined(SREG) && defined(SREG_I)
        #define FH_HAS_IRQ
        #endif
    }
    //#ifdef FH_HAS_IRQ
    #if true

    constexpr auto irq = avr::sreg;

    namespace details{
        using irq_t = std::base_type_t<decltype(irq)>;

        // enable/disable IRQ
		template<> struct func_fieldbit_impl<details::irq_t>: func_fieldbit_enable<decltype(avr::sreg_i)>{};   

        constexpr bool requires_atomic(std::size_t size){
            return size > 1;
        }
    }

    class no_irq{
        field_data_type<details::irq_t> _irq;
    public:
        inline no_irq()
        {
            _irq = read_(irq);
            cli();
        }
        
        no_irq(const no_irq&) = delete;

        inline ~no_irq(){
            write_(irq, _irq);
        }
    };

    // enable ISR by declaring __vector_<vector> and forwarding to handler
    #define FH_ISR(VECTOR, HANDLER)\
    namespace fasthal::details{ template<> struct isr_handler<VECTOR>{ static inline void handle() { HANDLER(); } }; }\
    ISR(_VECTOR(VECTOR)) { fasthal::details::isr<VECTOR>(); }    

    template<unsigned VNum>
    inline void run(interrupt<VNum> i){
        details::isr<VNum>();
    }    

    // runs irq if it's ready and enabled, but global interrupts disabled
    template<unsigned VNum>
    inline void try_irq(interrupt<VNum> i){
        if (!enabled_(irq) && enabled_(i) && ready_(i))
            details::isr<VNum>();
    }

    // runs irq if it's ready, regarding of enabled state, but global interrupts disabled
    template<unsigned VNum>
    inline void try_irq_force(interrupt<VNum> i){
        if (!enabled_(irq) && ready_(i))
            details::isr<VNum>();
    }

    template<unsigned VNum>
    inline void try_irq_force_no_irq(interrupt<VNum> i){
        if (ready_(i))
            details::isr<VNum>();
    }

    #endif
}

#endif