#ifndef FH_VFIELD_H_
#define FH_VFIELD_H_

#include "fieldbit.hpp"
#include "fieldbits.hpp"
#include "../sys/typelistutils.h"
//#include "../utils/bitholder.h"
#include "../mp/brigand.hpp"
#include "vfield_impl.hpp"


namespace fasthal{
    // vfield - static field-like wrapper for FieldBits
	template<class... TFieldBits>
	struct vfield
	{
		// TODO: Static assert that all TFieldBits are field bits		


		#ifdef VFIELD_OLD
		using fieldbits_t = typename fasthal::priv::MakeFieldBitList<TFieldBits...>::Result ;
		using fields_t = typename Loki::TL::NoDuplicates<typename priv::MakeFieldList<fieldbits_t>::Result>::Result;            
		using datatype_t = bytes_bitmask_type<maskSizeInBytes(brigand::count<TFieldBits...>::value)>;
		using impl_t = priv::FieldListIterator<fields_t, fieldbits_t, datatype_t>; 

		static void write(datatype_t value) 
		{
			impl_t::write(value);
		}

		static datatype_t read() 
		{
			return impl_t::read();
		}
		
		#else

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

		#endif
	};

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
	static void clearAndSet(vfield<TFieldBits...> field, TMaskType clearMask, TMaskType setMask) 
	{
		TField::impl_t::clearAndSet(clearMask, setMask);
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