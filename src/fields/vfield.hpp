#ifndef FH_VFIELD_H_
#define FH_VFIELD_H_

#include "info.hpp"
#include "../std/type_traits.hpp"
#include "fieldbit.hpp"
#include "vfield_impl.hpp"
#include "actions_ex.hpp"
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
			//impl_t::write(value);
			apply(impl_t::write_a(value));
		}

		static auto read()
		{
			return impl_t::read();
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

	// optimized actions_ex
	template<class... TFieldBits, 
		typename TField = vfield<TFieldBits...>,
		typename TDataType = field_data_type<TField>>
	constexpr auto write_a(vfield<TFieldBits...> field, TDataType value) 
    {
        return TField::impl_t::write_a(value);
    }

	template<class... TFieldBits, 
		typename TField = vfield<TFieldBits...>>
	constexpr auto read_a(vfield<TFieldBits...> field) 
    {
        return TField::impl_t::read_a();
    }

	template<class... TFieldBits, 
		typename TField = vfield<TFieldBits...>,
		typename TMaskType = field_mask_type<TField>>
	constexpr auto set_a(vfield<TFieldBits...> field, TMaskType mask) 
	{
		return TField::impl_t::set_a(mask);
	}

	template<class... TFieldBits, 
		typename TField = vfield<TFieldBits...>,
		typename TMaskType = field_mask_type<TField>>
	constexpr auto clear_a(vfield<TFieldBits...> field, TMaskType mask) 
	{
		return TField::impl_t::clear_a(mask);					
	}

    template<class... TFieldBits, 
		typename TField = vfield<TFieldBits...>,
		typename TClearMask = field_mask_type<TField>,
		typename TSetMask = field_mask_type<TField>>
	constexpr auto clear_set_a(vfield<TFieldBits...> field, TClearMask clearMask, TSetMask setMask) 
	{
		return TField::impl_t::clear_set_a(clearMask, setMask);
	}

	template<class... TFieldBits, 
		typename TField = vfield<TFieldBits...>,
		typename TMaskType = field_mask_type<TField>>
	constexpr auto toggle_a(vfield<TFieldBits...> field, TMaskType mask) 
	{
		return TField::impl_t::toggle_a(mask);
	}

    template<class... TFields, class... TFieldBits, typename TField = vfield<TFieldBits...>>
    constexpr auto get_a(vfield<TFieldBits...> field, details::field_action_results_t<TFields...> results)
    {
        return TField::impl_t::read(results);
    }

	// optimized actions
	template<class... TFieldBits, 
		typename TField = vfield<TFieldBits...>,
		typename TMaskType = field_mask_type<TField>>
	void set(vfield<TFieldBits...> field, TMaskType mask) 
	{
		apply(set_a(field, mask));
	}

	template<class... TFieldBits, 
		typename TField = vfield<TFieldBits...>,
		typename TMaskType = field_mask_type<TField>>
	void clear(vfield<TFieldBits...> field, TMaskType mask) 
	{
		apply(clear_a(field, mask));
	}

    template<class... TFieldBits, 
		typename TField = vfield<TFieldBits...>,
		typename TMaskType = field_mask_type<TField>>
	void clear_set(vfield<TFieldBits...> field, TMaskType clearMask, TMaskType setMask) 
	{
		return apply(clear_set_a(field, clearMask, setMask));
	}				

	template<class... TFieldBits, 
		typename TField = vfield<TFieldBits...>,
		typename TMaskType = field_mask_type<TField>>
	void toggle(vfield<TFieldBits...> field, TMaskType mask) 
	{
		return apply(toggle_a(field, mask));
	}
}

#endif