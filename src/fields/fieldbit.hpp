#ifndef FH_FIELDBIT_H_
#define FH_FIELDBIT_H_

#include "info.hpp"
#include "../std/type_traits.hpp"
#include "../std/std_types.hpp"
#include "../mp/brigand.hpp"
#include "../utils/types.hpp"
#include "actions.hpp"

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
		static constexpr auto zero_mask = typename mask_types_t::masktype_t{};

		static constexpr auto number = typename mask_types_t::bitnumer_t { VNumber };
		static constexpr auto mask = mask_types_t::bitToMask(number);
		// TODO: check if integral_constant is possible
		static constexpr auto mask_c = integral_constant<decltype(mask), mask>{};
		static constexpr auto inverted = VInverted; 
	};

	namespace details{
		template<class T>
		struct is_field_bit: std::false_type{};

		template<class TField, unsigned VNumber, bool VInverted>
		struct is_field_bit<field_bit<TField, VNumber, VInverted>>: std::true_type{};
	}

	// create field bit
	template<unsigned VNumber, class TField, enable_if_field_c<TField> dummy = nullptr>
	constexpr auto fieldBit(TField field){
		return field_bit<TField, VNumber, false>{};
	}

	// invert field bit
	template<class TField, unsigned VNumber, bool VInverted>
	constexpr decltype(auto) invert(field_bit<TField, VNumber, VInverted> fieldBit){
		return field_bit<TField, VNumber, !VInverted>{};
	}

	// actions
	template<class TField, unsigned VNumber>
	constexpr auto set(field_bit<TField, VNumber, false> fieldBit){
		return set(TField{}, field_bit<TField, VNumber, false>::mask_c);
	}

	template<class TField, unsigned VNumber>
	constexpr auto set(field_bit<TField, VNumber, true> fieldBit){
		return clear(TField{}, field_bit<TField, VNumber, false>::mask_c);
	}

	template<class TField, unsigned VNumber>
	constexpr auto clear(field_bit<TField, VNumber, false> fieldBit){
		return clear(TField{}, field_bit<TField, VNumber, false>::mask_c);
	}

	template<class TField, unsigned VNumber>
	constexpr auto clear(field_bit<TField, VNumber, true> fieldBit){
		return set(TField{}, field_bit<TField, VNumber, false>::mask_c);
	}

	template<class TField, unsigned VNumber>
	constexpr auto set(field_bit<TField, VNumber, false> fieldBit, bool v){
		using fieldbit_t = field_bit<TField, VNumber, false>;
		return v 
			? clear_set(TField{}, fieldbit_t::zero_mask, fieldbit_t::mask)
			: clear_set(TField{}, fieldbit_t::mask, fieldbit_t::zero_mask);
	}

	template<class TField, unsigned VNumber>
	constexpr auto set(field_bit<TField, VNumber, true> fieldBit, bool v){
		using fieldbit_t = field_bit<TField, VNumber, true>;
		return v 
			? clear_and_set(TField{}, fieldbit_t::mask, fieldbit_t::zero_mask)
			: clear_and_set(TField{}, fieldbit_t::zero_mask, fieldbit_t::mask);
	}

	template<class TField, unsigned VNumber, bool VInverted>
	constexpr auto toggle(field_bit<TField, VNumber, VInverted> fieldBit){
		return toggle(TField{}, field_bit<TField, VNumber, VInverted>::mask_c);
	}

	template<class TField, unsigned VNumber, bool VInverted>
	constexpr auto read(field_bit<TField, VNumber, VInverted> fieldBit){
		return read(TField{});
	}

	template<class TField, unsigned VNumber, typename... TFields>
	constexpr bool get(field_bit<TField, VNumber, false> fieldBit, details::field_action_results_t<TFields...> results){
		return (get(TField{}, results) & field_bit<TField, VNumber, false>::mask);
	}

	template<class TField, unsigned VNumber, typename... TFields>
	constexpr bool get(field_bit<TField, VNumber, true> fieldBit, details::field_action_results_t<TFields...> results){
		return !get(field_bit<TField, VNumber, false>{}, results);
	}

	// immediate actions
	template<class TField, unsigned VNumber, bool VInverted>
	constexpr void set_(field_bit<TField, VNumber, VInverted> fieldBit){
		apply(set(fieldBit));
	}
	
	template<class TField, unsigned VNumber, bool VInverted>
	constexpr void clear_(field_bit<TField, VNumber, VInverted> fieldBit){
		apply(clear(fieldBit));
	}

	template<class TField, unsigned VNumber, bool VInverted>
	constexpr void set_(field_bit<TField, VNumber, VInverted> fieldBit, bool v){
		apply(set(fieldBit, v));
	}

	template<class TField, unsigned VNumber, bool VInverted>
	constexpr void toggle_(field_bit<TField, VNumber, VInverted> fieldBit){
		apply(toggle(fieldBit));
	}

	template<class TField, unsigned VNumber, bool VInverted>
	constexpr auto read_(field_bit<TField, VNumber, VInverted> fieldBit){
		return get(fieldBit, apply(read(fieldBit)));
	}

	
}

#endif