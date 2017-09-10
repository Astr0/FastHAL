#pragma once

#ifndef DEVICES_H_
#define DEVICES_H_

#include "time.h"

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

    template<class Adc, int (*Zero)()>
    class AdcZero{
    public:
        static inline int read(){
            return Adc::read() - Zero();
        }
    };

    template<class Adc, unsigned Samples, unsigned Delay>
    class AdcCalibrate{
    static int _zero;
    public:
        static inline int read(){
            return Adc::read() - _zero;
        }

        static int calibrate(){
            int adc = 0;
            for (unsigned i = 0; i < Samples; i++) {
                adc += Adc::read();
                fasthal::Time::delay(Delay);
            }
            adc /= Samples;
            _zero = adc;
            return adc;
        }
    };

    template<class Adc, unsigned Timeout>
    class AdcRms
    {
    public:
        static float read(){
            uint32_t sum = 0, count = 0;
        
            uint32_t t_start = fasthal::Time::micros();
            while (fasthal::Time::micros() - t_start < Timeout) {
                int value = Adc::read();
                sum += value*value;
                count++;
            }
        
            return sqrt((float)sum / (float)count);
        }
    };

    template <class Adc, unsigned VRef, unsigned AdcScale, unsigned Sensitivity>
    class Acs712{
       
        // (Vref / AdcScale) / (Sensitivity/100)
        static constexpr float _factor = ((float)VRef / (float)AdcScale) / ((float)Sensitivity / 1000.0f);
    public:
        static inline float read(){
            return Adc::read() * _factor;                
        }
    };

    template <class Adc, unsigned VRef, unsigned AdcScale>
    class Acs712_05B: public Acs712<Adc, VRef, AdcScale, 185>{};

    template <class Adc, unsigned VRef, unsigned AdcScale>
    class Acs712_20A: public Acs712<Adc, VRef, AdcScale, 100>{};

    template <class Adc, unsigned VRef, unsigned AdcScale>
    class Acs712_30A: public Acs712<Adc, VRef, AdcScale, 66>{};
}

#define FASTHAL_ADCCALIBRATE(Adc, Delay, Samples, Name)\
typedef fasthal::AdcCalibrate<Adc, Delay, Samples> Name;\
template<> int fasthal::AdcCalibrate<Adc, Delay, Samples>::_zero = 0;


#endif
