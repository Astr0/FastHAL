/*
 * portinfo.h
 *
 * Created: 06.02.2013 0:58:06
 *  Author: astr0
 */ 


#ifndef PORTINFO_H_
#define PORTINFO_H_

#include "maskutils.h"
#include "typeutils.h"

namespace fasthal
{
	template<class TPort>
	struct PortInfo
	{
		typedef TPort PortType;
		typedef decltype(fasthal::common::declval<TPort>().read()) DataType;
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