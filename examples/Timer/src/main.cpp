#define F_CPU 16000000UL

#include "fasthal.h"

using namespace fasthal;

typedef PinB5 LedPin;

int main(){
	LedPin::setMode(PinMode::Output);
	
	Timer0::setWgm(Timer0Wgm::CtcA);
	Timer0::setOcA(200);
	Timer0::enable(Timer0Cs::P1024);
		
	uint8_t count = 0;
    while(1){
		if (Timer0::interruptsReady(Timer0F::OuputCompareA)){
			Timer0::clearInterruptsReady(Timer0F::OuputCompareA);
			count++;
			if (count == 10){
				count = 0;
				LedPin::toggle();
			}
		}
    }
    return 0;
}