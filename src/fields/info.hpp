/*
 * portinfo.h
 *
 * Created: 06.02.2013 0:58:06
 *  Author: astr0
 */ 


#ifndef FH_FIELDS_INFO_H_
#define FH_FIELDS_INFO_H_

#include "../sys/maskutils.h"
#include "../sys/typeutils.h"

namespace fasthal
{
	template<class TField>
	struct FieldInfo
	{
		typedef TField FieldType;
		typedef decltype(fasthal::common::declval<TField>().read()) DataType;
		private:
		typedef fasthal::common::BitMaskTypes<DataType> MT;
		public:
		typedef typename MT::OneBitMask BitMaskType;
		typedef typename MT::BitNumberType BitNumberType;
		typedef typename MT::MaskType MaskType;	
		static constexpr bool OnlyPinInterface = MT::OnlyBitInterface;
		static constexpr BitMaskType getPinMask(BitNumberType number){return MT::bitToMask(number);}						
		static constexpr BitNumberType getPinNumber(BitMaskType pinMask){return MT::maskToBit(pinMask);}
	};

	//  template<class TFieldBit>
    //  struct FieldBitInfo{ };

	//  template<class TField, unsigned VNumber, bool VInverted>
	//  struct FieldBitInfo<FieldBit<TField, VNumber, VInterted>
    //  {
    //      typedef TFieldBit PinType;
    //      typedef decltype(fasthal::common::declval<TFieldBit>().port()) FieldType;
    //      typedef decltype(fasthal::common::declval<TFieldBit>().mask()) BitMaskType;
    //      typedef decltype(fasthal::common::declval<TFieldBit>().number()) BitNumberType;
    //  };
}



#endif /* PORTINFO_H_ */