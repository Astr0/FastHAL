// nice ports abstraction

#pragma once

#ifndef AVR_PORTS_H_
#define AVR_PORTS_H_

#include "interrupts.h"
#include <avr/io.h>
#include "../../utils/functions.h"
#include "../../fields/fieldbits.hpp"

#define FASTHAL_DECLAREPORT(CODE)\
namespace priv\
{\
	FH_WRAPVARIABLE(PortReg ## CODE, PORT ## CODE)\
	FH_WRAPVARIABLE(PinReg ## CODE, PIN ## CODE)\
	FH_WRAPVARIABLE(DdrReg ## CODE, DDR ## CODE)\
}\
typedef AvrPort<priv::DdrReg ## CODE, priv::PortReg ## CODE, priv::PinReg ## CODE> Port ## CODE;\
typedef FieldBit<Port ## CODE, 0> Pin ## CODE ## 0;\
typedef FieldBit<Port ## CODE, 1> Pin ## CODE ## 1;\
typedef FieldBit<Port ## CODE, 2> Pin ## CODE ## 2;\
typedef FieldBit<Port ## CODE, 3> Pin ## CODE ## 3;\
typedef FieldBit<Port ## CODE, 4> Pin ## CODE ## 4;\
typedef FieldBit<Port ## CODE, 5> Pin ## CODE ## 5;\
typedef FieldBit<Port ## CODE, 6> Pin ## CODE ## 6;\
typedef FieldBit<Port ## CODE, 7> Pin ## CODE ## 7;\
typedef FieldBit<Port ## CODE, 0, true> Pin ## CODE ## 0Inv;\
typedef FieldBit<Port ## CODE, 1, true> Pin ## CODE ## 1Inv;\
typedef FieldBit<Port ## CODE, 2, true> Pin ## CODE ## 2Inv;\
typedef FieldBit<Port ## CODE, 3, true> Pin ## CODE ## 3Inv;\
typedef FieldBit<Port ## CODE, 4, true> Pin ## CODE ## 4Inv;\
typedef FieldBit<Port ## CODE, 5, true> Pin ## CODE ## 5Inv;\
typedef FieldBit<Port ## CODE, 6, true> Pin ## CODE ## 6Inv;\
typedef FieldBit<Port ## CODE, 7, true> Pin ## CODE ## 7Inv;\

namespace fasthal{
    struct PinMode {
        static const uint8_t Input = 0;
        static const uint8_t Output = 1;
        static const uint8_t InputPullup = 2;
    };

    template<class DDRREG, class PORTREG, class PINREG>
	class AvrPort
	{
		public:
		constexpr AvrPort(){}
		
		static constexpr uint8_t width(){return 8;}			
	
		inline static void write(uint8_t value) {PORTREG::value() = value;}
		inline static void clearAndSet(uint8_t clearMask, uint8_t setMask) {PORTREG::value() = (PORTREG::value() & ~clearMask) | setMask;}
		inline static void set(uint8_t mask) {PORTREG::value() |= mask;}
		inline static void clear(uint8_t mask) {PORTREG::value() &= ~mask;}
		inline static void toggle(uint8_t mask) {PORTREG::value() ^= mask;}
		inline static uint8_t read() {return PINREG::value();}
		inline static bool read(uint8_t mask){return PINREG::value() & mask;}

		static void setMode(uint8_t mask, uint8_t mode)
		{
			// NoInterrupts noInterrupts;
			switch (mode)
			{
				case PinMode::Input:
					DDRREG::value() &= ~mask;
					PORTREG::value() &= ~mask;
					break;
				case PinMode::InputPullup:
					DDRREG::value() &= ~mask;
					PORTREG::value() |= mask;
					break;
				default:
					// output
					DDRREG::value() |= mask;
			}
		}
			
		template<unsigned VNumber>
		struct FieldBit{
			typedef typename fasthal::FieldBit<AvrPort<DDRREG, PORTREG, PINREG>, VNumber> Type;
		};
    };	
    
    #ifdef PORTA
	FASTHAL_DECLAREPORT(A)
	#endif
	#ifdef PORTB
	FASTHAL_DECLAREPORT(B)
	#endif
	#ifdef PORTC
	FASTHAL_DECLAREPORT(C)
	#endif
	#ifdef PORTD
	FASTHAL_DECLAREPORT(D)
	#endif
	#ifdef PORTE
	FASTHAL_DECLAREPORT(E)
	#endif
	#ifdef PORTF
	FASTHAL_DECLAREPORT(F)
	#endif
	#ifdef PORTG
	FASTHAL_DECLAREPORT(G)
	#endif
	#ifdef PORTH
	FASTHAL_DECLAREPORT(H)
	#endif
	#ifdef PORTJ
	FASTHAL_DECLAREPORT(J)
    #endif
	#ifdef PORTK
	FASTHAL_DECLAREPORT(K)
    #endif
	#ifdef PORTL
	FASTHAL_DECLAREPORT(L)
    #endif
}    

#undef FASTHAL_DECLAREPORT

#endif
