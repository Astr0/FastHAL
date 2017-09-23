/*
 * pin.h
 *
 * Created: 03.02.2013 1:39:43
 *  Author: astr0
 */ 

#ifndef PIN_H_
#define PIN_H_

#include "pininfo.h"
#include "portinfo.h"
#include <inttypes.h>

namespace fasthal
{
	template<class TPort, unsigned VNumber>
	class Pin
	{
		static_assert(VNumber < TPort::width(), "Pin number out of range");
		typedef PortInfo<TPort> TPortInfo;
		public:

		constexpr Pin()
		{
		}

		static constexpr bool isInverted(){return false;}
		
		static constexpr TPort port() {return TPort();}
		static constexpr typename TPortInfo::PinMaskType mask() {return TPortInfo::getPinMask(VNumber);}
		static constexpr typename TPortInfo::PinNumberType number() {return VNumber;}
		static constexpr Pin<TPort, VNumber> notInverted(){return Pin<TPort, VNumber>();}

		static void set()
		{
			TPort::set(mask());
		}
	
		static void clear()
		{
			TPort::clear(mask());
		}
	
		static void set(bool val)
		{
			if (val)
				set();
			else
				clear();
		}

		static void toggle()
		{
			TPort::toggle(mask());
		}

		static bool read()
		{
			return TPort::read(mask());
		}

		static void setMode(uint8_t mode){
			TPort::setMode(mask(), mode);
		}
	};


	template<class TPin>
	class InvertedPin
	{
		static_assert(!TPin::isInverted(), "Don't invert inverted pins... This kills compilation time...");
		typedef PinInfo<TPin> TPinInfo;
		public:

		constexpr InvertedPin()
		{
		}

		static constexpr bool isInverted(){return !TPin::isInverted();}
		
		static constexpr typename TPinInfo::PortType port() {return TPin::port();}
		static constexpr typename TPinInfo::PinMaskType mask() {return TPin::mask();}
		static constexpr typename TPinInfo::PinNumberType number() {return TPin::number();}
		static constexpr TPin notInverted(){return TPin();}

		static void set()
		{
			TPin::clear();
		}
	
		static void clear()
		{
			TPin::set();
		}
	
		static void set(bool val)
		{
			TPin::set(!val);
		}

		static void toggle()
		{
			TPin::toggle();
		}

		static bool read()
		{
			return !TPin::read();
		}
		
		static void setMode(uint8_t mode){
			TPin::setMode(mode);
		}
	};			
				
	
}

#endif /* PIN_H_ */