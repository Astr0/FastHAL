#ifndef FH_VFIELD_H_
#define FH_VFIELD_H_

#include "info.hpp"
#include "../std/type_traits.hpp"
#include "fieldbit.hpp"
#include "vfield_impl.hpp"


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
			impl_t::write(value);
		}

		static auto read() 
		{
			auto result = datatype_t{};
			impl_t::read(result);
			return result;
		}
	};
	namespace details{
		template<class... TFieldBits>
		struct is_field_impl<vfield<TFieldBits...>>: std::true_type{};
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
		typename TMaskType = field_mask_type<TField>>
	static void clear_set(vfield<TFieldBits...> field, TMaskType clearMask, TMaskType setMask) 
	{
		TField::impl_t::clear_set(clearMask, setMask);
	}				

	template<class... TFieldBits, 
		typename TField = vfield<TFieldBits...>,
		typename TMaskType = field_mask_type<TField>>
	static void set(vfield<TFieldBits...> field, TMaskType mask) 
	{
		TField::impl_t::set(mask);
	}
	template<class... TFieldBits, 
		typename TField = vfield<TFieldBits...>,
		typename TMaskType = field_mask_type<TField>>
	static void clear(vfield<TFieldBits...> field, TMaskType mask) 
	{
		TField::impl_t::clear(mask);					
	}

	template<class... TFieldBits, 
		typename TField = vfield<TFieldBits...>,
		typename TMaskType = field_mask_type<TField>>
	static void toggle(vfield<TFieldBits...> field, TMaskType mask) 
	{
		TField::impl_t::toggle(mask);
	}
}

#endif