#ifndef FH_FIELDS_H_
#define FH_FIELDS_H_

#include "fieldbit.hpp"
#include "fieldbits.hpp"
#include "../sys/typelistutils.h"
#include "../sys/bitholder.h"


namespace fasthal{
    struct NullField
	{
		typedef NullField FieldType;
		typedef uint8_t DataType;
		
		static constexpr uint8_t width(){return 8 * sizeof(DataType);}
		
		static void write(DataType value) {}
		template<class TClear, class TSet>
		static void clearAndSet(TClear clearMask, TSet setMask) {}
		template<class TMask>
		static void set(TMask mask) {}
		template<class TMask>
		static void clear(TMask mask) {}
		template<class TMask>
		static void toggle(TMask mask) {}
		static constexpr DataType read() {return 0;}
		template<class TMask>
		static constexpr bool read(TMask pin){return false;}
		template<class TMask>
		static void setMode(TMask mask, uint8_t mode){}
			
		template<unsigned VNumber>
		struct FieldBit{
			typedef typename fasthal::FieldBit<FieldType, VNumber> Type;
		};
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
			static void setMode(MaskType mask, uint8_t mode)
			{
				priv::FieldListIterator<Fields, FieldBits, DataType>::setMode(mask);
			}
			
			template<BitNumberType VPin>
			struct FieldBit{
				typedef typename Loki::TL::TypeAt<FieldBits, VPin>::Result::FieldBit Type;
			};
	};
}

#endif