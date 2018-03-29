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
			using FieldBits = typename fasthal::priv::MakeFieldBitList<TFieldBits...>::Result ;
			using Fields = typename Loki::TL::NoDuplicates<typename priv::MakeFieldList<FieldBits>::Result>::Result ;
			
			static constexpr auto BitCount = brigand::count<TFieldBits...>::value;
			static constexpr auto ByteSize = fasthal::common::minSizeInBytes(BitCount);
			
			using DataType = typename fasthal::common::BitMaskType<ByteSize>::Result;
		public:		
			using Impl = priv::FieldListIterator<Fields, FieldBits, DataType>; 
			// port impl

			static void write(DataType value) 
			{
				Impl::write(value);
			}

			static DataType read() 
			{
				return Impl::read();
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
		Field::Impl::clearAndSet(clearMask, setMask);
	}				

	template<class... TFieldBits, 
		typename Field = VField<TFieldBits...>,
		typename MaskType = field_mask_type<Field>>
	static void set(VField<TFieldBits...> field, MaskType mask) 
	{
		Field::Impl::set(mask);
	}
	template<class... TFieldBits, 
		typename Field = VField<TFieldBits...>,
		typename MaskType = field_mask_type<Field>>
	static void clear(VField<TFieldBits...> field, MaskType mask) 
	{
		Field::Impl::clear(mask);					
	}

	template<class... TFieldBits, 
		typename Field = VField<TFieldBits...>,
		typename MaskType = field_mask_type<Field>>
	static void toggle(VField<TFieldBits...> field, MaskType mask) 
	{
		Field::Impl::toggle(mask);
	}
}

#endif