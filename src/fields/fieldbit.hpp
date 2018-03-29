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
		static_assert(VNumber < field_width<TField>(), "FieldBit number out of range");
		
		public:
		using Field = TField;

		static constexpr auto Number = typename mask_types::BitNumberType { VNumber };
		static constexpr auto Mask = mask_types::bitToMask(Number);
		static constexpr auto Inverted = VInverted;
	};

	// create field bit
	template<unsigned VNumber, class TField>
	constexpr decltype(auto) fieldBit(TField field){
		return FieldBit<TField, VNumber, false>{};
	}

	// invert field bit
	template<class TField, unsigned VNumber, bool VInverted>
	constexpr decltype(auto) invert(FieldBit<TField, VNumber, VInverted> fieldBit){
		return FieldBit<TField, VNumber, !VInverted>{};
	}

	template<class TField, unsigned VNumber>
	void set(FieldBit<TField, VNumber, false> fieldBit){
		set(TField{}, FieldBit<TField, VNumber, false>::Mask);
	}

	template<class TField, unsigned VNumber>
	void set(FieldBit<TField, VNumber, true> fieldBit){
		clear(TField{}, FieldBit<TField, VNumber, false>::Mask);
	}

	template<class TField, unsigned VNumber>
	void clear(FieldBit<TField, VNumber, false> fieldBit){
		clear(TField{}, FieldBit<TField, VNumber, false>::Mask);
	}

	template<class TField, unsigned VNumber>
	void clear(FieldBit<TField, VNumber, true> fieldBit){
		set(TField{}, FieldBit<TField, VNumber, false>::Mask);
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
		toggle(TField{}, FieldBit<TField, VNumber, VInverted>::Mask);
	}

	template<class TField, unsigned VNumber>
	bool read(FieldBit<TField, VNumber, false> fieldBit){
		return read(TField{}, FieldBit<TField, VNumber, false>::Mask);
	}

	template<class TField, unsigned VNumber>
	bool read(FieldBit<TField, VNumber, true> fieldBit){
		return !read(TField{}, FieldBit<TField, VNumber, true>::Mask);
	}
}

#endif