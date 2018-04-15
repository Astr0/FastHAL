#ifndef FH_ARDUINO_TIME_H_
#define FH_ARDUINO_TIME_H_

#include "../../std/std_types.hpp"
#include "../../hal/time.hpp"

namespace fasthal{
    struct arduino_time{
        static inline time_t ms(){return ::millis();}
        static inline time_t us(){return ::micros();}
        static void delay_us(std::uint16_t us){::delayMicroseconds(us);}
        static void delay_ms(time_t ms){::delay(ms);}
    };
}

#endif