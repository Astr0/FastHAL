/*
 * maskutils.h
 *
 * Created: 03.02.2013 20:58:09
 *  Author: astr0
 */ 
#pragma once

#ifndef FH_UTILS_MASK_H_
#define FH_UTILS_MASK_H_

#include "../mp/std_types.hpp"
#include "../mp/type_traits.hpp"
#include "../mp/brigand_ex.hpp"


namespace fasthal
{
	namespace common
	{
		// min bytes to hold bits
		constexpr unsigned minSizeInBytes(unsigned sizeInBits)
		{
			return sizeInBits == 0 ? 1 : ((sizeInBits / 8)  + ((sizeInBits % 8) ? 1 : 0));
		}
		
		// template<class MaskType>
		// constexpr MaskType setBits(int count)
		// {
		// 	return count == 0 ? 0 : ((1 << count) | setBits<MaskType>(count - 1));
		// }
				
		// TODO: Refactor this
		template<unsigned bytes>
		struct BitMaskType;
		
		template<>
		struct BitMaskType<1>
		{
			typedef uint_fast8_t Result;
		};
		template<>
		struct BitMaskType<2>
		{
			typedef uint_fast16_t Result;
		};
		template<>
		struct BitMaskType<3>
		{
			typedef uint_fast32_t Result;
		};
		template<>
		struct BitMaskType<4>
		{
			typedef uint_fast32_t Result;
		};		
		
		// and this
		template<class DataType>
		struct BitMaskTypes
		{
			using MaskType = DataType;
			using OneBitMask = DataType;
			using BitNumberType = brigand::number_type<sizeof(DataType) * 8>;
			static constexpr bool OnlyBitInterface = false;
			static constexpr OneBitMask bitToMask(BitNumberType num){return OneBitMask { 1 } << num; }
			static constexpr BitNumberType maskToBit(OneBitMask value)
			{
				return (value & 1) ? 0 : (1 + maskToBit(value >> 1));
			}
		};
		
		template<int VBytesToLeft, class T>
		static constexpr T shift(T value)
		{
			return VBytesToLeft == 0 
				? value 
				: (VBytesToLeft > 0 ? value << VBytesToLeft : value >> -VBytesToLeft);
		}
	}
}

#endif /* MASKUTILS_H_ */