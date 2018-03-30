#ifndef FH_FIELDBIT_H_
#define FH_FIELDBIT_H_

#include "info.hpp"
#include <inttypes.h>

namespace fasthal
{
	// The ONE AND ONLY Bit of ANY TField which implements field interface
	template<class TField, unsigned VNumber, bool VInverted = false>
	class field_bit
	{
		using mask_types_t = field_mask_types<TField>;
		static_assert(VNumber < field_width<TField>(), "field_bit number out of range");
		
		public:
		using field_t = TField;

		static constexpr auto number = typename mask_types_t::BitNumberType { VNumber };
		static constexpr auto mask = mask_types_t::bitToMask(number);
		static constexpr auto inverted = VInverted; 
	};

	// create field bit
	template<unsigned VNumber, class TField>
	constexpr decltype(auto) fieldBit(TField field){
		return field_bit<TField, VNumber, false>{};
	}

	// invert field bit
	template<class TField, unsigned VNumber, bool VInverted>
	constexpr decltype(auto) invert(field_bit<TField, VNumber, VInverted> fieldBit){
		return field_bit<TField, VNumber, !VInverted>{};
	}

	template<class TField, unsigned VNumber>
	void set(field_bit<TField, VNumber, false> fieldBit){
		set(TField{}, field_bit<TField, VNumber, false>::mask);
	}

	template<class TField, unsigned VNumber>
	void set(field_bit<TField, VNumber, true> fieldBit){
		clear(TField{}, field_bit<TField, VNumber, false>::mask);
	}

	template<class TField, unsigned VNumber>
	void clear(field_bit<TField, VNumber, false> fieldBit){
		clear(TField{}, field_bit<TField, VNumber, false>::mask);
	}

	template<class TField, unsigned VNumber>
	void clear(field_bit<TField, VNumber, true> fieldBit){
		set(TField{}, field_bit<TField, VNumber, false>::mask);
	}

	template<class TField, unsigned VNumber, bool VInverted>
	void set(field_bit<TField, VNumber, VInverted> fieldBit, bool v){
		if (v)
			set(fieldBit);
		else
			clear(fieldBit);
	}

	template<class TField, unsigned VNumber, bool VInverted>
	void toggle(field_bit<TField, VNumber, VInverted> fieldBit){
		toggle(TField{}, field_bit<TField, VNumber, VInverted>::mask);
	}

	template<class TField, unsigned VNumber>
	bool read(field_bit<TField, VNumber, false> fieldBit){
		return read(TField{}, field_bit<TField, VNumber, false>::mask);
	}

	template<class TField, unsigned VNumber>
	bool read(field_bit<TField, VNumber, true> fieldBit){
		return !read(TField{}, field_bit<TField, VNumber, true>::mask);
	}
}

#endif