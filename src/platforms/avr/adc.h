#pragma once

#ifndef AVR_ADC_H_
#define AVR_ADC_H_

#include <avr/io.h>

#if defined(ADMUX) && defined(ADCSRA) && defined(ADEN) && defined(ADSC)

#include <inttypes.h>
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
		
		static void start(){
			// start the conversion
			sbi(ADCSRA, ADSC); 
		}
		
		static bool running(){
			// ADSC is cleared when the conversion finishes
			return ADCSRA & (1 << ADSC);
		}
		
		static void wait(){
			while (running());
		}
		
	
		static void select(uint8_t channel, bool is8bit){
			#if defined(ADCSRB) && defined(MUX5)
			// the MUX5 bit of ADCSRB selects whether we're reading from channels
			// 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
			ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((channel >> 3) & 0x01) << MUX5);
			ADMUX = (ADMUX & 0xD0) | (channel & 0x07) | (is8bit ? (1 << ADLAR) : 0);
			#else
			// channel is 4 bits for temperature sensors and other stuff
			ADMUX = (ADMUX & 0xD0) | (channel & 0x0F) | (is8bit ? (1 << ADLAR) : 0);
			#endif
		}
		
		static uint16_t read(){
			// we have to read ADCL first; doing so locks both ADCL
			// and ADCH until ADCH is read.  reading ADCL second would
			// cause the results of each conversion to be discarded,
			// as ADCL and ADCH would be locked when it completed.
			return ADCL | (ADCH << 8);
		}

		static uint8_t read8bit(){
			return ADCH;
		}			
    };
    
		
    template<uint8_t Channel, bool is8bit>
    class AdcChannel{
    public:	
		typedef typename Loki::Select<is8bit, uint8_t, uint16_t>::Result result_t;
	
		static result_t read(){			
			Adc::select(Channel, is8bit);
			Adc::start();
            Adc::wait();
			return is8bit ? Adc::read8bit() : Adc::read();
        }
    };
}

#endif // ADC stuff

#endif