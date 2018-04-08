// nice ports abstraction

#pragma once

#ifndef FH_AVR_REGISTERS_H_
#define FH_AVR_REGISTERS_H_

#include <avr/io.h>
#include "../../utils/functions.h"
#include "../../std/std_fake.hpp"
#include "../../fields/info.hpp"
#include "../../fields/mfield.hpp"

#define FH_DECLARE_REGISTER_ONLY( NAME, VAR)\
namespace priv{\
	FH_WRAPVARIABLE(VAR ## _reg, VAR)\
}\
static constexpr auto NAME = details::avr_register<priv::VAR ## _reg>{};\


#define FH_DECLARE_REGISTER( NAME, VAR)\
namespace priv{\
	FH_WRAPVARIABLE(VAR ## _reg, VAR)\
}\
static constexpr auto NAME = details::avr_register<priv::VAR ## _reg>{};\
static constexpr auto NAME ## 0 = fieldBit<0>(NAME);\
static constexpr auto NAME ## 1 = fieldBit<1>(NAME);\
static constexpr auto NAME ## 2 = fieldBit<2>(NAME);\
static constexpr auto NAME ## 3 = fieldBit<3>(NAME);\
static constexpr auto NAME ## 4 = fieldBit<4>(NAME);\
static constexpr auto NAME ## 5 = fieldBit<5>(NAME);\
static constexpr auto NAME ## 6 = fieldBit<6>(NAME);\
static constexpr auto NAME ## 7 = fieldBit<7>(NAME);\

namespace fasthal{
	namespace avr{
		namespace details{
			template<class TAccess>
			struct avr_register
			{
				using datatype_t = std::base_type_t<decltype(TAccess::value())>;

				inline static void write(datatype_t value) { TAccess::value() = value; }
				inline static datatype_t read() {return TAccess::value();}
			};	
		}
	}

	namespace details{
		template<class TAccess>
		struct is_field_impl<::fasthal::avr::details::avr_register<TAccess>>: std::true_type{};
	}
    
	namespace avr{
		#include "registers_impl/gpio_reg.hpp"
		#include "registers_impl/adc_reg.hpp"
		#include "registers_impl/irq_reg.hpp"
		#include "registers_impl/uart_reg.hpp"
	}
}    

//#undef FASTHAL_DECLAREREGISTER
//#undef FASTHAL_DECLARE_REGISTER_ONLY

#endif
