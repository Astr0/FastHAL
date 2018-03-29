/*
 * pinlist.h
 *
 * Created: 02.02.2013 15:21:51
 *  Author: astr0
 */ 

#pragma once

#ifndef FH_FIELDBITS_H_
#define FH_FIELDBITS_H_

#include "../sys/typelistutils.h"
#include "../sys/maskutils.h"
#include "info.hpp"

namespace fasthal
{
	namespace priv{
		// Holds pin and position in FieldBitList
		template<int VPosition, class TFieldBit>
		struct BitPositionHolder
		{
			typedef TFieldBit FieldBit;
			static constexpr int Position = VPosition;
		};

		// Select Fields from pin list
		template<class TFieldBitHolder>
		struct SelectBitHolderField
		{
			using Result = typename TFieldBitHolder::FieldBit::Field;
		};

		template<class TFieldBitList>
		struct MakeFieldList
		{
			typedef typename fasthal::common::TL::Select<TFieldBitList, SelectBitHolderField>::Result Result;
		};


		// Predicate<BitPositionHolder> - pins that match 1 to 1 with Field pins
		template<class TFieldBitHolder>
		struct PinPositionMatchPredicate
		{
			static constexpr bool value = TFieldBitHolder::Position == TFieldBitHolder::FieldBit::Number;
		};

		// Predicate<BitPositionHolder> - inverted pins
		template<class TFieldBitHolder>
		struct InvertedFieldBitsPredicate
		{
			static constexpr bool value = TFieldBitHolder::FieldBit::Inverted;
		};

		// Result - Predicate<BitPositionHolder> - pins that belongs to TField Field
		template<class TField>
		struct FieldFieldBitsPredicate
		{
			template<class TFieldBitHolder>
			struct Result
			{
				static constexpr bool value = Loki::IsSameType<TField, typename SelectBitHolderField<TFieldBitHolder>::Result>::value;
			};
		};


		//	Mask for inverted pins

		template <class TList, class MaskType> 
		struct GetInversionMask
		{
			template <class TFieldBitHolder, MaskType VTail>
			struct Predicate
			{
				static constexpr MaskType value = (TFieldBitHolder::FieldBit::Inverted ? TFieldBitHolder::FieldBit::Mask : 0) | VTail;
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


		//	Mask for Field pins
		template <class TList, class MaskType>
		struct GetFieldMask
		{
			template <class TFieldBitHolder, MaskType VTail>
			struct Predicate
			{
				static constexpr MaskType value = TFieldBitHolder::FieldBit::Mask | VTail;
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
			static constexpr int PinNumber = Head::FieldBit::Number;
			static constexpr int BitPosition = Head::Position;
			static constexpr int value =
				((PinNumber == I::PinNumber - 1 &&
				BitPosition == I::BitPosition - 1) ?
				I::value + 1 : 1);
		};

		// Iterates through pin list to compute value to write to their Field
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

			static inline TDataType appendReadValue(TDataType FieldValue, TDataType result)
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
			typedef typename Head::FieldBit FieldBit;
			//typedef typename FieldBitInfo<FieldBit>::FieldType FieldType;
			typedef PinWriteIterator<Tail, TDataType> Next;
			
