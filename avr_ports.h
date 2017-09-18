#pragma once

#ifndef AVR_PORTS_H_
#define AVR_PORTS_H_

#include "avr_internals.h"

#define FASTHAL_WRAPVARIABLE(CLASSNAME, VARNAME)\
struct CLASSNAME\
{\
static decltype(VARNAME)& value(){return VARNAME;}\
};

#define FASTHAL_DECLAREPORT(CODE)\
namespace priv\
{\
	FASTHAL_WRAPVARIABLE(PortReg ## CODE, PORT ## CODE)\
	FASTHAL_WRAPVARIABLE(PinReg ## CODE, PIN ## CODE)\
	FASTHAL_WRAPVARIABLE(DdrReg ## CODE, DDR ## CODE)\
}\
typedef AvrPort<priv::DdrReg ## CODE, priv::PortReg ## CODE, priv::PinReg ## CODE> Port ## CODE;\
typedef Pin<Port ## CODE, 0> Pin ## CODE ## 0;\
typedef Pin<Port ## CODE, 1> Pin ## CODE ## 1;\
typedef Pin<Port ## CODE, 2> Pin ## CODE ## 2;\
typedef Pin<Port ## CODE, 3> Pin ## CODE ## 3;\
typedef Pin<Port ## CODE, 4> Pin ## CODE ## 4;\
typedef Pin<Port ## CODE, 5> Pin ## CODE ## 5;\
typedef Pin<Port ## CODE, 6> Pin ## CODE ## 6;\
typedef Pin<Port ## CODE, 7> Pin ## CODE ## 7;\
typedef InvertedPin<Pin ## CODE ## 0> Pin ## CODE ## 0Inv;\
typedef InvertedPin<Pin ## CODE ## 1> Pin ## CODE ## 1Inv;\
typedef InvertedPin<Pin ## CODE ## 2> Pin ## CODE ## 2Inv;\
typedef InvertedPin<Pin ## CODE ## 3> Pin ## CODE ## 3Inv;\
typedef InvertedPin<Pin ## CODE ## 4> Pin ## CODE ## 4Inv;\
typedef InvertedPin<Pin ## CODE ## 5> Pin ## CODE ## 5Inv;\
typedef InvertedPin<Pin ## CODE ## 6> Pin ## CODE ## 6Inv;\
typedef InvertedPin<Pin ## CODE ## 7> Pin ## CODE ## 7Inv;\

namespace fasthal{
    struct PinMode{
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
		inline static void clearAndSet(uint8_t clearMask, uint8_t setMask) {PORTREG::value() = PORTREG::value() & ~clearMask | setMask;}
		inline static void set(uint8_t mask) {PORTREG::value() |= mask;}
		inline static void clear(uint8_t mask) {PORTREG::value() &= ~mask;}
		inline static void toggle(uint8_t mask) {PORTREG::value() ^= mask;}
		inline static uint8_t read() {return PINREG::value();}
		inline static bool read(uint8_t mask){return PINREG::value() & mask;}

		static void setMode(uint8_t mask, uint8_t mode)
		{
			NoInterrupts noInterrupts;
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
					DDRREG::value() |= mask;
			}
		}
			
		template<unsigned VNumber>
		struct Pin{
			typedef typename fasthal::Pin<AvrPort<DDRREG, PORTREG, PINREG>, VNumber> Type;
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
}    

#undef FASTHAL_DECLAREPORT
#undef FASTHAL_WRAPVARIABLE

#endif
