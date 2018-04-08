#include "fasthal.hpp"

using namespace fasthal;

static constexpr auto ledPin = pinB5;

void test_timer0(){
	apply(
		write(timer0.wgm, timer0_wgm_v<timer0_wgm::ctc_a>),
		write(timer0.ocA.ocr, uint8_v<200>),
		write(timer0.cs, timer0_cs_v<timer0_cs::_1024>)
	);
	
	uint16_t count = 0;
	while(count < 400){
		if (ready_(timer0.ocA.irq)){
			reset_(timer0.ocA.irq);
			count++;
			if (count % 10 == 0){
				toggle_(ledPin);				
			}
		}
	}
}

void test_timer1(){
	apply(
		write(timer1.wgm, timer1_wgm_v<timer1_wgm::ctc_a>),
		write(timer1.ocA.ocr, uint16_v<10000>),
		write(timer1.cs, timer1_cs_v<timer1_cs::_1024>)
	);
	
	uint16_t count = 0;
	while(count < 20){
		if (ready_(timer1.ocA.irq)){
			reset_(timer1.ocA.irq);
			count++;
			toggle_(ledPin);
		}
	}
}

void test_timer2(){
	apply(
		write(timer2.wgm, timer2_wgm_v<timer2_wgm::normal>),
		write(timer2.ocA.ocr, uint8_v<200>),
		write(timer2.cs, timer2_cs_v<timer2_cs::_1024>)
	);
	
	uint16_t count = 0;
	while(1){
		if (ready_(timer2.ocA.irq)){
			reset_(timer2.ocA.irq);
			count++;
			if (count == 10){
				count = 0;
				toggle_(ledPin);
			}
		}
	}
}


int main(){
	makeOutput_(ledPin);
	test_timer0();
	test_timer1();
	test_timer2();
	while (true) ;
}