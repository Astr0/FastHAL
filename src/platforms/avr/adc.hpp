#ifndef FH_AVR_ADC_H_
#define FH_AVR_ADC_H_

#include <avr/io.h>
#include "registers.hpp"

#ifdef FH_HAS_ADC

#include "../../fields/fields.hpp"

namespace fasthal{
	//static constexpr auto adc = avr::adc;
	namespace details{
		using adc_t = decltype(::fasthal::avr::adc);
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
	template<typename TAdps = decltype(adc_ps::def)>
	constexpr auto set_ps(details::adc_t adc, TAdps ps = adc_ps::def){
		return write(avr::adps, ps);
	}
	template<typename TRes = decltype(adc_res::def)>
	constexpr auto set_res(details::adc_t adc, TRes res = adc_res::def){
		return set(avr::adlar, res);
	}
	template<typename TMux>
	constexpr auto select(details::adc_t adc, TMux mux){
		return write(avr::mux, mux);
	}
	constexpr auto enable(details::adc_t adc){
		return set(avr::aden);
	}
	constexpr auto disable(details::adc_t adc){
		return clear(avr::aden);
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
	constexpr auto start(details::adc_t adc){
		return set(avr::adsc);
	}	


	template<typename TRef = decltype(adc_ref::def)>
	void set_ref_(details::adc_t adc, TRef ref = adc_ref::def){
		apply(set_ref(adc, ref));
	}
	template<typename TAdps = decltype(adc_ps::def)>
	void set_ps_(details::adc_t adc, TAdps ps = adc_ps::def){
		apply(set_ps(adc, ps));
	}
	template<typename TRes = decltype(adc_res::def)>
	void set_res_(details::adc_t adc, TRes res = adc_res::def){
		apply(set_res(adc, res));
	}
	template<typename TMux>
	void select_(details::adc_t adc, TMux mux){
		apply(select(adc, mux));
	}	
	void enable_(details::adc_t adc){
		apply(enable(adc));
	}
	void disable_(details::adc_t adc){
		apply(disable(adc));
	}
	template<
		typename TRef = decltype(adc_ref::def),
		typename TRes = decltype(adc_res::def),
		typename TAdps = decltype(adc_ps::def)>
	void begin_(details::adc_t adc, TRef ref = adc_ref::def, TRes res = adc_res::def, TAdps ps = adc_ps::def)
	{
		apply(begin(adc, ref, res, ps));
	}

	void start_(details::adc_t adc){
		apply(start(adc));
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



    // class Adc{        
    // public:
	// 	static void begin(AdcPrescaler prescaler = AdcPrescaler::Default){
	// 		ADCSRA |= (1 << ADEN) | ((uint8_t)prescaler);
	// 	}
		
	// 	static void end() {
	// 		disable();
	// 	}
	
	// 	static void select(AdcRef ref, bool is8bit, std::uint8_t adMux){
	// 		#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
	// 		// 5 MUX bits and ADLAR IN ADCSRB
	// 		ADMUX = ((std::uint8_t)ref) | (adMuxmux & 0x1F);
			
	// 		fh_wbi(ADCSRB, ADLAR, is8bit);
	// 		#else
						
	// 		#if defined(ADCSRB) && defined(MUX5)
	// 		// the MUX5 bit of ADCSRB selects whether we're reading from channels
	// 		// 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
	// 		fh_wbi(ADCSRB, MUX5, adMux & 0x10);
	// 		#endif
			
	// 		ADMUX = ((std::uint8_t)ref) | (is8bit ? (1 << ADLAR) : 0) | (adMux & 0x0F);

	// 		#endif
	// 	}	
	
	//     static void enable(){
    //         fh_sbi(ADCSRA, ADEN); 
    //     }

    //     static void disable(){
    //         fh_cbi(ADCSRA, ADEN);
    //     }
		
	// 	static void start(){
	// 		// start the conversion
	// 		fh_sbi(ADCSRA, ADSC);
	// 	}
		
	// 	static bool running(){
	// 		// ADSC is cleared when the conversion finishes
	// 		return ADCSRA & (1 << ADSC);
	// 	}
		
	// 	static void wait(){
	// 		while (running());
	// 	}
		
	// 	static uint16_t read(){
	// 		return ADC;
	// 	}

	// 	static uint8_t read8bit(){
	// 		return ADCH;
	// 	}			
    // };
    
		
    // template<AdcRef ref, bool is8bit, uint8_t mux>
    // class AdcChannel{
    // public:	
	// 	typedef typename std::conditional<is8bit, uint8_t, uint16_t>::type result_t;
	
	// 	static result_t read(){			
	// 		Adc::select(ref, is8bit, mux);
	// 		Adc::start();
    //         Adc::wait();
	// 		return is8bit ? Adc::read8bit() : Adc::read();
    //     }
    // };
}

#endif // ADC stuff

#endif