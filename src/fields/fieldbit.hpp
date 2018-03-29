#ifndef FH_FIELDBIT_H_
#define FH_FIELDBIT_H_

#include "info.hpp"
#include <inttypes.h>

namespace fasthal
{
	template<class TField, unsigned VNumber>
	class FieldBit
	{
		static_assert(VNumber < TField::width(), "FieldBit number out of range");
		typedef FieldInfo<TField> TFieldInfo;
		public:

		static constexpr bool isInverted(){return false;}
		
		static constexpr TField port() {return TField();}
		static constexpr typename TFieldInfo::PinMaskType mask() {return TFieldInfo::getPinMask(VNumber);}
		static constexpr typename TFieldInfo::PinNumberType number() {return VNumber;}
		static constexpr FieldBit<TField, VNumber> notInverted(){return FieldBit<TField, VNumber>();}

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


	template<class TFieldBit>
	class InvertedFieldBit
	{
		static_assert(!TFieldBit::isInverted(), "Don't invert inverted pins... This kills compilation time...");
		typedef FieldBitInfo<TFieldBit> TFieldBitInfo;
		public:

		constexpr InvertedFieldBit()
		{
		}

		static constexpr bool isInverted(){return !TFieldBit::isInverted();}
		
		static constexpr typename TFieldBitInfo::PortType port() {return TFieldBit::port();}
		static constexpr typename TFieldBitInfo::PinMaskType mask() {return TFieldBit::mask();}
		static constexpr typename TFieldBitInfo::PinNumberType number() {return TFieldBit::number();}
		static constexpr TFieldBit notInverted(){return TFieldBit();}

		static void set()
		{
			TFieldBit::clear();
		}
	
		static void clear()
		{
			TFieldBit::set();
		}
	
		static void set(bool val)
		{
			TFieldBit::set(!val);
		}

		static void toggle()
		{
			TFieldBit::toggle();
		}

		static bool read()
		{
			return !TFieldBit::read();
		}
		
		static void setMode(uint8_t mode){
			TFieldBit::setMode(mode);
		}
	};			
				
	
}

#endif /* PIN_H_ */