#pragma once

#ifndef ACS712_H_
#define ACS712_H_

#include "time.h"
#include "adc.h"

namespace fasthal
{
    template <class Adc, unsigned ACFrequency, unsigned VRef, unsigned AdcScale, unsigned Sensitivity>
    class Acs712{
        typedef AdcRms<Adc, fasthal::Time::freqToMicros(ACFrequency)> RmsAdc;
        static constexpr float _factor = ((float)VRef / (float)AdcScale) * (float)Sensitivity / 1000.0f;
    public:
        static inline float readAC(){
            return RmsAdc::read() * _factor;
        }

        static inline float readDC(){
            return Adc::read() * _factor;                
        }
    };

    template <class Adc, unsigned ACFrequency, unsigned VRef, unsigned AdcScale>
    class Acs712_05B: public Acs712<Adc, ACFrequency, VRef, AdcScale, 185>{};

    template <class Adc, unsigned ACFrequency, unsigned VRef, unsigned AdcScale>
    class Acs712_20A: public Acs712<Adc, ACFrequency, VRef, AdcScale, 100>{};

    template <class Adc, unsigned ACFrequency, unsigned VRef, unsigned AdcScale>
    class Acs712_30A: public Acs712<Adc, ACFrequency, VRef, AdcScale, 66>{};
}

#endif // ACS712_H_
