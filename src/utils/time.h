#pragma once

#ifndef TIME_H_
#define TIME_H_

#include <inttypes.h>
#include "../platforms/time.h"

namespace fasthal{
    class TimeUtils{
    public:
        static inline constexpr Time::micros_t freqToMicros(uint8_t freq)
        {
            return 1000000 / freq;
        }
    };

    template<typename T, T (*GetTime)()>
    class Elapsed{
    private:
        T _lastTime;
    public:
        Elapsed(): _lastTime(GetTime()){            
        }

        void reset(){
            _lastTime = GetTime();
        }

        T elapsed(){
            return GetTime() - _lastTime;
        }

        bool elapsed(T time){
            return elapsed() >= time;
        }
    };

    typedef Elapsed<Time::millis_t, Time::millis> ElapsedMs;
}

#endif