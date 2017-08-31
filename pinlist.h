/*
 * pinlist.h
 *
 * Created: 02.02.2013 15:21:51
 *  Author: astr0
 */ 

#pragma once

#ifndef PINLIST_H_
#define PINLIST_H_

#include "typelistutils.h"
#include "maskutils.h"
#include "pininfo.h"
#include "portinfo.h"

namespace fasthal
{
	namespace priv{
		// Holds pin and position in PinList
		template<int VPosition, class TPin>
		struct PinPositionHolder
		{
			typedef TPin Pin;
			static constexpr int Position = VPosition;
		};

		// Select Ports from pin list
		template<class TPinHolder>
		struct SelectPinPort
		{
			typedef typename PinInfo<typename TPinHolder::Pin>::PortType Result;
		};

		template<class TPinList>
		struct MakePortList
		{
			typedef typename fasthal::common::TL::Select<TPinList, SelectPinPort>::Result Result;
		};


		// Predicate<PinPositionHolder> - pins that match 1 to 1 with port pins
		template<class TPinHolder>
		struct PinPositionMatchPredicate
		{
			static constexpr bool value = TPinHolder::Position == TPinHolder::Pin::number();
		};

		// Predicate<PinPositionHolder> - inverted pins
		template<class TPinHolder>
		struct InvertedPinsPredicate
		{
			static constexpr bool value = TPinHolder::Pin::isInverted();
		};

		// Result - Predicate<PinPositionHolder> - pins that belongs to TPort Port
		template<class TPort>
		struct PortPinsPredicate
		{
			template<class TPinHolder>
			struct Result
			{
				static constexpr bool value = Loki::IsSameType<TPort, typename PinInfo<typename TPinHolder::Pin>::PortType>::value;
			};
		};


		//	Mask for inverted pins

		template <class TList, class MaskType> 
		struct GetInversionMask
		{
			template <class TPinHolder, MaskType VTail>
			struct Predicate
			{
				static constexpr MaskType value = (TPinHolder::Pin::isInverted() ? TPinHolder::Pin::mask() : 0) | VTail;
			};
			static constexpr MaskType Empty = MaskType();					
			static constexpr MaskType value = fasthal::common::TL::Aggregate<TList, MaskType, Empty, Predicate>::value;
		};

		// Ignore inversion mask
		template <class TList, class MaskType> 
		struct FalseInversionMask
		{
			static constexpr MaskType value = MaskType();			
		};


		//	Mask for port pins
		template <class TList, class MaskType>
		struct GetPortMask
		{
			template <class TPinHolder, MaskType VTail>
			struct Predicate
			{
				static constexpr MaskType value = TPinHolder::Pin::mask() | VTail;
			};					
			static constexpr MaskType Empty = MaskType();
			static constexpr MaskType value = fasthal::common::TL::Aggregate<TList, MaskType, Empty, Predicate>::value;
		};


		// Value bit mask for pin list
		template <class TList, class TValueType> struct GetValueMask;

		template <class TValueType> 
		struct GetValueMask<Loki::NullType, TValueType>
		{
			typedef fasthal::common::BitMaskTypes<TValueType> BM;
			typedef typename BM::MaskType MaskType;
			static constexpr MaskType value = MaskType();
		};

		template <class Head, class Tail, class TValueType>
		struct GetValueMask<Loki::Typelist<Head, Tail>, TValueType>
		{
			typedef fasthal::common::BitMaskTypes<TValueType> BM;
			typedef typename BM::MaskType MaskType;					
			static constexpr MaskType value = BM::bitToMask(Head::Position) | GetValueMask<Tail, TValueType>::value;
		};

		// Number of sequental pins in list
		template <class TList> struct GetSerialCount;

		template <> struct GetSerialCount<Loki::NullType>
		{
			static constexpr int value = 0;
			static constexpr int PinNumber = -1;
			static constexpr int BitPosition = -1;
		};
		template <class Head, class Tail>

		struct GetSerialCount< Loki::Typelist<Head, Tail> >
		{
			typedef GetSerialCount<Tail> I;
			static constexpr int PinNumber = Head::Pin::number();
			static constexpr int BitPosition = Head::Position;
			static constexpr int value =
				((PinNumber == I::PinNumber - 1 &&
				BitPosition == I::BitPosition - 1) ?
				I::value + 1 : 1);
		};

