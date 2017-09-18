#pragma once

#ifndef TIME_H_
#define TIME_H_

#include <inttypes.h>

namespace fasthal{
    class Time{
    public:
        static uint32_t micros();
        static uint32_t millis();
        static void delay(uint32_t millis);
        
        static inline constexpr uint32_t freqToMicros(uint8_t freq)
        {
            return 1000000 / freq;
        }
    };

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

        uint32_t elapsed(){
            return GetTime() - _lastTime;
        }

        bool elapsed(uint32_t time){
            return elapsed() >= time;
        }

        bool elapsed(uint16_t time){
            return elapsed() >= time;
        }
    };

    class ElapsedMs: public Elapsed<fasthal::Time::millis>{
    };
}

#endif