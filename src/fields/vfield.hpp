#ifndef FH_VFIELD_H_
#define FH_VFIELD_H_

#include "info.hpp"
#include "../std/type_traits.hpp"
#include "fieldbit.hpp"
#include "vfield_impl.hpp"
#include "actions.hpp"
#include "actions_apply.hpp"
#include "../mp/brigand.hpp"


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
	template<class... TFieldBits, 
		typename TField = vfield<TFieldBits...>,
		typename TDataType = field_data_type<TField>>
	constexpr auto write(vfield<TFieldBits...> field, TDataType value) 
    {
        return TField::impl_t::write(value);
    }

	template<class... TFieldBits, 
		typename TField = vfield<TFieldBits...>>
	constexpr auto read(vfield<TFieldBits...> field) 
    {
        return TField::impl_t::read();
    }

	template<class... TFieldBits, 
		typename TField = vfield<TFieldBits...>,
		typename TMaskType = field_mask_type<TField>>
	constexpr auto set(vfield<TFieldBits...> field, TMaskType mask) 
	{
		return TField::impl_t::set(mask);
	}

	template<class... TFieldBits, 
		typename TField = vfield<TFieldBits...>,
		typename TMaskType = field_mask_type<TField>>
	constexpr auto clear(vfield<TFieldBits...> field, TMaskType mask) 
	{
		return TField::impl_t::clear(mask);					
	}

    template<class... TFieldBits, 
		typename TField = vfield<TFieldBits...>,
		typename TClearMask = field_mask_type<TField>,
		typename TSetMask = field_mask_type<TField>>
	constexpr auto clear_set(vfield<TFieldBits...> field, TClearMask clearMask, TSetMask setMask) 
	{
		return TField::impl_t::clear_set(clearMask, setMask);
	}

	template<class... TFieldBits, 
		typename TField = vfield<TFieldBits...>,
		typename TMaskType = field_mask_type<TField>>
	constexpr auto toggle(vfield<TFieldBits...> field, TMaskType mask) 
	{
		return TField::impl_t::toggle(mask);
	}

    template<class... TFields, class... TFieldBits, typename TField = vfield<TFieldBits...>>
    constexpr auto get_a(vfield<TFieldBits...> field, details::field_action_results_t<TFields...> results)
    {
        return TField::impl_t::read(results);
    }
}

#endif