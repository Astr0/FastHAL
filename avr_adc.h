#pragma once

#ifndef AVR_ADC_H_
#define AVR_ADC_H_

namespace fasthal{
    class AdcRef{        
        static uint8_t _ref;
    public:
    #if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
        static const uint8_t Default = 0;
        static const uint8_t External = 1;
        static const uint8_t Internal1v1 = 2;
        static const uint8_t Internal = 2;
    #elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
        static const uint8_t Default = 0;
        static const uint8_t External = 4;
        static const uint8_t Internal1v1 = 8;
        static const uint8_t Internal = Internal1v1;
        static const uint8_t Internal2v56 = 9;
        static const uint8_t Internal2v56_ExtCap = 13;
    #else  
    #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
        static const uint8_t Internal1v1 = 2;
        static const uint8_t Internal2v56 = 3;
    #else
        static const uint8_t Internal = 3;
    #endif
        static const uint8_t Default = 1;
        static const uint8_t External = 0;
    #endif 

        static uint8_t get();
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
        
            // set the analog reference (high two bits of ADMUX) and select the
            // channel (low 4 bits).  this also sets ADLAR (left-adjust result)
            // to 0 (the default).
        #if defined(ADMUX)
        #if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
            ADMUX = (AdcRef::get() << 4) | (Channel & 0x07);
        #else
            ADMUX = (AdcRef::get() << 6) | (Channel & 0x07);
        #endif
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
}

#define FASTHAL_INITADC(x) uint8_t fasthal::AdcRef::get(){return x;}


#endif