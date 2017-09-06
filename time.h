#pragma once

#ifndef TIME_H_
#define TIME_H_

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
}

#endif