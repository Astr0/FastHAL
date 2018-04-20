#define F_CPU 16000000UL

#include "fasthal.h"

using namespace fasthal;

typedef PinB5 LedPin;
Bounce<InvertedPin<PinD2>, ElapsedChecks<50000>> button;

int main(){	
	LedPin::setMode(PinMode::Output);
	button.begin(PinMode::InputPullup);
	while(1){
		if (button.update() && button.read())
			LedPin::toggle();
	}
	return 0;
}