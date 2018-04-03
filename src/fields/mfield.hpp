#ifndef FH_MFIELD_H_
#define FH_MFIELD_H_

#include "info.hpp"
#include "../std/type_traits.hpp"
#include "../std/std_fake.hpp"

namespace fasthal{
    template<class TField, field_mask_type<TField> VMask>
    struct mfield {
        static_assert((~VMask) != decltype(VMask){}, "Full mask, use field directly!");

        static constexpr auto field = TField{};
        using field_t = TField;
        using datatype_t = field_data_type<field_t>;
        using mask_t = field_mask_type<TField>;
        static constexpr auto mask = VMask;
        static constexpr auto mask_c = integral_constant<mask_t, VMask>{};

        static void write(datatype_t value) 
		{
            // clear and set
            TField::write(TField::read() & ~mask | (value & mask));
		}

		static auto read()
		{
			return TField::read() & mask;
		}

        template<typename T>
        static constexpr auto mask_value(T value){
            return value & mask;
        }

        template<typename T, T V>
        static constexpr auto mask_value(integral_constant<T, V> value){
            constexpr auto result = mask_value(V);
            return integral_constant<decltype(result), result>{};
        }
    };

    namespace details{
		template<class TField, field_mask_type<TField> VMask>
		struct is_field_impl<mfield<TField, VMask>>: std::true_type{};

		template<class TField, field_mask_type<TField> VMask>
        struct needs_field_actions_impl<mfield<TField, VMask>>: std::false_type{};

		template<class TField>
		struct is_mfield_impl: std::false_type{};

		template<class TField, field_mask_type<TField> VMask>
		struct is_mfield_impl<mfield<TField, VMask>>: std::true_type{};		

		template<class TField>
		using enable_if_mfield = std::enable_if_c<is_mfield_impl<std::base_type_t<TField>>::value>;
	}

    // optimized actions 
	template<class TField, 		
		typename TDataType = field_data_type<TField>,
		details::enable_if_mfield<TField> dummy = nullptr>
	constexpr auto write(TField field, TDataType value) 
    {
        return clear_set(TField::field, TField::mask_c, TField::mask_value(value));
    }

	template<class TField, 		
		details::enable_if_mfield<TField> dummy = nullptr>
	constexpr auto read(TField field) 
    {
        return read(TField::field);
    }

	template<class TField, 		
		typename TMaskType = field_mask_type<TField>,
		details::enable_if_mfield<TField> dummy = nullptr>
	constexpr auto set(TField field, TMaskType mask) 
	{
		return set(TField::field, TField::mask_value(mask));
	}

	template<class TField, 		
		typename TMaskType = field_mask_type<TField>,
		details::enable_if_mfield<TField> dummy = nullptr>
	constexpr auto clear(TField field, TMaskType mask) 
	{
		return clear(TField::field, TField::mask_value(mask));					
	}

	template<class TField, 		
		typename TClearMask = field_mask_type<TField>,
		typename TSetMask = field_mask_type<TField>,
		details::enable_if_mfield<TField> dummy = nullptr>
	constexpr auto clear_set(TField field, TClearMask clearMask, TSetMask setMask) 
	{
		return clear_set(TField::field, TField::mask_value(clearMask), TField::mask_value(setMask));
	}

	template<class TField, 		
		typename TMaskType = field_mask_type<TField>,
		details::enable_if_mfield<TField> dummy = nullptr>
	constexpr auto toggle(TField field, TMaskType mask) 
	{
		return toggle(TField::field, TField::mask_value(mask));
	}

    template<class TField, class... TFields, details::enable_if_mfield<TField> dummy = nullptr>
    constexpr auto get(TField field, details::field_action_results_t<TFields...> results)
    {
        return get(TField::field, results) & TField::mask;
    }
}

#endif