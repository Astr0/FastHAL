#ifndef FH_FIELDS_H_
#define FH_FIELDS_H_

#include "fieldbit.hpp"
#include "fieldbits.hpp"
#include "../sys/typelistutils.h"
#include "../sys/bitholder.h"


namespace fasthal{
    class NullPort
	{
		public:
		constexpr NullPort(){}
			
		typedef NullPort PortType;
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
		struct Pin{
			typedef typename fasthal::Pin<PortType, VNumber> Type;
		};
	};
	typedef Pin<NullPort, 0> NullPin;

    // VPort - static port-like wrapper for Pins
	template<class... TPins>
	class VPort
	{
		private:
			typedef typename fasthal::priv::MakePinList<TPins...>::Result Pins;
			typedef typename Loki::TL::NoDuplicates<typename priv::MakePortList<Pins>::Result>::Result Ports;
			static constexpr unsigned PinCount = fasthal::common::TL::length<Pins>();
			static constexpr unsigned ByteSize = fasthal::common::minSizeInBytes(PinCount);
			
			//typedef fasthal::common::BitHolder<fasthal::common::minSizeInBytes(PinCount)> DataType;
			typedef typename fasthal::common::BitMaskType<fasthal::common::minSizeInBytes(PinCount)>::Result DataType;
			typedef fasthal::common::BitMaskTypes<DataType> BitMaskTypes;			
			typedef typename BitMaskTypes::OneBitMask PinMaskType;
			typedef typename BitMaskTypes::BitNumberType PinNumberType;
			typedef typename BitMaskTypes::MaskType MaskType;

		public:
			constexpr VPort(){}
	
			typedef VPort<TPins...> PortType;
			
			static constexpr PinNumberType width(){return PinCount;}
						
			static void write(DataType value) 
			{
				priv::PortListIterator<Ports, Pins, DataType>::write(value);
			}
			
			static void clearAndSet(MaskType clearMask, MaskType setMask) 
			{
				priv::PortListIterator<Ports, Pins, DataType>::clearAndSet(clearMask, setMask);
			}				
			static void set(MaskType mask) 
			{
				priv::PortListIterator<Ports, Pins, DataType>::set(mask);
			}
			static void clear(MaskType mask) 
			{
				priv::PortListIterator<Ports, Pins, DataType>::clear(mask);					
			}
			static void toggle(MaskType mask) 
			{
				priv::PortListIterator<Ports, Pins, DataType>::toggle(mask);	
			}
			static DataType read() 
			{
				return priv::PortListIterator<Ports, Pins, DataType>::read();
			}
			static bool read(PinMaskType pin)
			{
				// TODO: Optimize this
				//return priv::PortListIterator<Ports, Pins, DataType>::Read(pin);
				return read() & pin;
			}
			static void setMode(MaskType mask, uint8_t mode)
			{
				priv::PortListIterator<Ports, Pins, DataType>::setMode(mask);
			}
			
			template<PinNumberType VPin>
			struct Pin{
				typedef typename Loki::TL::TypeAt<Pins, VPin>::Result::Pin Type;
			};
	};
}

#endif