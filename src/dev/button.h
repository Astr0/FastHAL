#pragma once

#ifndef DEV_BUTTON_H_
#define DEV_BUTTON_H_

#include <inttypes.h>
#include "../sys/maskutils.h"

namespace fasthal{
	
	//  nice debouncer for TFieldBit. Uses TStableElapsed instance to poll stability interval (elapsed() and reset()).
    template<class TFieldBit, class TStableElapsed>
    class Bounce{
    private:   
        enum BounceState{
            Unstable = 1,
            Debounced = 2,
            Changed = 4
        };
        
		TStableElapsed _stable;
        uint8_t _state;
        
    public:
		Bounce(): _state(0){
			
		}
	
        Bounce(bool state): _state(state ? (BounceState::Debounced | BounceState::Unstable) : 0){
        }

        void begin(uint8_t mode){
            TFieldBit::setMode(mode);
        }

        bool update(){
            bool current = TFieldBit::read();
            _state &= ~BounceState::Changed;

			if (current == readUnstable()){				
				if (_stable.elapsed() && (current != read())){
					// debounced state changed and threshold passed
					_state = (_state ^ BounceState::Debounced) | BounceState::Changed;
					return true;
				}
			} else {
                _stable.reset();
                _state ^= BounceState::Unstable;
			}

            return false;
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

    enum ButtonEvent: uint8_t{
        None = 0,
        Click = 1,
        LongClick = 2,
        ClickClick = Click << 2 | Click,
        LongClickClick = LongClick << 2 | Click,
        ClickLongClick = Click << 2 | LongClick,
        LongClickLongClick = LongClick << 2 | LongClick
    };

    //template<class TBounce, uint16_t LongClickInterval, uint16_t SequenceInterval>
    //class Button{
    //private:
        //TBounce _bounce;
        //uint8_t _sequence;
        //ElapsedMs _stateTime;
    //public:
        //Button(bool state): _bounce(state), _sequence(0){
        //}
//
        //Button():_sequence(0){            
        //}
//
        //void begin(uint8_t mode){
            //_bounce.begin(mode);
        //}
//
        //ButtonEvent update(){
            //if (!_bounce.update())
                //return ButtonEvent::None; 
            //// state changed
            //auto prevStateTime = _stateTime.elapsed();
            //_stateTime.reset();
//
            //if (_bounce.read())
            //{
                //// button was pressed - check if sequence continues
                //_sequence = (prevStateTime <= SequenceInterval) ? (_sequence << 2) : 0;
                //return ButtonEvent::None;
            //}
            //else
            //{
                //// button was up - it's either short, long or double click
                //_sequence |= (prevStateTime < LongClickInterval) ? ButtonEvent::Click : ButtonEvent::LongClick;
                //return ButtonEvent(_sequence);
            //}            
        //}
//
        //void reset(){
            //_sequence = 0;
        //}
    //};
}    

#endif