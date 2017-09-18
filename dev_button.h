#pragma once

#ifndef DEV_BUTTON_H_
#define DEV_BUTTON_H_

#include "time.h"
#include <inttypes.h>

namespace fasthal{
    template<class TPin, uint16_t Interval>
    class Bounce{
    private:   
        enum BounceState{
            Unstable = 0,
            Debounced = 1,
            Changed = 2
        };
        
        ElapsedMs _elapsed;
        uint8_t _state;
        
    public:
        Bounce():_state(BounceState::Unstable){
        }

        Bounce(bool state): _state(state ? BounceState::Debounced : BounceState::Unstable){            
        }

        void begin(uint8_t mode){
            TPin::setMode(mode);
        }

        bool update(){
            bool current = TPin::read();
            _state &= ~BounceState::Changed;

            if (current != readUnstable()){
                _elapsed.reset();
                _state ^= BounceState::Unstable;
            } else if ((current != read()) && _elapsed.elapsed(Interval)){
                // debounced state changed and threashold passed
                _state = (_state ^ BounceState::Debounced) | BounceState::Changed;
                return true;
            }
            return false;
        }

        bool stateStable(uint16_t time){
            return _elapsed.elapsed(time);
        }

        bool changed(){
            return _state & BounceState::Changed;
        }

        bool readUnstable(){
            return _state & BounceState::Unstable;
        }

        bool read(){
            return _state & BounceState::Debounced;
        }

        bool fell(){
            return changed() && !read();
        }

        bool rose(){
            return changed() && read();
        }
    };
}    

#endif