#include <avr/io.h>
#include "fasthal.hpp"

using namespace fasthal;
using namespace fasthal::avr;

constexpr uint8_t channel = 0;
constexpr auto my_adc_res = adc_res::_8;

// TODO: HAL to make this much nicer 
// static constexpr auto led = gpioD5;
static constexpr auto led = portD5;
static constexpr auto led_ddr = ddrD5;


//typedef AdcChannel<AdcRef::Default, is8bit, channel> TestAdc;

int main(void)
{
 	//	Adc::begin();

	apply(
		// default prescaler
		set_ps(adc),
		// and default voltage
		set_ref(adc, adc_ref::def),		
		// set desired resoultion
		set_res(adc, my_adc_res),
		// enable ADC
		enable(adc),
		//makeOutput(led)
		set(led_ddr) 
	);
	
	while (true){

	}
	
	// LedPin::setMode(PinMode::Output);
	
    // while (1) 
    // {
	// 	auto val = TestAdc::read();
	// 	LedPin::set(val < 100);
    // }
}

