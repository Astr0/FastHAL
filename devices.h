#pragma once

#ifndef DEVICES_H_
#define DEVICES_H_

#include "time.h"
#include "acs712.h"

namespace fasthal{
    template<uint32_t (*GetTime)()>
    class Elapsed{
    private:
        uint32_t _lastTime;
    public:
        Elapsed(): _lastTime(0){            
        }

        void reset(){
            _lastTime = GetTime();
        }

        bool elapsed(uint32_t time){
            return (GetTime() - _lastTime) >= time;
        }
    };

    class ElapsedMs: public Elapsed<fasthal::Time::millis>{
    };

    template<class TPin>
    class Button{
    private:        
        ElapsedMs _noBounce;
        bool _pressed;

    public:
        Button():_pressed(false){
        }

        void begin(uint8_t mode){
            TPin::setMode(mode);
        }

        bool debounce(uint32_t time){
            bool pressed = TPin::read();
            if (pressed != _pressed){
                _pressed = pressed;
                _noBounce.reset();
                return false;
            }
            return _noBounce.elapsed(time);
        }

        bool down(uint32_t time){
            return debounce(time) && _pressed;
        }
        
        bool up(uint32_t time){
            return debounce(time) && !_pressed;
        }

        bool pressed(){
            return _pressed;
        }
    };

    template<class TPin>
    class PinBlink{
    private:
        uint32_t _on;
        uint32_t _off;
        ElapsedMs _elapsed;
    public:
        PinBlink():_on(0), _off(-1){            
        }

        PinBlink(uint32_t on, uint32_t off):_on(on), _off(off){            
        }

        void begin(bool on){
            TPin::setMode(PinMode::Output);
            TPin::set(on);
        }

        void reset(){
            _elapsed.reset();
            TPin::clear();
        }

        void change(uint32_t on, uint32_t off){
            _on = on;
            _off = off;
        }

        void update(){
            if (TPin::read()){
                if (_elapsed.elapsed(_on))
                {
                    reset();
                }
            } else{
                if (_elapsed.elapsed(_off)){
                    _elapsed.reset();
                    TPin::set();
                }
            }
        }
    };
}

#endif
