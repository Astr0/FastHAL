#ifndef FH_FIELDBIT_H_
#define FH_FIELDBIT_H_

#include "info.hpp"
#include "../std/type_traits.hpp"
#include "../std/std_types.hpp"
#include "../std/std_fake.hpp"
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
		static constexpr auto inline zero_mask = typename mask_types_t::masktype_t{};

		static constexpr auto inline number = typename mask_types_t::bitnumer_t { VNumber };
		static constexpr auto inline mask = mask_types_t::bitToMask(number);
		// TODO: check if integral_constant is possible
		static constexpr auto inline mask_c = integral_constant<decltype(mask), mask>{};
		static constexpr auto inline inverted = VInverted; 
	};

	namespace details{
		template<class T>
		struct is_field_bit: std::false_type{};

		template<class TField, unsigned VNumber, bool VInverted>
		struct is_field_bit<field_bit<TField, VNumber, VInverted>>: std::true_type{};		
	}

	// create field bit
	template<unsigned VNumber, class TField, enable_if_field_c<TField> dummy = nullptr>
	constexpr auto inline fieldBit(TField field){
		return field_bit<TField, VNumber, false>{};
	}

	// invert field bit
	template<class TField, unsigned VNumber, bool VInverted>
	constexpr decltype(auto) invert(field_bit<TField, VNumber, VInverted> fieldBit){
		return field_bit<TField, VNumber, !VInverted>{};
	}

	namespace details{
		template<class TFieldBit>
		static constexpr auto inline set_fieldbit(TFieldBit fieldBit, integral_constant<bool, true>){
			return set(typename TFieldBit::field_t{}, TFieldBit::mask_c);
		}
		template<class TFieldBit>
		static constexpr auto inline set_fieldbit(TFieldBit fieldBit, integral_constant<bool, false>){
			return clear(typename TFieldBit::field_t{}, TFieldBit::mask_c);
		}
	}
	// actions
	template<class TField, unsigned VNumber, bool VInverted>
	constexpr auto inline set(field_bit<TField, VNumber, VInverted> fieldBit){
		return details::set_fieldbit(fieldBit, integral_constant<bool, !VInverted>{});
	}

	template<class TField, unsigned VNumber, bool VInverted>
	constexpr auto inline clear(field_bit<TField, VNumber, VInverted> fieldBit){
		return details::set_fieldbit(fieldBit, integral_constant<bool, VInverted>{});
	}

	template<class TField, unsigned VNumber, bool VInverted>
	constexpr auto inline set(field_bit<TField, VNumber, VInverted> fieldBit, bool v){
		using fieldbit_t = field_bit<TField, VNumber, VInverted>;
		if constexpr (VInverted) 
			v = !v;
		// shitty, but type of result should be the same...
		return v 
			? clear_set(TField{}, fieldbit_t::zero_mask, fieldbit_t::mask)
			: clear_set(TField{}, fieldbit_t::mask, fieldbit_t::zero_mask);
	}

	template<class TField, unsigned VNumber, bool VInverted, bool V>
	constexpr auto inline set(field_bit<TField, VNumber, VInverted> fieldBit, integral_constant<bool, V> v){
		return details::set_fieldbit(fieldBit, integral_constant<bool, VInverted ? !V : V>{});
	}

	template<class TField, unsigned VNumber, bool VInverted>
	constexpr auto inline toggle(field_bit<TField, VNumber, VInverted> fieldBit){
		return toggle(TField{}, field_bit<TField, VNumber, VInverted>::mask_c);
	}

	template<class TField, unsigned VNumber, bool VInverted>
	constexpr auto inline read(field_bit<TField, VNumber, VInverted> fieldBit){
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
	inline void set_(field_bit<TField, VNumber, VInverted> fieldBit, bool v){
		if constexpr (VInverted)
			v = !v;
		if (v)
			set_(TField{}, field_bit<TField, VNumber, VInverted>::mask_c);
		else
			clear_(TField{}, field_bit<TField, VNumber, VInverted>::mask_c);
		//apply(set(fieldBit, v));
	}

	template<class TField, unsigned VNumber, bool VInverted>
	inline void set_(field_bit<TField, VNumber, VInverted> fieldBit){
		if constexpr (VInverted)
			clear_(TField{}, field_bit<TField, VNumber, VInverted>::mask_c);
		else
			set_(TField{}, field_bit<TField, VNumber, VInverted>::mask_c);
		//apply(set(fieldBit));
	}
	
	template<class TField, unsigned VNumber, bool VInverted>
	inline void clear_(field_bit<TField, VNumber, VInverted> fieldBit){
		if constexpr (VInverted)
			set_(TField{}, field_bit<TField, VNumber, VInverted>::mask_c);
		else
			clear_(TField{}, field_bit<TField, VNumber, VInverted>::mask_c);
		//apply(clear(fieldBit));
	}



	template<class TField, unsigned VNumber, bool VInverted>
	inline void toggle_(field_bit<TField, VNumber, VInverted> fieldBit){
		toggle_(TField{}, field_bit<TField, VNumber, VInverted>::mask_c);
		//apply(toggle(fieldBit));
	}

	template<class TField, unsigned VNumber, bool VInverted>
	constexpr bool inline read_(field_bit<TField, VNumber, VInverted> fieldBit){
		bool v = TField::read() & field_bit<TField, VNumber, VInverted>::mask;
		if constexpr (VInverted)
			v = !v;
		return v;
		//return get(fieldBit, apply(read(fieldBit)));
	}

	// they are evil and not inlined!!!
	template<class TField, unsigned VNumber, bool VInverted>
	constexpr auto inline wait_lo(field_bit<TField, VNumber, VInverted> fieldBit){
		while (read_(field_bit<TField, VNumber, VInverted>{}));
	}

	template<class TField, unsigned VNumber, bool VInverted>
	constexpr auto inline wait_hi(field_bit<TField, VNumber, VInverted> fieldBit){
		while (!read_(field_bit<TField, VNumber, VInverted>{}));
	}	
}

#endif