		// Iterates through pin list to compute value to write to their port
		// TODO: Refactor mask stuff to generate normal type, use |= rather than return result |
		template <class TList, class TDataType> struct PinWriteIterator;

		template <class TDataType> 
		class PinWriteIterator<Loki::NullType, TDataType>
		{
			typedef fasthal::common::BitMaskTypes<TDataType> MT;
			typedef typename MT::MaskType MaskType;
			
			public:
			
			template<class DataType, template<class, class> class InversionMask>
			static inline DataType appendValue(DataType value, DataType result)
			{
				return result;
			}
			
			static inline TDataType appendWriteValue(TDataType value, TDataType result)
			{
				return result;
			}

			static inline TDataType appendMaskValue(MaskType value, MaskType result)
			{
				return result;
			}

			static inline TDataType appendReadValue(TDataType portValue, TDataType result)
			{
				return result;
			}
			
			static void write(MaskType mask)
			{   }
		
			static void clearAndSet(MaskType clearMask, MaskType setMask)
			{	}

			static void set(MaskType mask)
			{   }

			static void clear(MaskType mask)
			{   }

			static void toggle(MaskType mask)
			{   }		
			
			static void setMode(MaskType mask, uint8_t mode)
			{   }	
		};

		template <class Head, class Tail, class TDataType>
		class PinWriteIterator<Loki::Typelist<Head, Tail>, TDataType>
		{
			typedef fasthal::common::BitMaskTypes<TDataType> MT;
			typedef typename MT::MaskType MaskType;
			typedef PinWriteIterator<Tail, TDataType> NextIterator;
			static constexpr typename MT::OneBitMask ListMask = MT::bitToMask(Head::Position);
			typedef typename Head::Pin Pin;
			typedef typename PinInfo<Pin>::PortType PortType;
			typedef PinWriteIterator<Tail, TDataType> Next;
			
			public:
			template<class DataType, template<class, class> class InversionMask>
			static inline DataType appendValue(DataType value, DataType result)
			{
				typedef Loki::Typelist<Head, Tail> CurrentList;
				typedef typename fasthal::common::TL::Where<CurrentList, PinPositionMatchPredicate>::Result TransparentPins;
				typedef typename fasthal::common::TL::Where<CurrentList, fasthal::common::TL::Not<PinPositionMatchPredicate>::Result>::Result NotTransparentPins;
				constexpr int TransparentCount = Loki::TL::Length<TransparentPins>::value;

				if(TransparentCount > 1)
				{
					result |= (value &
							GetPortMask<TransparentPins, DataType>::value) ^
							InversionMask<TransparentPins, DataType>::value;

					return PinWriteIterator<NotTransparentPins, TDataType>::template appendValue<DataType, InversionMask>(value, result);
				}

				constexpr int SerialLength = GetSerialCount<CurrentList>::value;

				if(SerialLength >= 2)
				{
					typedef typename fasthal::common::TL::TakeFirst<CurrentList, SerialLength>::Result SerialList;
					typedef typename fasthal::common::TL::SkipFirst<CurrentList, SerialLength>::Result RestList;

					result |= (fasthal::common::Shifter<Head::Pin::number() - Head::Position>::shift(value) & GetPortMask<SerialList, DataType>::value) ^ InversionMask<SerialList, DataType>::value;

					return PinWriteIterator<RestList, TDataType>::template appendValue<DataType, InversionMask>(value, result);
				}

				if(!(InversionMask<Loki::Typelist<Head, Loki::NullType>, DataType>::value))
				{
					if(value & ListMask)
						result |= Head::Pin::mask();
				}
				else
				{
					if(!(value & ListMask))
						result |= Head::Pin::mask();
				}

				return PinWriteIterator<Tail, TDataType>::template appendValue<DataType, InversionMask>(value, result);
			}
			
			static inline TDataType appendWriteValue(TDataType value, TDataType result)
			{
				return appendValue<TDataType, GetInversionMask>(value, result);
			}

