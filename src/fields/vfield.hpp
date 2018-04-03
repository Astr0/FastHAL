#ifndef FH_VFIELD_H_
#define FH_VFIELD_H_

#include "info.hpp"
#include "../std/type_traits.hpp"
#include "fieldbit.hpp"
#include "vfield_impl.hpp"
#include "actions.hpp"
#include "actions_apply.hpp"
#include "../mp/brigand.hpp"
#include "../std/std_fake.hpp"


namespace fasthal{
    // vfield - static field-like wrapper for FieldBits
	template<class... TFieldBits>
	struct vfield
	{
		static_assert(brigand::all<brigand::list<TFieldBits...>, brigand::bind<details::is_field_bit, brigand::_1>>::value, "Only field bits can be passed to make vfield");
	
		using impl_container_t = details::vfield_impl<TFieldBits...>;
		using datatype_t = typename impl_container_t::datatype_t;
		using impl_t = typename impl_container_t::impl_t;

		static void write(datatype_t value) 
		{
			apply(impl_t::write(value));
		}

		static auto read()
		{
			return impl_t::read(apply(impl_t::read()));
		}
	};
	namespace details{
		template<class... TFieldBits>
		struct is_field_impl<vfield<TFieldBits...>>: std::true_type{};

	    template<class... TFieldBits>
        struct needs_field_actions_impl<vfield<TFieldBits...>>: std::false_type{};

		template<class TField>
		struct is_vfield_impl: std::false_type{};

		template<class... TFieldBits>
		struct is_vfield_impl<vfield<TFieldBits...>>: std::true_type{};		

		template<class TField>
		using enable_if_vfield = std::enable_if_c<is_vfield_impl<std::base_type_t<TField>>::value>;
	}

	// create vfield
	template<class... TBits>
	constexpr decltype(auto) vField(TBits... bits){
		return vfield<TBits...>{};
	}

	// type info
	template <class... TFieldBits>
	struct field_width<vfield<TFieldBits...>>: brigand::count<TFieldBits...>{};

	// create vfield fieldBit - return passed field bit
	template<unsigned VNumber, class... TFieldBits>
	constexpr decltype(auto) fieldBit(vfield<TFieldBits...> field){
		return brigand::at_c<brigand::list<TFieldBits...>, VNumber> {};
	}

	// optimized actions 
	template<class TField, 		
		typename TDataType = field_data_type<TField>,
		details::enable_if_vfield<TField> dummy = nullptr>
	constexpr auto write(TField field, TDataType value) 
    {
        return TField::impl_t::write(value);
    }

	template<class TField, 		
		details::enable_if_vfield<TField> dummy = nullptr>
	constexpr auto read(TField field) 
    {
        return TField::impl_t::read();
    }

	template<class TField, 		
		typename TMaskType = field_mask_type<TField>,
		details::enable_if_vfield<TField> dummy = nullptr>
	constexpr auto set(TField field, TMaskType mask) 
	{
		return TField::impl_t::set(mask);
	}

	template<class TField, 		
		typename TMaskType = field_mask_type<TField>,
		details::enable_if_vfield<TField> dummy = nullptr>
	constexpr auto clear(TField field, TMaskType mask) 
	{
		return TField::impl_t::clear(mask);					
	}

	template<class TField, 		
		typename TClearMask = field_mask_type<TField>,
		typename TSetMask = field_mask_type<TField>,
		details::enable_if_vfield<TField> dummy = nullptr>
	constexpr auto clear_set(TField field, TClearMask clearMask, TSetMask setMask) 
	{
		return TField::impl_t::clear_set(clearMask, setMask);
	}

	template<class TField, 		
		typename TMaskType = field_mask_type<TField>,
		details::enable_if_vfield<TField> dummy = nullptr>
	constexpr auto toggle(TField field, TMaskType mask) 
	{
		return TField::impl_t::toggle(mask);
	}

    template<class TField, class... TFields, details::enable_if_vfield<TField> dummy = nullptr>
    constexpr auto get(TField field, details::field_action_results_t<TFields...> results)
    {
        return TField::impl_t::read(results);
    }
}

#endif