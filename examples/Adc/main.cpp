#define F_CPU 16000000UL


#include <avr/io.h>
#include "fasthal.h"
#include "util/delay.h"

using namespace fasthal;

const uint8_t channel = 0;
const bool is8bit = true;

typedef AdcChannel<AdcRef::Default, is8bit, channel> TestAdc;
typedef PinB5 LedPin;

int main(void)
{
	Adc::begin();
	
	LedPin::setMode(PinMode::Output);
	
    while (1) 
    {
		auto val = TestAdc::read();
		LedPin::set(val < 100);
    }
}

