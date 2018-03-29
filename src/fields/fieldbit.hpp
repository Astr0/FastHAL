#ifndef FH_FIELDBIT_H_
#define FH_FIELDBIT_H_

#include "info.hpp"
#include <inttypes.h>

namespace fasthal
{
	// The ONE AND ONLY Bit of ANY TField which implements field interface
	template<class TField, unsigned VNumber, bool VInverted = false>
	struct FieldBit
	{
		static_assert(VNumber < TField::width(), "FieldBit number out of range");

		using Field = TField;
		using TFieldInfo = FieldInfo<TField>;

		static constexpr auto Number = typename TFieldInfo::BitNumberType { VNumber };
		static constexpr auto Mask = TFieldInfo::getPinMask(VNumber);

		static constexpr auto Inverted = VInverted;



		//using FieldType = FieldInfo::DataType;

		static void set() { TField::set(Mask); }
	
		static void clear() { TField::clear(Mask); }
	
		static void set(bool val) { 
			if (val)
				set();
			else
				clear();
		}

		static void toggle() {
			TField::toggle(Mask);
		}

		static bool read() {
			return TField::read(Mask);
		}
	};


	
}

#endif /* PIN_H_ */