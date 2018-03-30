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
	// shift value by VBytes
	template<int VBytesToLeft, class T>
	static constexpr T shift(T value)
	{
		return VBytesToLeft == 0 
			? value 
			: (VBytesToLeft > 0 ? value << VBytesToLeft : value >> -VBytesToLeft);
	}

	// min bytes to hold bits
	constexpr unsigned maskSizeInBytes(unsigned sizeInBits)
	{
		return sizeInBits == 0 ? 1 : ((sizeInBits / 8)  + ((sizeInBits % 8) ? 1 : 0));
	}

	// template<class MaskType>
	// constexpr MaskType setBits(int count)
	// {
	// 	return count == 0 ? 0 : ((1 << count) | setBits<MaskType>(count - 1));
	// }

	// bit mask type that holds N bytes of bits
	namespace details{
		template<unsigned bytes>
		struct bytes_bitmask_type_impl;
		
		template<>
		struct bytes_bitmask_type_impl<1>
		{
			using type = std::uint8_t;
		};
		template<>
		struct bytes_bitmask_type_impl<2>
		{
			using type = std::uint16_t;
		};
		template<>
		struct bytes_bitmask_type_impl<3>
		{
			using type = std::uint32_t;
		};
		template<>
		struct bytes_bitmask_type_impl<4>
		{
			using type = std::uint32_t;
		};		

		// define fallback implementation in user code
	}

	template<unsigned bytes>
	using bytes_bitmask_type = typename details::bytes_bitmask_type_impl<bytes>::type;
		
	// TODO: Refactor this
	template<class DataType>
	struct bitmask_types
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

	template <class DataType>
	using bitmask_type = typename bitmask_types<DataType>::MaskType;
}

#endif /* MASKUTILS_H_ */