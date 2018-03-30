// nice ports abstraction

#pragma once

#ifndef AVR_PORTS_H_
#define AVR_PORTS_H_

#include "interrupts.h"
#include <avr/io.h>
#include "../../utils/functions.h"
//#include "../../fields/fieldbits.hpp"

#define FASTHAL_DECLAREREGISTER( NAME, VAR)\
namespace priv{\
	FH_WRAPVARIABLE(VAR ## Reg, VAR)\
}\
static constexpr auto NAME = AvrRegister<priv::VAR ## Reg>{};\
static constexpr auto NAME ## 0 = fieldBit<0>(NAME);\
static constexpr auto NAME ## 1 = fieldBit<1>(NAME);\
static constexpr auto NAME ## 2 = fieldBit<2>(NAME);\
static constexpr auto NAME ## 3 = fieldBit<3>(NAME);\
static constexpr auto NAME ## 4 = fieldBit<4>(NAME);\
static constexpr auto NAME ## 5 = fieldBit<5>(NAME);\
static constexpr auto NAME ## 6 = fieldBit<6>(NAME);\
static constexpr auto NAME ## 7 = fieldBit<7>(NAME);\


#define FASTHAL_DECLAREPORT(CODE)\
FASTHAL_DECLAREREGISTER(port ## CODE, PORT ## CODE)\
FASTHAL_DECLAREREGISTER(pin ## CODE, PIN ## CODE)\
FASTHAL_DECLAREREGISTER(ddr ## CODE, DDR ## CODE)

namespace fasthal{
    template<class TAccess>
	struct AvrRegister
	{
		inline static void write(uint8_t value) { TAccess::value() = value; }
		inline static uint8_t read() {return TAccess::value();}
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
