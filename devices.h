#pragma once

#ifndef DEVICES_H_
#define DEVICES_H_

#include "dev_button.h"
#include "dev_adc.h"

namespace fasthal{
    template<class TPin>
    class PinBlink{
    private:
        uint16_t _on;
        uint16_t _off;
        ElapsedMs _elapsed;
    public:
        PinBlink():_on(0), _off(-1){            
        }

        PinBlink(uint16_t on, uint16_t off):_on(on), _off(off){            
        }

        void begin(bool on){
            TPin::setMode(PinMode::Output);
            TPin::set(on);
        }

        void reset(){
            _elapsed.reset();
            TPin::clear();
        }

        void change(uint16_t on, uint16_t off){
            _on = on;
            _off = off;
        }

        void update(){
            auto time = TPin::read() ? _on : _off;
            if (_elapsed.elapsed(time)){
                _elapsed.reset();
                TPin::toggle();
            }
        }
    };
}


#endif
