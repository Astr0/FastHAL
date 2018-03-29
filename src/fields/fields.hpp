#ifndef FH_FIELDS_H_
#define FH_FIELDS_H_

#include "fieldbit.hpp"
#include "fieldbits.hpp"
#include "../sys/typelistutils.h"
#include "../sys/bitholder.h"


namespace fasthal{
    struct NullField
	{
		static void write(uint8_t value) {}
		static void clearAndSet(uint8_t clearMask, uint8_t setMask) {}
		static void set(uint8_t mask) {}
		static void clear(uint8_t mask) {}
		static void toggle(uint8_t mask) {}
		static constexpr uint8_t read() {return 0;}
		static constexpr bool read(uint8_t pin){return false;}
	};
	typedef FieldBit<NullField, 0> NullPin;

    // VField - static field-like wrapper for FieldBits
	template<class... TFieldBits>
	class VField
	{
		private:
			typedef typename fasthal::priv::MakeFieldBitList<TFieldBits...>::Result FieldBits;
			typedef typename Loki::TL::NoDuplicates<typename priv::MakeFieldList<FieldBits>::Result>::Result Fields;
			static constexpr unsigned BitCount = fasthal::common::TL::length<FieldBits>();
			static constexpr unsigned ByteSize = fasthal::common::minSizeInBytes(BitCount);
			
			//typedef fasthal::common::BitHolder<fasthal::common::minSizeInBytes(BitCount)> DataType;
			typedef typename fasthal::common::BitMaskType<fasthal::common::minSizeInBytes(BitCount)>::Result DataType;
			typedef fasthal::common::BitMaskTypes<DataType> BitMaskTypes;			
			typedef typename BitMaskTypes::OneBitMask BitMaskType;
			typedef typename BitMaskTypes::BitNumberType BitNumberType;
			typedef typename BitMaskTypes::MaskType MaskType;

		public:
			constexpr VField(){}
	
			typedef VField<TFieldBits...> FieldType;
			
			static constexpr BitNumberType width(){return BitCount;}
						
			static void write(DataType value) 
			{
				priv::FieldListIterator<Fields, FieldBits, DataType>::write(value);
			}
			
			static void clearAndSet(MaskType clearMask, MaskType setMask) 
			{
				priv::FieldListIterator<Fields, FieldBits, DataType>::clearAndSet(clearMask, setMask);
			}				
			static void set(MaskType mask) 
			{
				priv::FieldListIterator<Fields, FieldBits, DataType>::set(mask);
			}
			static void clear(MaskType mask) 
			{
				priv::FieldListIterator<Fields, FieldBits, DataType>::clear(mask);					
			}
			static void toggle(MaskType mask) 
			{
				priv::FieldListIterator<Fields, FieldBits, DataType>::toggle(mask);	
			}
			static DataType read() 
			{
				return priv::FieldListIterator<Fields, FieldBits, DataType>::read();
			}
			static bool read(BitMaskType bit)
			{
				// TODO: Optimize this
				//return priv::FieldListIterator<Fields, FieldBits, DataType>::Read(pin);
				return read() & bit;
			}
			
			template<BitNumberType VPin>
			struct FieldBit{
				typedef typename Loki::TL::TypeAt<FieldBits, VPin>::Result::FieldBit Type;
			};
	};
}

#endif