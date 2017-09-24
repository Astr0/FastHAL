#define F_CPU 16000000UL


#include <avr/io.h>
#include "fasthal.h"
#include "util/delay.h"

using namespace fasthal;

typedef AdcChannel<0, true> TestAdc;
typedef PinB5 LedPin;

int main(void)
{
	Adc::begin();
	
	LedPin::setMode(PinMode::Output);
	//Adc::begin();
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | ()
	
    while (1) 
    {
		auto val = TestAdc::read();
		LedPin::set(val < 200);
    }
}

