#pragma once

#ifndef AVR_ADC_H_
#define AVR_ADC_H_

#include <avr/io.h>

#if defined(ADMUX) && defined(ADCSRA) && defined(ADEN) && defined(ADSC)

#include <inttypes.h>
#include "../../utils/functions.h"

namespace fasthal{

	// ATTINY24/44/84: ADMUX: REFS1 REFS0 MUX5  MUX4  MUX3 MUX2 MUX1 MUX0.   ADCSRA: ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0.   ADCSRB: BIN ACME X   ADLAR X    ADTS2 ADTS1 ADTS0
    // ATTINY25/45/85: ADMUX: REFS1 REFS0 ADLAR REFS2 MUX3 MUX2 MUX1 MUX0.   ADCSRA: ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0.   ADCSRB: BIN ACME IPR X     X    ADTS2 ADTS1 ADTS0 
	// Mega:           ADMUX: REFS1 REFS0 ADLAR MUX4  MUX3 MUX2 MUX1 MUX0.   ADCSRA: ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0    ADCSRB: X   ACME X   X     MUX5 ADTS2 ADTS1 ADTS0
    // Others:         ADMUX: REFS1 REFS0 ADLAR X     MUX3 MUX2 MUX1 MUX0.   ADCSRA: ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0.   ADCSRB: X   ACME X   X     X    ADTS2 ADTS1 ADTS0
    enum class AdcRef:uint8_t{
    #if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
        Default             = (0 << REFS1) | (0 << REFS0),
        External            = (0 << REFS1) | (1 << REFS0),
        Internal1v1         = (1 << REFS1) | (0 << REFS0)
        //Internal            = (1 << REFS1) | (1 << REFS0)
    #elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
        Default             = (0 << REFS2) | (0 << REFS1) | (0 << REFS0),
        External            = (0 << REFS2) | (1 << REFS1) | (1 << REFS0),
        Internal1v1         = (0 << REFS2) | (1 << REFS1) | (0 << REFS0),
        Internal2v56        = (1 << REFS2) | (1 << REFS1) | (0 << REFS0),
        Internal2v56_ExtCap = (1 << REFS2) | (1 << REFS1) | (1 << REFS0)
    #else  
        Default      = (0 << REFS1) | (1 << REFS0),
        External     = (0 << REFS1) | (0 << REFS0),
    #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
        Internal1v1  = (1 << REFS1) | (0 << REFS0),
        Internal2v56 = (1 << REFS1) | (1 << REFS0)
    #else
        Internal     = (1 << REFS1) | (1 << REFS0)
    #endif
    #endif 
    };
	
	enum class AdcPrescaler: uint8_t{
		P2   = (0 << ADPS2) | (0 << ADPS1) | (1 << ADPS0),
		P4   = (0 << ADPS2) | (1 << ADPS1) | (0 << ADPS0),
		P8   = (0 << ADPS2) | (1 << ADPS1) | (1 << ADPS0),
		P16  = (1 << ADPS2) | (0 << ADPS1) | (0 << ADPS0),
		P32  = (1 << ADPS2) | (0 << ADPS1) | (1 << ADPS0),
		P64  = (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0),
		P128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0),
		// default a2d prescaler should be inside the desired 50-200 KHz range.
		#if F_CPU >= 16000000 // 16 MHz / 128 = 125 KHz
		Default = P128
		#elif F_CPU >= 8000000 // 8 MHz / 64 = 125 KHz
		Default = P64
		#elif F_CPU >= 4000000 // 4 MHz / 32 = 125 KHz
		Default = P32
		#elif F_CPU >= 2000000 // 2 MHz / 16 = 125 KHz
		Default = P16		
		#elif F_CPU >= 1000000 // 1 MHz / 8 = 125 KHz
		Default = P8
		#elif F_CPU >= 500000 // 0.5 MHz / 4 = 125 KHz
		Default = P4
		#else // 128 kHz / 2 = 64 KHz 
		Default = P2
		#endif			
	};


    class Adc{        
    public:
		static void begin(AdcPrescaler prescaler = AdcPrescaler::Default){
			ADCSRA |= (1 << ADEN) | ((uint8_t)prescaler);
		}
		
		static void end() {
			disable();
		}
	
		static void select(AdcRef ref, bool is8bit, uint8_t mux){
			#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
			// 5 MUX bits and ADLAR IN ADCSRB
			ADMUX = ((uint8_t)ref) | (mux & 0x1F);
			
			fh_vbi(ADCSRB, ADLAR, is8bit);
			#else
						
			#if defined(ADCSRB) && defined(MUX5)
			// the MUX5 bit of ADCSRB selects whether we're reading from channels
			// 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
			fh_vbi(ADCSRB, MUX5, mux & 0x10)
			#endif
			
			ADMUX = ((uint8_t)ref) | (is8bit ? (1 << ADLAR) : 0) | (mux & 0x0F);

			#endif
		}	
	
	    static void enable(){
            fh_sbi(ADCSRA, ADEN); 
        }

        static void disable(){
            fh_cbi(ADCSRA, ADEN);
        }
		
		static void start(){
			// start the conversion
			fh_sbi(ADCSRA, ADSC);
		}
		
		static bool running(){
			// ADSC is cleared when the conversion finishes
			return ADCSRA & (1 << ADSC);
		}
		
		static void wait(){
			while (running());
		}
		
		static uint16_t read(){
			// we have to read ADCL first; doing so locks both ADCL
			// and ADCH until ADCH is read.  reading ADCL second would
			// cause the results of each conversion to be discarded,
			// as ADCL and ADCH would be locked when it completed.
			return ADC;
		}

		static uint8_t read8bit(){
			return ADCH;
		}			
    };
    
		
    template<AdcRef ref, bool is8bit, uint8_t mux>
    class AdcChannel{
    public:	
		typedef typename Loki::Select<is8bit, uint8_t, uint16_t>::Result result_t;
	
		static result_t read(){			
			Adc::select(ref, is8bit, mux);
			Adc::start();
            Adc::wait();
			return is8bit ? Adc::read8bit() : Adc::read();
        }
    };
}

#endif // ADC stuff

#endif