			static inline TDataType appendMaskValue(MaskType value, MaskType result)
			{
				return appendValue<MaskType, FalseInversionMask>(value, result);
			}

			static inline TDataType appendReadValue(TDataType portValue, TDataType result)
			{
				typedef Loki::Typelist<Head, Tail> CurrentList;
				typedef typename fasthal::common::TL::Where<CurrentList, PinPositionMatchPredicate>::Result TransparentPins;
				typedef typename fasthal::common::TL::Where<CurrentList, fasthal::common::TL::Not<PinPositionMatchPredicate>::Result>::Result NotTransparentPins;
				constexpr int TransparentCount = Loki::TL::Length<TransparentPins>::value;

				if(TransparentCount > 1)
				{
					result |= (portValue &
							GetValueMask<TransparentPins, MaskType>::value) ^
							GetInversionMask<TransparentPins, TDataType>::value;

					return PinWriteIterator<NotTransparentPins, TDataType>::appendReadValue(portValue, result);
				}
				constexpr int SerialLength = GetSerialCount<CurrentList>::value;

				if(SerialLength >= 2)
				{
					typedef typename fasthal::common::TL::TakeFirst<CurrentList, SerialLength>::Result SerialList;
					typedef typename fasthal::common::TL::SkipFirst<CurrentList, SerialLength>::Result RestList;


					result |= fasthal::common::Shifter<Head::Position - Head::Pin::number()>::shift(portValue ^ GetInversionMask<SerialList, MaskType>::value) & GetValueMask<SerialList, MaskType>::value;
					return PinWriteIterator<RestList, TDataType>::appendReadValue(portValue, result);
				}

				if(GetInversionMask<Loki::Typelist<Head, Loki::NullType>, MaskType>::value)
				{
					if(!(portValue & Head::Pin::mask()))
						result |= ListMask;

				}else
				{
					if(portValue & Head::Pin::mask())
						result |= ListMask;
				}


				return PinWriteIterator<Tail, TDataType>::appendReadValue(portValue, result);
			}
			
			static void write(TDataType value)
			{
				Pin::set(value & ListMask);
				Next::write(value);
			}
			
			static void clearAndSet(MaskType clearMask, MaskType setMask)
			{				
				if (clearMask & ListMask)
				{
					if (!(setMask & ListMask))
						Pin::clear();
				} else if (setMask & ListMask)
					Pin::set();
				Next::clearAndSet(clearMask, setMask);								
			}

			static void set(MaskType mask)					
			{
				if (mask & ListMask)
					Pin::set();
				Next::set(mask);			
			}

			static void clear(MaskType mask)					
			{
				if (mask & ListMask)
					Pin::clear();
				Next::clear(mask);
			}

			static void toggle(MaskType mask)					
			{
				if (mask & ListMask)
					Pin::toggle(mask);
				Next::toggle(mask);						
			}		
			
			static void read(TDataType& result)
			{
				if (Pin::read())
					result |= ListMask;
				Next::read(result);
			}

			static void setMode(MaskType mask, uint8_t mode)					
			{
				if (mask & ListMask)
					Pin::setMode(mode);
				Next::setMode(mask, mode);			
			}
		};
		
		// Iterates through port list and write value to them
		// TODO: Fix DataType, MaskType and PinMaskType!!!
		//		 Add Read(PinMaskType) - performance!		
		
		template <class TPortList, class TPinList, class TValueType>
		struct PortListIterator;						
		template <class TPortList, class TPinList, class TValueType, bool VIsComplex>
		struct PortListIteratorImpl;

		template <class TPinList, class TValueType, bool VIsComplex> 
		class PortListIteratorImpl<Loki::NullType, TPinList, TValueType, VIsComplex>
		{
			typedef fasthal::common::BitMaskTypes<TValueType> BitMaskTypes;
			typedef typename BitMaskTypes::OneBitMask PinMaskType;
			typedef typename BitMaskTypes::BitNumberType PinNumberType;
			typedef typename BitMaskTypes::MaskType MaskType;
			public:					
			typedef TValueType DataType;

			static void write(DataType value)
			{   }
		
