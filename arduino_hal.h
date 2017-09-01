/*
 * arduino_hal.h
 *
 * Created: 31.08.2017 16:33:04
 *  Author: astr0
 */ 
 #pragma once
  
 #ifndef ARDUINO_HAL_H_
 #define ARDUINO_HAL_H_

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
    }
}


 #endif  /* ARDUINO_HAL_H_ */