#ifndef FH_FIELDBIT_H_
#define FH_FIELDBIT_H_

#include "info.hpp"
#include <inttypes.h>

namespace fasthal
{
	// The ONE AND ONLY Bit of ANY TField which implements field interface
	template<class TField, unsigned VNumber, bool VInverted = false>
	class FieldBit
	{
		using mask_types = field_mask_types<TField>;
		static_assert(VNumber < TField::width(), "FieldBit number out of range");
		
		public:
		using Field = TField;

		static constexpr auto Number = typename mask_types::BitNumberType { VNumber };
		static constexpr auto Mask = mask_types::bitToMask(Number);
		static constexpr auto Inverted = VInverted;
	};

	template<class TField, unsigned VNumber>
	void set(FieldBit<TField, VNumber, false> fieldBit){
		TField::set(FieldBit<TField, VNumber, false>::Mask);
	}

	template<class TField, unsigned VNumber>
	void set(FieldBit<TField, VNumber, true> fieldBit){
		TField::clear(FieldBit<TField, VNumber, false>::Mask);
	}

	template<class TField, unsigned VNumber>
	void clear(FieldBit<TField, VNumber, false> fieldBit){
		TField::clear(FieldBit<TField, VNumber, false>::Mask);
	}

	template<class TField, unsigned VNumber>
	void clear(FieldBit<TField, VNumber, true> fieldBit){
		TField::set(FieldBit<TField, VNumber, false>::Mask);
	}

	template<class TField, unsigned VNumber, bool VInverted>
	void set(FieldBit<TField, VNumber, VInverted> fieldBit, bool v){
		if (v)
			set(fieldBit);
		else
			clear(fieldBit);
	}

	template<class TField, unsigned VNumber, bool VInverted>
	void toggle(FieldBit<TField, VNumber, VInverted> fieldBit){
		TField::toggle(FieldBit<TField, VNumber, VInverted>::Mask);
	}

	template<class TField, unsigned VNumber>
	bool read(FieldBit<TField, VNumber, false> fieldBit){
		return TField::read(FieldBit<TField, VNumber, false>::Mask);
	}

	template<class TField, unsigned VNumber>
	bool read(FieldBit<TField, VNumber, true> fieldBit){
		return !TField::read(FieldBit<TField, VNumber, true>::Mask);
	}
}

#endif