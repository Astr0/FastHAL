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
}

#endif