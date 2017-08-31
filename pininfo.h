/*
 * pininfo.h
 *
 * Created: 03.02.2013 1:39:43
 *  Author: astr0
 */ 

 #ifndef PININFO_H_
 #define PININFO_H_
 
 #include "typeutils.h"
 
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
 }
 
 #endif /* PININFO_H_ */