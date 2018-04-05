#include <avr/io.h>
#include "fasthal.hpp"

using namespace fasthal;
using namespace fasthal::avr;

constexpr auto channel = mux_v<0>;
constexpr auto my_adc_res = adc_res::_8;

// TODO: HAL to make this much nicer 
// static constexpr auto led = gpioD5;
static constexpr auto led = portD5;
static constexpr auto led_ddr = ddrD5;


//typedef AdcChannel<AdcRef::Default, is8bit, channel> TestAdc;

int main(void)
{

	apply(
	 	//	Adc::begin();
		// default prescaler
		set_ps(adc),
		// and default voltage
		set_ref(adc, adc_ref::def),		
		// set desired resoultion
		set_res(adc, my_adc_res),
		// // select channel
		// select(adc, channel),
		// enable ADC
		enable(adc),
	
		// LedPin::setMode(PinMode::Output);
		//makeOutput(led)
		set(led_ddr) 
	);
	
	while (true){		
		auto val = convert_(adc, channel);
		set(led, val < 100);
	}
	
	
    // while (1) 
    // {
	// 	auto val = TestAdc::read();
	// 	LedPin::set(val < 100);
    // }
}

