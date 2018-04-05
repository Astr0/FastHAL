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
		 // default voltage, desired resolution
		begin(adc, adc_ref::def, my_adc_res),
		// LedPin::setMode(PinMode::Output);
		//makeOutput(led)
		set(led_ddr) 
	);
	
	while (true){		
	// 	auto val = TestAdc::read();
		auto val = convert_(adc, channel);
	// 	LedPin::set(val < 100);
		set(led, val < 100);
	}
	
	
    // while (1) 
    // {
    // }
}