			public:
			template<class DataType, template<class, class> class InversionMask>
			static inline DataType appendValue(DataType value, DataType result)
			{
				typedef Loki::Typelist<Head, Tail> CurrentList;
				typedef typename fasthal::common::TL::Where<CurrentList, PinPositionMatchPredicate>::Result TransparentFieldBits;
				typedef typename fasthal::common::TL::Where<CurrentList, fasthal::common::TL::Not<PinPositionMatchPredicate>::Result>::Result NotTransparentFieldBits;
				constexpr int TransparentCount = Loki::TL::Length<TransparentFieldBits>::value;

				if(TransparentCount > 1)
				{
					result |= (value &
							GetFieldMask<TransparentFieldBits, DataType>::value) ^
							InversionMask<TransparentFieldBits, DataType>::value;

					return PinWriteIterator<NotTransparentFieldBits, TDataType>::template appendValue<DataType, InversionMask>(value, result);
				}

				constexpr int SerialLength = GetSerialCount<CurrentList>::value;

				if(SerialLength >= 2)
				{
					typedef typename fasthal::common::TL::TakeFirst<CurrentList, SerialLength>::Result SerialList;
					typedef typename fasthal::common::TL::SkipFirst<CurrentList, SerialLength>::Result RestList;

					result |= (fasthal::common::Shifter<FieldBit::Number - Head::Position>::shift(value) & GetFieldMask<SerialList, DataType>::value) ^ InversionMask<SerialList, DataType>::value;

					return PinWriteIterator<RestList, TDataType>::template appendValue<DataType, InversionMask>(value, result);
				}

				if(!(InversionMask<Loki::Typelist<Head, Loki::NullType>, DataType>::value))
				{
					if(value & ListMask)
						result |= FieldBit::Mask;
				}
				else
				{
					if(!(value & ListMask))
						result |= FieldBit::Mask;
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

			static inline TDataType appendReadValue(TDataType FieldValue, TDataType result)
			{
				typedef Loki::Typelist<Head, Tail> CurrentList;
				typedef typename fasthal::common::TL::Where<CurrentList, PinPositionMatchPredicate>::Result TransparentFieldBits;
				typedef typename fasthal::common::TL::Where<CurrentList, fasthal::common::TL::Not<PinPositionMatchPredicate>::Result>::Result NotTransparentFieldBits;
				constexpr int TransparentCount = Loki::TL::Length<TransparentFieldBits>::value;

				if(TransparentCount > 1)
				{
					result |= (FieldValue &
							GetValueMask<TransparentFieldBits, MaskType>::value) ^
							GetInversionMask<TransparentFieldBits, TDataType>::value;

					return PinWriteIterator<NotTransparentFieldBits, TDataType>::appendReadValue(FieldValue, result);
				}
				constexpr int SerialLength = GetSerialCount<CurrentList>::value;

				if(SerialLength >= 2)
				{
					typedef typename fasthal::common::TL::TakeFirst<CurrentList, SerialLength>::Result SerialList;
					typedef typename fasthal::common::TL::SkipFirst<CurrentList, SerialLength>::Result RestList;


					result |= fasthal::common::Shifter<Head::Position - Head::FieldBit::Number>::shift(FieldValue ^ GetInversionMask<SerialList, MaskType>::value) & GetValueMask<SerialList, MaskType>::value;
					return PinWriteIterator<RestList, TDataType>::appendReadValue(FieldValue, result);
				}

				if(GetInversionMask<Loki::Typelist<Head, Loki::NullType>, MaskType>::value)
				{
					if(!(FieldValue & Head::FieldBit::Mask))
						result |= ListMask;

				}else
				{
					if(FieldValue & Head::FieldBit::Mask)
						result |= ListMask;
				}


				return PinWriteIterator<Tail, TDataType>::appendReadValue(FieldValue, result);
			}
			
			static void write(TDataType value)
			{
				FieldBit::set(value & ListMask);
				Next::write(value);
			}
			
			static void clearAndSet(MaskType clearMask, MaskType setMask)
			{				
				if (clearMask & ListMask)
				{
					if (!(setMask & ListMask))
						FieldBit::clear();
				} else if (setMask & ListMask)
					FieldBit::set();
				Next::clearAndSet(clearMask, setMask);								
			}

			static void set(MaskType mask)					
			{
				if (mask & ListMask)
					FieldBit::set();
				Next::set(mask);			
			}

			static void clear(MaskType mask)					
			{
				if (mask & ListMask)
					FieldBit::clear();
				Next::clear(mask);
			}

			static void toggle(MaskType mask)					
			{
				if (mask & ListMask)
					FieldBit::toggle(mask);
				Next::toggle(mask);						
			}		
			
			static void read(TDataType& result)
			{
				if (FieldBit::read())
					result |= ListMask;
				Next::read(result);
			}

			static void setMode(MaskType mask, uint8_t mode)					
			{
				if (mask & ListMask)
					FieldBit::setMode(mode);
				Next::setMode(mask, mode);			
			}
		};
		
		// Iterates through Field list and write value to them
		// TODO: Fix DataType, MaskType and BitMaskType!!!
		//		 Add Read(BitMaskType) - performance!		
		
		template <class TFieldList, class TFieldBitList, class TValueType>
		struct FieldListIterator;						
		template <class TFieldList, class TFieldBitList, class TValueType, bool VIsComplex>
		struct FieldListIteratorImpl;

		template <class TFieldBitList, class TValueType, bool VIsComplex> 
		class FieldListIteratorImpl<Loki::NullType, TFieldBitList, TValueType, VIsComplex>
		{
			typedef fasthal::common::BitMaskTypes<TValueType> BitMaskTypes;
			typedef typename BitMaskTypes::OneBitMask BitMaskType;
			typedef typename BitMaskTypes::BitNumberType BitNumberType;
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
		
		template <class THead, class TTail, class TFieldBitList, class TValueType>
		class FieldListIteratorImpl< Loki::Typelist<THead, TTail>, TFieldBitList, TValueType, true>
		{
			//bits on current field
			typedef typename fasthal::common::TL::Where<TFieldBitList, FieldFieldBitsPredicate<THead>::template Result>::Result FieldBits;
			static constexpr int FieldBitsLength = Loki::TL::Length<FieldBits>::value;
	
			typedef typename fasthal::common::TL::Where<FieldBits, InvertedFieldBitsPredicate>::Result InvertedFieldBits;
			static constexpr int InvertedFieldBitsLength = Loki::TL::Length<InvertedFieldBits>::value;
	
			typedef typename fasthal::common::TL::Where<FieldBits, fasthal::common::TL::Not<InvertedFieldBitsPredicate>::Result>::Result NotInvertedFieldBits;
			static constexpr int NotInvertedFieldBitsLength = Loki::TL::Length<NotInvertedFieldBits>::value;

			typedef THead Field; //Head points to current Field in the list.
			typedef field_data_type<Field> FieldType;
			typedef typename field_mask_types<Field>::MaskType FieldMaskType;
			static constexpr FieldMaskType Mask = GetFieldMask<FieldBits, FieldMaskType>::value;

			typedef typename Loki::Select<sizeof(FieldType) >= sizeof(TValueType), FieldType, TValueType>::Result DataType;
			
			typedef fasthal::common::BitMaskTypes<DataType> BitMaskTypes;
			typedef typename BitMaskTypes::OneBitMask BitMaskType;
			typedef typename BitMaskTypes::BitNumberType BitNumberType;
			typedef typename BitMaskTypes::MaskType MaskType;
			
			typedef PinWriteIterator<FieldBits, DataType> PinWrite;
			typedef FieldListIterator<TTail, TFieldBitList, TValueType> Next;
			
			public:

			static void write(DataType value)
			{
				// Apply inversion mask on value
				DataType result = PinWrite::appendWriteValue(value, DataType(0));

				if(FieldBitsLength == (int)Field::width())// whole Field
					Field::write(result);
				else
				{
					Field::clearAndSet(Mask, result);
				}

				Next::write(value);
			}

			static void clearAndSet(MaskType clearMask, MaskType setMask)
			{
				MaskType resultC = PinWrite::appendMaskValue(clearMask, MaskType());
				MaskType resultS = PinWrite::appendMaskValue(setMask, MaskType());
				if (NotInvertedFieldBitsLength == FieldBitsLength)
				{
					// All not inverted
					Field::clearAndSet(resultC, resultS);
				} else if (InvertedFieldBitsLength == FieldBitsLength)
				{
					// All inverted
					Field::clearAndSet(resultS, resultC);				
				} else
				{
					// Mix... calculate set and clear masks...
					constexpr MaskType inversionMask = GetInversionMask<FieldBits, MaskType>::value;
					// clear - not inverted clear + inverted set
					DataType clearMask = (resultC & ~inversionMask) | (resultS & inversionMask);
					// set - not inverted set + inverted clear
					DataType setMask = (resultS & ~inversionMask) | (resultC & inversionMask);
					Field::clearAndSet(clearMask, setMask);					
				}

				Next::clearAndSet(clearMask, setMask);
			}

			static void set(MaskType value)
			{
				// Invert inverted
				//static_assert(1 == FieldBitsLength, "All pins are normal");
				if (NotInvertedFieldBitsLength == FieldBitsLength)
				{
					MaskType result = PinWrite::appendMaskValue(value, MaskType());
					Field::set(result);					
				} else if (InvertedFieldBitsLength == FieldBitsLength)
				{
					MaskType result = PinWrite::appendMaskValue(value, MaskType());
					Field::clear(result);					
				} else
				{
					MaskType clearMask = PinWriteIterator<InvertedFieldBits, DataType>::appendMaskValue(value, MaskType());
					MaskType setMask = PinWriteIterator<NotInvertedFieldBits, DataType>::appendMaskValue(value, MaskType());
					Field::clearAndSet(clearMask, setMask);
				}

				Next::set(value);
			}

			static void clear(MaskType value)
			{
				// Invert inverted
				if (NotInvertedFieldBitsLength == FieldBitsLength)
				{
					MaskType result = PinWrite::appendMaskValue(value, MaskType());
					Field::clear(result);
				} else if (InvertedFieldBitsLength == FieldBitsLength)
				{
					MaskType result = PinWrite::appendMaskValue(value, MaskType());
					Field::set(result);
				} else
				{
					MaskType clearMask = PinWriteIterator<NotInvertedFieldBits, DataType>::appendMaskValue(value, MaskType());
					MaskType setMask = PinWriteIterator<InvertedFieldBits, DataType>::appendMaskValue(value, MaskType());
					Field::clearAndSet(clearMask, setMask);
				}

				Next::clear(value);
			}

			static void toggle(MaskType value)
			{
				// Ignore inverted - toggle does not care
				MaskType result = PinWrite::appendMaskValue(value, MaskType());
				Field::toggle(result);

				Next::toggle(value);
			}
	
			static DataType read()
			{
				// Apply inversion mask on value
				DataType FieldValue = Field::read();
				return PinWrite::appendReadValue(FieldValue, Next::read());
			}

			static void setMode(MaskType mask, uint8_t mode)
			{
				MaskType result = PinWrite::appendMaskValue(mask, MaskType());
				Field::setMode(result, mode);

				Next::setMode(mask, mode);
			}
		};				


		template <class THead, class TTail, class TFieldBitList, class TValueType>
		class FieldListIteratorImpl< Loki::Typelist<THead, TTail>, TFieldBitList, TValueType, false>
		{
			//pins on current Field
			typedef typename fasthal::common::TL::Where<TFieldBitList, FieldFieldBitsPredicate<THead>::template Result>::Result FieldBits;

			typedef THead Field; //Head points to current Field in the list.

			typedef TValueType DataType;
			
			typedef fasthal::common::BitMaskTypes<DataType> BitMaskTypes;
			typedef typename BitMaskTypes::OneBitMask BitMaskType;
			typedef typename BitMaskTypes::BitNumberType BitNumberType;
			typedef typename BitMaskTypes::MaskType MaskType;
			typedef PinWriteIterator<FieldBits, DataType> PinWrite;
			typedef FieldListIterator<TTail, TFieldBitList, TValueType> Next;
			
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

		template<class FieldList, class ValueType>
		struct IsComplexFieldListImpl
		{
			typedef typename FieldList::Head FieldType;
			static constexpr bool value = !field_mask_types<FieldType>::OnlyBitInterface && !fasthal::common::BitMaskTypes<ValueType>::OnlyBitInterface;
		};

		template<class ValueType>
		struct IsComplexFieldListImpl<Loki::NullType, ValueType>
		{
			static constexpr bool value = false;
		};

		template <class FieldList, class FieldBitList, class ValueType> 
		struct FieldListIterator: public FieldListIteratorImpl<FieldList, FieldBitList, ValueType, IsComplexFieldListImpl<FieldList, ValueType>::value>
		{
		};


		// Creates pin list from passed pins.
		template<typename ...TFieldBits>
		struct MakeFieldBitList
		{
			typedef typename fasthal::common::TL::MakeList<0, BitPositionHolder, TFieldBits...>::Result Result;
		};
	}
}

#endif /* PINLIST_H_ */