#ifndef FH_AVR_ADC_H_
#define FH_AVR_ADC_H_

#include <avr/io.h>
#include "registers.hpp"

#ifdef FH_HAS_ADC

#include "../../fields/fields.hpp"
#include "../../fields/func_fieldbit.hpp"
#include "../../utils/functions.h"
#include "../../std/std_fake.hpp"

namespace fasthal{
	//static constexpr auto adc = avr::adc;
	namespace details{
		using adc_t = std::base_type_t<decltype(::fasthal::avr::adc)>;
	}

	struct adc_ref{
		#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
			static constexpr auto def                 = avr::refs_v<avr::REFS::_  | avr::REFS::_>; // VCC
			static constexpr auto external            = avr::refs_v<avr::REFS::_  | avr::REFS::_0>;
			static constexpr auto internal1v1         = avr::refs_v<avr::REFS::_1 | avr::REFS::_>;
		#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
			static constexpr auto def                 = avr::refs_v<avr::REFS::_  | avr::REFS::_  | avr::REFS::_>; // VCC
			static constexpr auto external            = avr::refs_v<avr::REFS::_  | avr::REFS::_1 | avr::REFS::_0>;
			static constexpr auto internal1v1         = avr::refs_v<avr::REFS::_  | avr::REFS::_1 | avr::REFS::_>;
			static constexpr auto internal2v56        = avr::refs_v<avr::REFS::_2 | avr::REFS::_1 | avr::REFS::_>;
			static constexpr auto internal2v56_ExtCap = avr::refs_v<avr::REFS::_2 | avr::REFS::_1 | avr::REFS::_0>;
		#else  
			static constexpr auto external     = avr::refs_v<avr::REFS::_  | avr::REFS::_>;			
			static constexpr auto def          = avr::refs_v<avr::REFS::_  | avr::REFS::_0>; // AVCC ext cap
		#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
			static constexpr auto internal1v1  = avr::refs_v<avr::REFS::_1 | avr::REFS::_>;
			static constexpr auto internal2v56 = avr::refs_v<avr::REFS::_1 | avr::REFS::_0>;
		#else			
			static constexpr auto internal1v1  = avr::refs_v<avr::REFS::_1 | avr::REFS::_0>; // was just "internal"
		#endif
		#endif 
	};

	struct adc_ps{
		static constexpr auto _2   = avr::adps_v<avr::ADPS::_  | avr::ADPS::_  | avr::ADPS::_0>;
		static constexpr auto _4   = avr::adps_v<avr::ADPS::_  | avr::ADPS::_1 | avr::ADPS::_>;
		static constexpr auto _8   = avr::adps_v<avr::ADPS::_  | avr::ADPS::_1 | avr::ADPS::_0>;
		static constexpr auto _16  = avr::adps_v<avr::ADPS::_2 | avr::ADPS::_  | avr::ADPS::_>;
		static constexpr auto _32  = avr::adps_v<avr::ADPS::_2 | avr::ADPS::_  | avr::ADPS::_0>;
		static constexpr auto _64  = avr::adps_v<avr::ADPS::_2 | avr::ADPS::_1 | avr::ADPS::_>;
		static constexpr auto _128 = avr::adps_v<avr::ADPS::_2 | avr::ADPS::_1 | avr::ADPS::_0>;
		// default a2d prescaler should be inside the desired 50-200 KHz range.
		#if F_CPU >= 16000000 // 16 MHz / 128 = 125 KHz
		static constexpr auto def = _128;
		#elif F_CPU >= 8000000 // 8 MHz / 64 = 125 KHz
		static constexpr auto def = _64;
		#elif F_CPU >= 4000000 // 4 MHz / 32 = 125 KHz
		static constexpr auto def = _32;
		#elif F_CPU >= 2000000 // 2 MHz / 16 = 125 KHz
		static constexpr auto def = _16;		
		#elif F_CPU >= 1000000 // 1 MHz / 8 = 125 KHz
		static constexpr auto def = _8;
		#elif F_CPU >= 500000 // 0.5 MHz / 4 = 125 KHz
		static constexpr auto def = _4;
		#else // 128 kHz / 2 = 64 KHz 
		static constexpr auto def = _2;
		#endif			
	};

