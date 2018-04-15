#ifndef FH_AVR_ADC_H_
#define FH_AVR_ADC_H_

#include <avr/io.h>
#include "registers.hpp"
#include "../../fields/fields.hpp"
#include "../../fields/func_fieldbit.hpp"
#include "../../utils/functions.h"
#include "../../std/std_fake.hpp"

namespace fasthal{
	namespace avr{
		#include "adc_impl.hpp"
	}

	#ifdef FH_HAS_ADC
	//static constexpr auto adc = avr::adc;
	namespace details{
		struct adc_impl{
			static constexpr auto result = avr::adcr;
			static constexpr auto result8 = avr::adch;
			static constexpr auto ps = avr::adps;
			static constexpr auto refs = avr::refs;
			static constexpr auto mux = avr::mux;
			static constexpr auto mode8 = avr::adlar;
		};

		template<>
		struct func_fieldbit_impl<adc_impl>:
			// enable
			func_fieldbit_enable<decltype(avr::aden)>,
			// ready, wait
			func_fieldbit_ready<decltype(invert(avr::adsc))>,
			// reset (start)
			func_fieldbit_reset<decltype(avr::adsc)>
		{};
	}
	static constexpr auto adc = details::adc_impl{};

	template<typename TMux>
	constexpr auto select(details::adc_impl adc, TMux mux){
		return write(avr::mux, mux);
	}
	template<typename TMux>
	inline void select_(details::adc_impl adc, TMux mux){
		write_(avr::mux, mux);
	}	

	template<
		typename TRef = decltype(avr::ad_ref::def),
		typename TMode8 = integral_constant<bool, false>,
		typename TAdps = decltype(avr::ad_ps::def)>
	inline constexpr auto begin(details::adc_impl adc, TRef ref = avr::ad_ref::def, TMode8 mode8 = integral_constant<bool, false>{}, TAdps ps = avr::ad_ps::def){
		return combine(
			write(adc.refs, ref),
			write(adc.ps, ps),
			set(adc.mode8, mode8),
			enable(adc)			
		);
	}
	template<
		typename TRef = decltype(avr::ad_ref::def),
		typename TMode8 = integral_constant<bool, false>,
		typename TAdps = decltype(avr::ad_ps::def)>
	inline void begin_(details::adc_impl adc, TRef ref = avr::ad_ref::def, TMode8 mode8 = integral_constant<bool, false>{}, TAdps ps = avr::ad_ps::def){
		apply(begin(adc, ref, mode8, ps));
	}

	namespace details{
		template<typename TMux>
		inline void do_adc(details::adc_impl adc, TMux mux){
			apply(
				select(adc, mux), 
				reset(adc)
			);
			wait_(adc);
		}
	}

	template<typename TMux>
	constexpr auto convert_(details::adc_impl adc, TMux mux){
		details::do_adc(adc, mux);
		return read_(adc.result);
	}
	template<typename TMux>
	constexpr auto convert8_(details::adc_impl adc, TMux mux){
		details::do_adc(adc, mux);
		return read_(adc.result8);
	}

	namespace details{
		template<typename TMux>
		struct func_fieldbit_adc{
			using mux_t = TMux;
		};
	}

	template<typename T, typename TMux = typename details::func_fieldbit<T>::mux_t>
	constexpr auto select(T adMux){
		return select(adc, TMux{});
	}
	template<typename T, typename TMux = typename details::func_fieldbit<T>::mux_t>
	void select_(T adMux){
		select_(adc, TMux{});
	}

	template<typename T, typename TMux = typename details::func_fieldbit<T>::mux_t>
	constexpr auto convert_(T adMux){
		return convert_(adc, TMux{});
	}

	template<typename T, typename TMux = typename details::func_fieldbit<T>::mux_t>
	constexpr auto convert8_(T adMux){
		return convert8_(adc, TMux{});
	}
	#endif
}

#endif