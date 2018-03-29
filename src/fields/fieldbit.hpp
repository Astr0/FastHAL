#ifndef FH_FIELDBIT_H_
#define FH_FIELDBIT_H_

#include "info.hpp"
#include <inttypes.h>

namespace fasthal
{
	template<class TField, unsigned VNumber>
	class Pin
	{
		static_assert(VNumber < TField::width(), "Pin number out of range");
		typedef PortInfo<TField> TPortInfo;
		public:

		constexpr Pin()
		{
		}

		static constexpr bool isInverted(){return false;}
		
		static constexpr TField port() {return TField();}
		static constexpr typename TPortInfo::PinMaskType mask() {return TPortInfo::getPinMask(VNumber);}
		static constexpr typename TPortInfo::PinNumberType number() {return VNumber;}
		static constexpr Pin<TField, VNumber> notInverted(){return Pin<TField, VNumber>();}

		static void set()
		{
			TField::set(mask());
		}
	
		static void clear()
		{
			TField::clear(mask());
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
			TField::toggle(mask());
		}

		static bool read()
		{
			return TField::read(mask());
		}

		static void setMode(uint8_t mode){
			TField::setMode(mask(), mode);
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