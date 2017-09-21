#pragma once

#ifndef AVR_ADC_H_
#define AVR_ADC_H_

#include <inttypes.h>
#include <avr/io.h>
#include "../../utils/functions.h"

namespace fasthal{

    enum AdcRef:uint8_t{
    #if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
        Default = 0,
        External = 1,
        Internal1v1 = 2,
        Internal = 2
    #elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
        Default = 0,
        External = 4,
        Internal1v1 = 8,
        Internal = Internal1v1,
        Internal2v56 = 9,
        Internal2v56_ExtCap = 13
    #else  
    #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
        Internal1v1 = 2,
        Internal2v56 = 3,
    #else
        Internal = 3,
    #endif
        Default = 1,
        External = 0
    #endif 
    };


    class Adc{        
    public:
        static void begin(AdcRef ref = AdcRef::Default){
            setReference(ref);
            enable();
        }
        // ATTINY25/45/85: REFS1 REFS0 ADLAR REFS2 MUX3 MUX2 MUX1 MUX0
        // Others: REFS1 REFS0 ADLAR MUX3 MUX2 MUX1 MUX0
        static void setReference(AdcRef ref){
            #if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
                // ref is 3 bytes with ADLAR in between
                ADMUX = (ref << 4) | (ADMUX & 0x2F);
            #else
                // ref is 2 bytes
                ADMUX = (ref << 6) | (ADMUX & 0x3F);
            #endif
        }

        static AdcRef getReference(){
            #if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
                // ref is 3 bytes with ADLAR in between
                return (AdcRef)((ADMUX >> 4) & 0x0D);
            #else
                // ref is 2 bytes
                return (AdcRef)(ADMUX >> 6);
            #endif
        }

        static void enable(){
            sbi(ADCSRA, ADEN); 
        }

        static void disable(){
            cbi(ADCSRA, ADEN);
        }
    };
    
    template<unsigned Channel>
    class AvrAdc{
    public:
        static int read(){
        #if defined(ADCSRB) && defined(MUX5)
            // the MUX5 bit of ADCSRB selects whether we're reading from channels
            // 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
            ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((Channel >> 3) & 0x01) << MUX5);
        #endif
        
        #if defined(ADMUX)
            ADMUX = (ADMUX & 0xF0) | (Channel & 0x07);
        #endif
        
            // without a delay, we seem to read from the wrong channel
            //delay(1);
        
        #if defined(ADCSRA) && defined(ADCL)
            // start the conversion
            ADCSRA |= (1 << ADSC);
        
            // ADSC is cleared when the conversion finishes
            while (ADCSRA & (1 << ADSC));
        
            // we have to read ADCL first; doing so locks both ADCL
            // and ADCH until ADCH is read.  reading ADCL second would
            // cause the results of each conversion to be discarded,
            // as ADCL and ADCH would be locked when it completed.
            return ADCL | (ADCH << 8);
        #else
            // we dont have an ADC, return 0
            return 0;
        #endif
        }
    };

    typedef AvrAdc<0> AdcA0;
    typedef AvrAdc<1> AdcA1;
    typedef AvrAdc<2> AdcA2;
    typedef AvrAdc<3> AdcA3;
    typedef AvrAdc<4> AdcA4;
    typedef AvrAdc<5> AdcA5;
    typedef AvrAdc<6> AdcA6;
    typedef AvrAdc<7> AdcA7;
    typedef AvrAdc<8> AdcA8;
    typedef AvrAdc<9> AdcA9;
    typedef AvrAdc<10> AdcA10;
    typedef AvrAdc<11> AdcA11;
    typedef AvrAdc<12> AdcA12;
    typedef AvrAdc<13> AdcA13;
    typedef AvrAdc<14> AdcA14;
    typedef AvrAdc<15> AdcA15;
}

#endif