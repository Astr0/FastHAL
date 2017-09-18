#pragma once

#ifndef AVR_INTERNALS_H_
#define AVR_INTERNALS_H_

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
    };
}

#endif