			static void clearAndSet(MaskType clearMask, MaskType setMask)
			{	}

			static void set(MaskType value)
			{   }

			static void clear(MaskType value)
			{   }

			static void toggle(MaskType value)
			{   }

			static DataType read()
			{
				return DataType();
			}

			static void setMode(MaskType value, uint8_t mode)
			{   }
		};
		
		template <class THead, class TTail, class TPinList, class TValueType>
		class PortListIteratorImpl< Loki::Typelist<THead, TTail>, TPinList, TValueType, true>
		{
			//pins on current port
			typedef typename fasthal::common::TL::Where<TPinList, PortPinsPredicate<THead>::template Result>::Result Pins;
			static constexpr int PinsLength = Loki::TL::Length<Pins>::value;
	
			typedef typename fasthal::common::TL::Where<Pins, InvertedPinsPredicate>::Result InvertedPins;
			static constexpr int InvertedPinsLength = Loki::TL::Length<InvertedPins>::value;
	
			typedef typename fasthal::common::TL::Where<Pins, fasthal::common::TL::Not<InvertedPinsPredicate>::Result>::Result NotInvertedPins;
			static constexpr int NotInvertedPinsLength = Loki::TL::Length<NotInvertedPins>::value;

			typedef THead Port; //Head points to current port in the list.
			typedef typename PortInfo<Port>::DataType PortType;
			typedef typename PortInfo<Port>::MaskType PortMaskType;
			static constexpr PortMaskType Mask = GetPortMask<Pins, PortMaskType>::value;

			typedef typename Loki::Select<sizeof(PortType) >= sizeof(TValueType), PortType, TValueType>::Result DataType;
			
			typedef fasthal::common::BitMaskTypes<DataType> BitMaskTypes;
			typedef typename BitMaskTypes::OneBitMask PinMaskType;
			typedef typename BitMaskTypes::BitNumberType PinNumberType;
			typedef typename BitMaskTypes::MaskType MaskType;
			
			typedef PinWriteIterator<Pins, DataType> PinWrite;
			typedef PortListIterator<TTail, TPinList, TValueType> Next;
			
			public:

			static void write(DataType value)
			{
				// Apply inversion mask on value
				DataType result = PinWrite::appendWriteValue(value, DataType(0));

				if(PinsLength == (int)Port::width())// whole port
					Port::write(result);
				else
				{
					Port::clearAndSet(Mask, result);
				}

				Next::write(value);
			}

			static void clearAndSet(MaskType clearMask, MaskType setMask)
			{
				MaskType resultC = PinWrite::appendMaskValue(clearMask, MaskType());
				MaskType resultS = PinWrite::appendMaskValue(setMask, MaskType());
				if (NotInvertedPinsLength == PinsLength)
				{
					// All not inverted
					Port::clearAndSet(resultC, resultS);
				} else if (InvertedPinsLength == PinsLength)
				{
					// All inverted
					Port::clearAndSet(resultS, resultC);				
				} else
				{
					// Mix... calculate set and clear masks...
					constexpr MaskType inversionMask = GetInversionMask<Pins, MaskType>::value;
					// clear - not inverted clear + inverted set
					DataType clearMask = (resultC & ~inversionMask) | (resultS & inversionMask);
					// set - not inverted set + inverted clear
					DataType setMask = (resultS & ~inversionMask) | (resultC & inversionMask);
					Port::clearAndSet(clearMask, setMask);					
				}

				Next::clearAndSet(clearMask, setMask);
			}

			static void set(MaskType value)
			{
				// Invert inverted
				//static_assert(1 == PinsLength, "All pins are normal");
				if (NotInvertedPinsLength == PinsLength)
				{
					MaskType result = PinWrite::appendMaskValue(value, MaskType());
					Port::set(result);					
				} else if (InvertedPinsLength == PinsLength)
				{
					MaskType result = PinWrite::appendMaskValue(value, MaskType());
					Port::clear(result);					
				} else
				{
					MaskType clearMask = PinWriteIterator<InvertedPins, DataType>::appendMaskValue(value, MaskType());
					MaskType setMask = PinWriteIterator<NotInvertedPins, DataType>::appendMaskValue(value, MaskType());
					Port::clearAndSet(clearMask, setMask);
				}

				Next::set(value);
			}

