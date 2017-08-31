/*
 * avr_ports.h
 *
 * Created: 01.02.2013 17:01:04
 *  Author: astr0
 */ 

#pragma once

#ifndef AVR_PORTS_H_
#define AVR_PORTS_H_

#include "avr_port.h"

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
typedef AVRPort<priv::DdrReg ## CODE, priv::PortReg ## CODE, priv::PinReg ## CODE> Port ## CODE;\
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


namespace fasthal
{
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

#endif /* AVR_PORTS_H_ */