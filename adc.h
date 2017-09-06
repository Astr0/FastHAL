#pragma once

#ifndef ADC_H_
#define ADC_H_

#include "time.h"

namespace fasthal
{
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
        static int read(){
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
}

#define FASTHAL_ADCCALIBRATE(Adc, Delay, Samples, Name)\
typedef fasthal::AdcCalibrate<Adc, Delay, Samples> Name;\
template<> int fasthal::AdcCalibrate<Adc, Delay, Samples>::_zero = 0;

#endif // ADC_H_