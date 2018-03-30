// /*
//  * bitholder.h
//  *
//  * Created: 06.02.2013 0:38:31
//  *  Author: astr0
//  */ 


// #ifndef BITHOLDER_H_
// #define BITHOLDER_H_

// #include "mask.hpp"
// #include "../mp/brigand_ex.hpp"

// namespace fasthal
// {
// 	// TODO: Fix/remove/refactor
// 	namespace common
// 	{
// 		template<class TNumber, bool VNot>
// 		class BitNumber
// 		{	
// 			private:
// 				const TNumber _number;
				
// 				using bit_number_t = BitNumber<TNumber, VNot>;
// 			public:
// 				constexpr BitNumber():_number{}
// 				{
			
// 				}
// 				constexpr BitNumber(TNumber number):_number{number}
// 				{
// 				}
		
// 				constexpr auto number(){return _number;}
		
// 				template<class T>
// 				constexpr auto operator<<(T val){return bit_number_t{number() + val};}
// 				template<class T>
// 				constexpr auto operator>>(T val){return bit_number_t{number() - val};}
		
// 				template<class T>
// 				bit_number_t& operator<<=(T val){_number += val; return *this;}
// 				template<class T>
// 				bit_number_t& operator>>=(T val){_number -= val; return *this;}
		
// 				constexpr auto operator~(){ return BitNumber<TNumber, !VNot>{_number};}
// 		};
		
// 		template<unsigned N>
// 		class BitHolder
// 		{
// 			private:
// 				static constexpr unsigned maxBitIndex = N * 8;
// 				static constexpr unsigned byteSize = N;

// 				using bit_holder_t = BitHolder<N>;
// 			public:
// 				uint8_t _buf[byteSize];
		
// 				using byte_number_t = brigand::number_type<N>;
			
// 				// constexpr uint8_t operator[](byte_number_t number){return _buf[number];}
// 				// uint8_t& operator[](byte_number_t number){return _buf[number];}
		
// 			public:
// 				using bit_number_t = brigand::number_type<maxBitIndex>;
// 				using masktype_t = bit_holder_t;
// 				using bit_masktype_t = BitNumber<byte_number_t, false>;


// 				template<class Num>
// 				inline bit_holder_t& operator|=(const BitNumber<Num, false> num)
// 				{
// 					_buf[num.number() / 8] |= 1 << (num.number() % 8);
// 					return *this;
// 				}

// 				template<class Num>
// 				inline bit_holder_t& operator^=(const BitNumber<Num, false> num)
// 				{
// 					_buf[num.number() / 8] ^= 1 << (num.number() % 8);
// 					return *this;
// 				}

// 				template<class Num>
// 				inline bit_holder_t& operator&=(const BitNumber<Num, true> num)
// 				{
			
// 					_buf[num.number() / 8] &= ~(1 << (num.number() % 8));
// 					return *this;
// 				}
		
// 				template<class Num>
// 				constexpr inline bool operator&(const BitNumber<Num, false> num)const
// 				{
// 					return _buf[num.number() / 8] & (1 << (num.number() % 8));
// 				}
				
// 			private:
// 				bool isTruthy()const
// 				{
// 					for (bit_number_t i = 0; i < N; ++i)
// 						if (_buf[i])
// 							return true;
// 					return false;					
// 				}
// 			public:
				
// 				constexpr operator bool() const
// 				{
// 					return isTruthy();
// 				}
// 				constexpr bool operator !()const
// 				{
// 					return !isTruthy();
// 				}				
// 		};
		
		
// 	}

// 	namespace details{
// 		// set this as fallback for large bit masks
// 		template<unsigned bytes>
// 		struct bytes_bitmask_type_impl
// 		{
// 			using type = common::BitHolder<bytes>;
// 		};
// 	}
			
// 	// bitmask type info
// 	template<unsigned N>
// 	struct bitmask_types<common::BitHolder<N>>
// 	{
// 		using MaskType = common::BitHolder<N>;
// 		using OneBitMask = typename MaskType::bit_masktype_t;
// 		using BitNumberType = typename MaskType::bit_number_t;
// 		static constexpr bool OnlyBitInterface = true;
// 		static constexpr OneBitMask bitToMask(BitNumberType num){return OneBitMask() << num; }
// 		static constexpr BitNumberType maskToBit(OneBitMask value)
// 		{
// 			return value.number();
// 		}					
// 	};
// }



// #endif /* BITHOLDER_H_ */