	struct adc_res{
		static constexpr auto _8 = integral_constant<bool, true>{};
		static constexpr auto _10 = integral_constant<bool, false>{};
		static constexpr auto def = _10;
	};


	template<typename TRef = decltype(adc_ref::def)>
	constexpr auto set_ref(details::adc_t adc, TRef ref = adc_ref::def){
		return write(avr::refs, ref);
	}
	template<typename TRef = decltype(adc_ref::def)>
	void set_ref_(details::adc_t adc, TRef ref = adc_ref::def){
		write_(avr::refs, ref);
	}

	template<typename TAdps = decltype(adc_ps::def)>
	constexpr auto set_ps(details::adc_t adc, TAdps ps = adc_ps::def){
		return write(avr::adps, ps);
	}
	template<typename TAdps = decltype(adc_ps::def)>
	void set_ps_(details::adc_t adc, TAdps ps = adc_ps::def){
		write_(avr::adps, ps);
	}

	template<typename TRes = decltype(adc_res::def)>
	constexpr auto set_res(details::adc_t adc, TRes res = adc_res::def){
		return set(avr::adlar, res);
	}
	template<typename TRes = decltype(adc_res::def)>
	void set_res_(details::adc_t adc, TRes res = adc_res::def){
		set_(avr::adlar, res);
	}

	template<typename TMux>
	constexpr auto select(details::adc_t adc, TMux mux){
		return write(avr::mux, mux);
	}
	template<typename TMux>
	inline void select_(details::adc_t adc, TMux mux){
		write_(avr::mux, mux);
	}	

	// enable/disable ADC
	namespace details{
		template<>
		struct func_fieldbit_impl<adc_t>: func_fieldbit_enable<decltype(avr::aden)>{};
	}	
	
	template<
		typename TRef = decltype(adc_ref::def),
		typename TRes = decltype(adc_res::def),
		typename TAdps = decltype(adc_ps::def)>
	constexpr auto begin(details::adc_t adc, TRef ref = adc_ref::def, TRes res = adc_res::def, TAdps ps = adc_ps::def){
		return combine(
			set_ref(adc, ref),
			set_res(adc, res),
			set_ps(adc, ps),
			enable(adc)			
		);
	}
	template<
		typename TRef = decltype(adc_ref::def),
		typename TRes = decltype(adc_res::def),
		typename TAdps = decltype(adc_ps::def)>
	void begin_(details::adc_t adc, TRef ref = adc_ref::def, TRes res = adc_res::def, TAdps ps = adc_ps::def)
	{
		apply(begin(adc, ref, res, ps));
	}

	constexpr auto start(details::adc_t adc){
		return set(avr::adsc);
	}	
	void start_(details::adc_t adc){
		set_(avr::adsc);
	}
	constexpr auto running_(details::adc_t adc){
		return read_(avr::adsc);
	}
	void wait_(details::adc_t adc){
		wait_lo(avr::adsc);
	}
	template<typename TMux>
	constexpr auto convert_(details::adc_t adc, TMux mux){
		apply(
			select(adc, mux), 
			start(adc)
		);
		wait_(adc);
		return read_(adc);
	}
	template<typename TMux>
	constexpr auto convert8_(details::adc_t adc, TMux mux){
		apply(
			select(adc, mux), 
			start(adc)
		);
		wait_(adc);
		return read_(avr::adch);
	}

	namespace details{
		template<typename TMux>
		struct func_fieldbit_adc{
			using mux_t = TMux;
		};
	}

	template<typename T, typename TMux = typename details::func_fieldbit<T>::mux_t>
	constexpr auto select(T adMux){
		return write(avr::mux, TMux{});
	}
	template<typename T, typename TMux = typename details::func_fieldbit<T>::mux_t>
	void select_(T adMux){
		write_(avr::mux, TMux{});
	}	

	template<typename T, typename TMux = typename details::func_fieldbit<T>::mux_t>
	constexpr auto convert_(T adMux){
		return convert_(details::adc_t{}, TMux{});
	}

	template<typename T, typename TMux = typename details::func_fieldbit<T>::mux_t>
	constexpr auto convert8_(T adMux){
		return convert8_(details::adc_t{}, TMux{});
	}
}

#endif // ADC stuff

#endif