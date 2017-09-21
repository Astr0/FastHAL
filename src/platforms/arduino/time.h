#pragma once

#ifndef ARDUINO_TIME_H_
#define ARDUINO_TIME_H_

namespace fasthal{
    class Time{
    public:
        typedef uint32_t millis_t;
        typedef uint32_t micros_t;

        static inline millis_t millis(){return ::millis();}
        static inline micros_t micros(){return ::micros();}
        static inline void delayMs(millis_t millis){::delay(millis);}
        static inline void delayUs(micros_t micros){::delayMicroseconds(micros);}
    };
}

#endif
