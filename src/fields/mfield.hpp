#ifndef FH_MFIELD_H_
#define FH_MFIELD_H_

#include "info.hpp"
#include "nullfield.hpp"
#include "../std/type_traits.hpp"
#include "../std/std_fake.hpp"

namespace fasthal{
    template<class TField, field_mask_type<TField> VMask, typename TDataType = field_data_type<TField>>
    struct mfield {
        static_assert(is_field<TField>::value, "TField is not a field");
        static_assert((~VMask) != decltype(VMask){}, "Full mask, use field directly!");

        static constexpr auto field = TField{};
        using field_t = TField;
        using datatype_t = TDataType;
        using mask_t = field_mask_type<TField>;
        static constexpr auto mask = VMask;
        static constexpr auto mask_c = integral_constant<mask_t, VMask>{};

        template<typename T>
        static constexpr auto mask_value(T value){
            return static_cast<mask_t>(value) & mask;
        }

        template<typename T, T V>
        static constexpr auto mask_value(integral_constant<T, V> value){
            constexpr auto result = mask_value(V);
            return integral_constant<decltype(result), result>{};
        }

        static void write(datatype_t value) 
		{
            // clear and set
            TField::write(TField::read() & ~mask | mask_value(value));
		}

		static auto read()
		{
			return static_cast<datatype_t>(TField::read() & mask);
		}

    };

    namespace details{
		template<class TField, field_mask_type<TField> VMask, typename TDataType>
		struct is_field_impl<mfield<TField, VMask, TDataType>>: std::true_type{};

		template<class TField, field_mask_type<TField> VMask, typename TDataType>
        struct needs_field_actions_impl<mfield<TField, VMask, TDataType>>: std::false_type{};

		template<class TField>
		struct is_mfield_impl: std::false_type{};

		template<class TField, field_mask_type<TField> VMask, typename TDataType>
		struct is_mfield_impl<mfield<TField, VMask, TDataType>>: std::true_type{};		

		template<class TField>
		using enable_if_mfield = std::enable_if_c<is_mfield_impl<std::base_type_t<TField>>::value>;
	}

	// create mfield
	template<unsigned VMask, class TField>
	constexpr decltype(auto) mField(TField field){
		return mfield<TField, field_mask_type<TField>{ VMask }>{};
	}

	template<unsigned VMask, typename TDataType, class TField>
	constexpr decltype(auto) mField(TField field){
		return mfield<TField, field_mask_type<TField>{ VMask }, TDataType>{};
	}

	// create mfield fieldBit - check for mask, mask ok
	template<unsigned VNumber, class TField, field_mask_type<TField> VMask, typename TDataType, std::enable_if_c<(VMask & (1 << VNumber)) != 0> dummy = nullptr>
	constexpr decltype(auto) fieldBit(mfield<TField, VMask, TDataType> field){
		return fieldBit<VNumber>(mfield<TField, VMask, TDataType>::field);
	}

    // not masked bit - return nullBit
    template<unsigned VNumber, class TField, field_mask_type<TField> VMask, typename TDataType, std::enable_if_c<(VMask & (1 << VNumber)) == 0> dummy = nullptr>
	constexpr decltype(auto) fieldBit(mfield<TField, VMask, TDataType> field){
		return nullBit;
	}


    // optimized actions 
	template<class TField, 		
		typename TDataType = field_data_type<TField>,
		details::enable_if_mfield<TField> dummy = nullptr>
	constexpr auto write(TField field, TDataType value) 
    {
        // full clear, set masked
        return clear_set(TField::field, TField::mask_c, TField::mask_value(value));
    }

	template<class TField, 		
		details::enable_if_mfield<TField> dummy = nullptr>
	constexpr auto read(TField field) 
    {
        // read underlying
        return read(TField::field);
    }

	template<class TField, 		
		typename TMaskType = field_mask_type<TField>,
		details::enable_if_mfield<TField> dummy = nullptr>
	constexpr auto set(TField field, TMaskType mask) 
	{
        // set masked
		return set(TField::field, TField::mask_value(mask));
	}

	template<class TField, 		
		typename TMaskType = field_mask_type<TField>,
		details::enable_if_mfield<TField> dummy = nullptr>
	constexpr auto clear(TField field, TMaskType mask) 
	{
        // clear masked
		return clear(TField::field, TField::mask_value(mask));					
	}

	template<class TField, 		
		typename TClearMask = field_mask_type<TField>,
		typename TSetMask = field_mask_type<TField>,
		details::enable_if_mfield<TField> dummy = nullptr>
	constexpr auto clear_set(TField field, TClearMask clearMask, TSetMask setMask) 
	{
        // mask both
		return clear_set(TField::field, TField::mask_value(clearMask), TField::mask_value(setMask));
	}

	template<class TField, 		
		typename TMaskType = field_mask_type<TField>,
		details::enable_if_mfield<TField> dummy = nullptr>
	constexpr auto toggle(TField field, TMaskType mask) 
	{
        // mask
		return toggle(TField::field, TField::mask_value(mask));
	}

    template<class TField, class... TFields, details::enable_if_mfield<TField> dummy = nullptr>
    constexpr auto get(TField field, details::field_action_results_t<TFields...> results)
    {
        // get underlying and mask it
        return static_cast<typename TField::datatype_t>(get(TField::field, results) & TField::mask);
    }
}

#endif