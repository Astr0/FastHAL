/*
 * avr_ports.h
 *
 * Created: 01.02.2013 16:33:04
 *  Author: astr0
 */ 
#pragma once


#ifndef AVR_PORT_H_
#define AVR_PORT_H_

#include "pin.h"
#include "defines.h"

namespace fasthal
{
	template<class DDRREG, class PORTREG, class PINREG>
	class AVRPort
	{
		public:
		constexpr AVRPort(){}
		
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
			uint8_t oldSREG = SREG;
			cli();
			switch (mode)
			{
				case INPUT:
					DDRREG::value() &= ~mask;
					PORTREG::value() &= ~mask;
					break;
				case INPUT_PULLUP:
					DDRREG::value() &= ~mask;
					PORTREG::value() |= mask;
					break;
				default:
					DDRREG::value() |= mask;
			}
			SREG = oldSREG;			
		}
			
		template<unsigned VNumber>
		struct Pin{
			typedef typename fasthal::Pin<AVRPort<DDRREG, PORTREG, PINREG>, VNumber> Type;
		};
	};	
}

#endif /* AVR_PORT_H_ */