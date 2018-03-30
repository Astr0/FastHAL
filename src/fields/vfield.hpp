#ifndef FH_VFIELD_H_
#define FH_VFIELD_H_

#include "fieldbit.hpp"
#include "fieldbits.hpp"
#include "../sys/typelistutils.h"
#include "../utils/bitholder.h"
#include "../mp/brigand.hpp"


namespace fasthal{
    // VField - static field-like wrapper for FieldBits
	template<class... TFieldBits>
	class VField
	{
		private:
			using fieldbits_t = typename fasthal::priv::MakeFieldBitList<TFieldBits...>::Result ;
			using fields_t = typename Loki::TL::NoDuplicates<typename priv::MakeFieldList<fieldbits_t>::Result>::Result;			
			using datatype_t = bytes_bitmask_type<maskSizeInBytes(brigand::count<TFieldBits...>::value)>;
		public:		
			using impl_t = priv::FieldListIterator<fields_t, fieldbits_t, datatype_t>; 

			static void write(datatype_t value) 
			{
				impl_t::write(value);
			}

			static datatype_t read() 
			{
				return impl_t::read();
			}
	};

	// create VField
	template<class... TBits>
	constexpr decltype(auto) vfield(TBits... bits){
		return VField<TBits...>{};
	}

	// type info
	template <class... TFieldBits>
	struct field_width<VField<TFieldBits...>>: brigand::count<TFieldBits...>{};

	// create VField fieldBit - return passed field bit
	template<unsigned VNumber, class... TFieldBits>
	constexpr decltype(auto) fieldBit(VField<TFieldBits...> field){
		return brigand::at_c<brigand::list<TFieldBits...>, VNumber> {};
	}

	// optimized actions
    template<class... TFieldBits, 
		typename Field = VField<TFieldBits...>,
		typename MaskType = field_mask_type<Field>>
	static void clearAndSet(VField<TFieldBits...> field, MaskType clearMask, MaskType setMask) 
	{
		Field::impl_t::clearAndSet(clearMask, setMask);
	}				

	template<class... TFieldBits, 
		typename Field = VField<TFieldBits...>,
		typename MaskType = field_mask_type<Field>>
	static void set(VField<TFieldBits...> field, MaskType mask) 
	{
		Field::impl_t::set(mask);
	}
	template<class... TFieldBits, 
		typename Field = VField<TFieldBits...>,
		typename MaskType = field_mask_type<Field>>
	static void clear(VField<TFieldBits...> field, MaskType mask) 
	{
		Field::impl_t::clear(mask);					
	}

	template<class... TFieldBits, 
		typename Field = VField<TFieldBits...>,
		typename MaskType = field_mask_type<Field>>
	static void toggle(VField<TFieldBits...> field, MaskType mask) 
	{
		Field::impl_t::toggle(mask);
	}
}

#endif