/*
 * arduino_hal.h
 *
 * Created: 31.08.2017 16:33:04
 *  Author: astr0
 */ 
 #pragma once
  
 #ifndef ARDUINO_HAL_H_
 #define ARDUINO_HAL_H_

#define FASTHAL_TIME_ARDUINO()\
uint32_t fasthal::Time::millis(){return ::millis();}\
uint32_t fasthal::Time::micros(){return ::micros();}\
void fasthal::Time::delay(uint32_t millis){::delay(millis);}

namespace fasthal{
    namespace arduino{
        typedef fasthal::PinD0 PinD0;
        typedef fasthal::PinD1 PinD1;
        typedef fasthal::PinD2 PinD2;
        typedef fasthal::PinD3 PinD3;
        typedef fasthal::PinD4 PinD4;
        typedef fasthal::PinD5 PinD5;
        typedef fasthal::PinD6 PinD6;
        typedef fasthal::PinD7 PinD7;
        typedef fasthal::PinB0 PinD8;
        typedef fasthal::PinB1 PinD9;
        typedef fasthal::PinB2 PinD10;
        typedef fasthal::PinB3 PinD11;
        typedef fasthal::PinB4 PinD12;
        typedef fasthal::PinB5 PinD13;

        typedef fasthal::PinC0 PinA0;
        typedef fasthal::PinC1 PinA1;
        typedef fasthal::PinC2 PinA2;
        typedef fasthal::PinC3 PinA3;
        typedef fasthal::PinC4 PinA4;
        typedef fasthal::PinC5 PinA5;        

        typedef fasthal::AdcA0 AdcA0;
        typedef fasthal::AdcA1 AdcA1;
        typedef fasthal::AdcA2 AdcA2;
        typedef fasthal::AdcA3 AdcA3;
        typedef fasthal::AdcA4 AdcA4;
        typedef fasthal::AdcA5 AdcA5;
        typedef fasthal::AdcA6 AdcA6;
        typedef fasthal::AdcA7 AdcA7;

        #ifdef LED_BUILTIN 
        #define MAKE_LED_BUILTIN(Pin) typedef PinD ## Pin LedBuiltinPin;
        #define MAKE_LED_BUILTIN2(Pin) MAKE_LED_BUILTIN(Pin)
        MAKE_LED_BUILTIN2(LED_BUILTIN)
        #undef MAKE_LED_BUILTIN
        #undef MAKE_LED_BUILTIN2
        #endif
    }
}


 #endif  /* ARDUINO_HAL_H_ */