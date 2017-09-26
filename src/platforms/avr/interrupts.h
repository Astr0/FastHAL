#pragma once

#ifndef AVR_INTERNALS_H_
#define AVR_INTERNALS_H_

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

namespace fasthal{
    class NoInterrupts{
        uint8_t _sreg;
    public:
        inline NoInterrupts()
        {
            _sreg = SREG;
            cli();
        }

        inline ~NoInterrupts(){
            SREG = _sreg;	
        }

        static inline bool enabled() { return bit_is_clear(SREG, SREG_I);}
    };
	
	template<class ieReg, typename interruptenable_t>
	struct InterruptControl{
		void interruptsEnable(interruptenable_t interrupts){			
			ieReg::value() |= (uint8_t)interrupts;
		}
		
		void interruptsDisable(interruptenable_t interrupts){
			ieReg::value() &= ~((uint8_t)interrupts);
		}
	};
	
	template<class ifReg, typename interruptflag_t>
	struct InterruptFlags{
		bool interruptsReady(interruptflag_t interrupts){
			return ifReg::value() & (uint8_t)interrupts;
		}
		
		void clearInterruptsReady(interruptflag_t interrupts){
			// to clear we actually have to set them to 1 =/
			ifReg::value() |= (uint8_t)interrupts;
		}
	};
}

#endif