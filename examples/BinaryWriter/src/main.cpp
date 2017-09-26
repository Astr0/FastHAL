#ifndef F_CPU
#define F_CPU 16000000UL
#endif

//#define OS
//#define S1
#include "Arduino.h"

#include "fasthal.h"

const auto RxSize = 64;
const auto TxSize = 64;

using namespace fasthal;


FASTHAL_UART0(RxSize, TxSize);
#ifdef S1
FASTHAL_UART1(RxSize, TxSize);
#endif

void setup(){
    Adc::begin();
    Uart0::begin(9600);
    Uart0tx::begin();
    Uart0rx::begin();
    #ifdef S1
    Uart1::begin(115000);
    Uart1tx::begin();
    Uart1rx::begin();
    #endif
}

#ifdef OS
auto Uart0os = MakeOutStream<Uart0tx>();
auto Uart0tw = MakeBinaryWriter(Uart0os);
#ifdef S1
auto Uart1os = MakeOutStream<Uart1tx>();
auto Uart1tw = MakeBinaryWriter(Uart1os);
#endif

#else

auto Uart0tw = MakeBinaryWriter<Uart0tx>();
#ifdef S1
auto Uart1tw = MakeBinaryWriter<Uart1tx>();
#endif

#endif

template<typename T>
void test(T& writer, uint8_t read){
    writer.write("Hello from Arduino:");
    writer.write(read);
    writer.write("uint32_t: ");
    writer.write((uint32_t)read);
    writer.write("float: ");
    writer.write((float)read);
    writer.write("double: ");
    writer.write((double)read);
}

typedef AdcChannel<AdcRef::Default, false, 0> AdcA0;

void loop(){
    auto read = AdcA0::read();
    
    test(Uart0tw, read);
    #ifdef S1
    test(Uart1tw, read);
    #endif
    
    // Time::delayMs(1000);
}


int main(){
	setup();
	while (1){
		loop();
	}
	return 0;
}