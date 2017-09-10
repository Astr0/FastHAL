#pragma once

#ifndef ACS712_H_
#define ACS712_H_

#include "time.h"
#include "adc.h"

namespace fasthal
{
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

#endif // ACS712_H_
