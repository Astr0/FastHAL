#pragma once

#ifndef DEV_ADC_H_
#define DEV_ADC_H_

#include <math.h>

namespace fasthal{
    template<class Adc, int (*Zero)()>
    class AdcZero{
    public:
        static inline int read(){
            return Adc::read() - Zero();
        }
    };

    //template<class Adc, unsigned Samples, unsigned Delay>
    //class AdcCalibrate{
    //static int _zero;
    //public:
        //static inline int read(){
            //return Adc::read() - _zero;
        //}
//
        //static int calibrate(){
            //int adc = 0;
            //for (unsigned i = 0; i < Samples; i++) {
                //adc += Adc::read();
                //fasthal::Time::delayMs(Delay);
            //}
            //adc /= Samples;
            //_zero = adc;
            //return adc;
        //}
    //};
//
    //template<class Adc, unsigned Timeout>
    //class AdcRms
    //{
    //public:
        //static float read(){
            //uint32_t sum = 0, count = 0;
        //
            //uint32_t t_start = fasthal::Time::micros();
            //while (fasthal::Time::micros() - t_start < Timeout) {
                //int value = Adc::read();
                //sum += value*value;
                //count++;
            //}
        //
            //return sqrt((float)sum / (float)count);
        //}
    //};

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

#define FASTHAL_ADCCALIBRATE(Adc, Delay, Samples, Name)\
typedef fasthal::AdcCalibrate<Adc, Delay, Samples> Name;\
template<> int fasthal::AdcCalibrate<Adc, Delay, Samples>::_zero = 0;


#endif