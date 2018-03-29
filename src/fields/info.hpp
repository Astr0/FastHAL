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
	 template<class TPin>
     struct PinInfo
     {
         typedef TPin PinType;
         typedef decltype(fasthal::common::declval<TPin>().port()) PortType;
         typedef decltype(fasthal::common::declval<TPin>().mask()) PinMaskType;
         typedef decltype(fasthal::common::declval<TPin>().number()) PinNumberType;
     };

	template<class TField>
	struct PortInfo
	{
		typedef TField PortType;
		typedef decltype(fasthal::common::declval<TField>().read()) DataType;
		private:
		typedef fasthal::common::BitMaskTypes<DataType> MT;
		public:
		typedef typename MT::OneBitMask PinMaskType;
		typedef typename MT::BitNumberType PinNumberType;
		typedef typename MT::MaskType MaskType;	
		static constexpr bool OnlyPinInterface = MT::OnlyBitInterface;
		static constexpr PinMaskType getPinMask(PinNumberType number){return MT::bitToMask(number);}						
		static constexpr PinNumberType getPinNumber(PinMaskType pinMask){return MT::maskToBit(pinMask);}
	};
}



#endif /* PORTINFO_H_ */