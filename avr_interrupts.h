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
        NoInterrupts()
        {
            _sreg = SREG;
            cli();
        }

        ~NoInterrupts(){
            SREG = _sreg;	
        }

        static bool enabled() { return bit_is_clear(SREG, SREG_I);}
    };
}

#endif