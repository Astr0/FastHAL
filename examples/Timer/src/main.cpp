#define F_CPU 16000000UL

#include "fasthal.h"

using namespace fasthal;

typedef PinB5 LedPin;

void TestTimer0(){
	Timer0::setWgm(Timer0Wgm::CtcA);
	Timer0::setOcA(200);
	Timer0::enable(Timer0Cs::P1024);
	
	uint16_t count = 0;
	while(count < 400){
		if (Timer0::interruptsReady(Timer0F::OuputCompareA)){
			Timer0::clearInterruptsReady(Timer0F::OuputCompareA);
			count++;
			if (count % 10 == 0){
				LedPin::toggle();
			}
		}
	}
}

void TestTimer1(){
	Timer1::setWgm(Timer1Wgm::CtcA);
	Timer1::setOcA(10000);
	Timer1::enable(Timer1Cs::P1024);
	
	uint16_t count = 0;
	while(count < 20){
		if (Timer1::interruptsReady(Timer1F::OuputCompareA)){
			Timer1::clearInterruptsReady(Timer1F::OuputCompareA);
			count++;
			LedPin::toggle();
		}
	}
}

void TestTimer2(){
	Timer2::setWgm(Timer2Wgm::Normal);
	Timer2::setOcA(200);
	Timer2::enable(Timer2Cs::P1024);
	
	uint16_t count = 0;
	while(1){
		if (Timer2::interruptsReady(Timer2F::OuputCompareA)){
			Timer2::clearInterruptsReady(Timer2F::OuputCompareA);
			count++;
			if (count == 10){
				count = 0;
				LedPin::toggle();
			}
		}
	}
}

int main(){
	LedPin::setMode(PinMode::Output);
	TestTimer0();
	TestTimer1();
	TestTimer2();
	
	while(1);
	
    return 0;
}