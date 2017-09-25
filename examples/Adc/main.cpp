#define F_CPU 16000000UL


#include <avr/io.h>
#include "fasthal.h"
#include "util/delay.h"

using namespace fasthal;

typedef AdcChannel<AdcRef::Default, true, 0> TestAdc;
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

