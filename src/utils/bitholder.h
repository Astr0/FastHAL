/*
 * bitholder.h
 *
 * Created: 06.02.2013 0:38:31
 *  Author: astr0
 */ 


#ifndef BITHOLDER_H_
#define BITHOLDER_H_

#include "mask.hpp"
#include "../mp/brigand_ex.hpp"

namespace fasthal
{
	namespace common
	{
		template<class TNumber>
		class NotBitNumber;

		template<class TNumber>
		class BitNumber
		{	
			private:
				const TNumber _number;
			public:
				constexpr BitNumber():_number()
				{
			
				}
				constexpr BitNumber(TNumber number):_number(number)
				{
				}
		
				constexpr TNumber number(){return _number;}
		
				template<class T>
				constexpr BitNumber<TNumber> operator<<(T val){return BitNumber<TNumber>(number() + val);}
				template<class T>
				constexpr BitNumber<TNumber> operator>>(T val){return BitNumber<TNumber>(number() - val);}
		
				template<class T>
				BitNumber<TNumber>& operator<<=(T val){_number += val; return *this;}
				template<class T>
				BitNumber<TNumber>& operator>>=(T val){_number -= val; return *this;}
		
				NotBitNumber<TNumber> operator~(){ return NotBitNumber<TNumber>(_number);}
		};

		template<class TNumber>
		class NotBitNumber: public BitNumber<TNumber>
		{
			private:
				const TNumber _number;
			public:
				constexpr NotBitNumber():_number()
				{
			
				}
				constexpr NotBitNumber(TNumber number):_number(number)
				{
				}
		
				constexpr TNumber number(){return _number;}
		
				template<class T>
				constexpr NotBitNumber<TNumber> operator<<(T val){return NotBitNumber<TNumber>(number() + val);}
				template<class T>
				constexpr NotBitNumber<TNumber> operator>>(T val){return NotBitNumber<TNumber>(number() - val);}
		
				template<class T>
				NotBitNumber<TNumber>& operator<<=(T val){_number += val; return *this;}
				template<class T>
				NotBitNumber<TNumber>& operator>>=(T val){_number -= val; return *this;}
		
				BitNumber<TNumber> operator~(){ return BitNumber<TNumber>(_number);}
		};	

		template<unsigned N>
		class BitHolder
		{
			private:
				static constexpr unsigned MaxBitIndex = N * 8;
				static constexpr unsigned ByteSize = N;
			public:
				uint8_t _buf[ByteSize];
		
				using ByteNumberType = brigand::number_type<N>;
			
				constexpr uint8_t operator[](ByteNumberType number){return _buf[number];}
				uint8_t& operator[](ByteNumberType number){return _buf[number];}
		
			public:
				using BitNumberType = brigand::number_type<MaxBitIndex>;
				using MaskType = BitHolder<N>;
				using BitMaskType = BitNumber<BitNumberType>;


				template<class Num>
				inline BitHolder<N>& operator|=(const BitNumber<Num> num)
				{
					_buf[num.number() / 8] |= 1 << (num.number() % 8);
					return *this;
				}

				template<class Num>
				inline BitHolder<N>& operator^=(const BitNumber<Num> num)
				{
					_buf[num.number() / 8] ^= 1 << (num.number() % 8);
					return *this;
				}

				template<class Num>
				inline BitHolder<N>& operator&=(const NotBitNumber<Num> num)
				{
			
					_buf[num.number() / 8] &= ~(1 << (num.number() % 8));
					return *this;
				}
		
				template<class Num>
				constexpr inline bool operator&(const BitNumber<Num> num)const
				{
					return _buf[num.number() / 8] & (1 << (num.number() % 8));
				}
				
			private:
				bool isTruthy()const
				{
					for (ByteNumberType i = 0; i < N; ++i)
						if (_buf[i])
							return true;
					return false;					
				}
			public:
				
				constexpr operator bool() const
				{
					return isTruthy();
				}
				constexpr bool operator !()const
				{
					return !isTruthy();
				}				
		};
		
		
	}

	namespace details{
		// set this as fallback for large bit masks
		template<unsigned bytes>
		struct bytes_bitmask_type_impl
		{
			using type = common::BitHolder<bytes>;
		};
	}
			
	// bitmask type info
	template<unsigned N>
	struct bitmask_types<common::BitHolder<N>>
	{
		using MaskType = common::BitHolder<N>;
		using OneBitMask = typename MaskType::BitMaskType;
		using BitNumberType = typename MaskType::BitNumberType;
		static constexpr bool OnlyBitInterface = true;
		static constexpr OneBitMask bitToMask(BitNumberType num){return OneBitMask() << num; }
		static constexpr BitNumberType maskToBit(OneBitMask value)
		{
			return value.number();
		}					
	};
}



#endif /* BITHOLDER_H_ */