			static void clear(MaskType value)
			{
				// Invert inverted
				if (NotInvertedPinsLength == PinsLength)
				{
					MaskType result = PinWrite::appendMaskValue(value, MaskType());
					Port::clear(result);
				} else if (InvertedPinsLength == PinsLength)
				{
					MaskType result = PinWrite::appendMaskValue(value, MaskType());
					Port::set(result);
				} else
				{
					MaskType clearMask = PinWriteIterator<NotInvertedPins, DataType>::appendMaskValue(value, MaskType());
					MaskType setMask = PinWriteIterator<InvertedPins, DataType>::appendMaskValue(value, MaskType());
					Port::clearAndSet(clearMask, setMask);
				}

				Next::clear(value);
			}

			static void toggle(MaskType value)
			{
				// Ignore inverted - toggle does not care
				MaskType result = PinWrite::appendMaskValue(value, MaskType());
				Port::toggle(result);

				Next::toggle(value);
			}
	
			static DataType read()
			{
				// Apply inversion mask on value
				DataType portValue = Port::read();
				return PinWrite::appendReadValue(portValue, Next::read());
			}

			static void setMode(MaskType mask, uint8_t mode)
			{
				MaskType result = PinWrite::appendMaskValue(mask, MaskType());
				Port::setMode(result, mode);

				Next::setMode(mask, mode);
			}
		};				


		template <class THead, class TTail, class TPinList, class TValueType>
		class PortListIteratorImpl< Loki::Typelist<THead, TTail>, TPinList, TValueType, false>
		{
			//pins on current port
			typedef typename fasthal::common::TL::Where<TPinList, PortPinsPredicate<THead>::template Result>::Result Pins;

			typedef THead Port; //Head points to current port in the list.

			typedef TValueType DataType;
			
			typedef fasthal::common::BitMaskTypes<DataType> BitMaskTypes;
			typedef typename BitMaskTypes::OneBitMask PinMaskType;
			typedef typename BitMaskTypes::BitNumberType PinNumberType;
			typedef typename BitMaskTypes::MaskType MaskType;
			typedef PinWriteIterator<Pins, DataType> PinWrite;
			typedef PortListIterator<TTail, TPinList, TValueType> Next;
			
			public:

			static void write(DataType value)
			{
				PinWrite::write(value);						

				Next::write(value);
			}

			static void clearAndSet(MaskType clearMask, MaskType setMask)
			{
				PinWrite::clearAndSet(clearMask, setMask);

				Next::clearAndSet(clearMask, setMask);
			}

			static void set(MaskType value)
			{
				PinWrite::set(value);

				Next::set(value);
			}

			static void clear(MaskType value)
			{
				PinWrite::clear(value);

				Next::clear(value);
			}

			static void toggle(MaskType value)
			{
				PinWrite::toggle(value);

				Next::toggle(value);
			}
			
			static DataType read()
			{
				DataType result;
				PinWrite::read(result);
				return result;
			}
		
			static void setMode(MaskType value, uint8_t mode)
			{
				PinWrite::setMode(value, mode);

				Next::setMode(value, mode);
			}
		};

		template<class PortList, class ValueType>
		struct IsComplexPortListImpl
		{
			typedef typename PortList::Head PortType;
			static constexpr bool value = !PortInfo<PortType>::OnlyPinInterface && !fasthal::common::BitMaskTypes<ValueType>::OnlyBitInterface;
		};

		template<class ValueType>
		struct IsComplexPortListImpl<Loki::NullType, ValueType>
		{
			static constexpr bool value = false;
		};

		template <class PortList, class PinList, class ValueType> 
		struct PortListIterator: public PortListIteratorImpl<PortList, PinList, ValueType, IsComplexPortListImpl<PortList, ValueType>::value>
		{
		};


		// Creates pin list from passed pins.
		template<typename ...TPins>
		struct MakePinList
		{
			typedef typename fasthal::common::TL::MakeList<0, PinPositionHolder, TPins...>::Result Result;
		};
	}
}

#endif /* PINLIST_H_ */