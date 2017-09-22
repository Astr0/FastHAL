#include "Arduino.h"
#include "fasthal.h"

const auto RxSize = 64;
const auto TxSize = 64;

using namespace fasthal;

#define FH
//#define OS
//#define S1

#ifdef FH
FASTHAL_UART0(RxSize, TxSize);
#ifdef S1
FASTHAL_UART1(RxSize, TxSize);
#endif
#endif

void setup(){
    Adc::begin();
    #ifdef FH
    Uart0::begin(9600);
    Uart0tx::begin();
    Uart0rx::begin();
    #ifdef S1
    Uart1::begin(115000);
    Uart1tx::begin();
    Uart1rx::begin();
    #endif
    #else
    Serial.begin(9600);    
    #ifdef S1
    Serial1.begin(115000);
    #endif
    #endif

}

#ifdef FH

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

#endif

template<typename T>
void test(T& writer, uint8_t read){
    writer.write("Hello from Arduino:");
    writer.write(read);
    writer.write("uint32_t: ");
    writer.write((uint32_t)read);
    writer.write("float: ");
    #ifdef FH
    writer.write((float)read);
    #else
    float readF = (float)read;
    writer.write(*(uint32_t*)(char*)(&readF));
    #endif

    writer.write("double: ");
    #ifdef FH
    writer.write((double)read);
    #else
    double readD = (double)read;
    writer.write(*(uint32_t*)(char*)(&readD));    
    writer.write(*(uint32_t*)(char*)(&readD + 4));    
    #endif
}

void loop(){
    auto read = arduino::AdcA0::read();
    
    auto ms = Time::micros();
    #ifdef FH
    
    test(Uart0tw, read);
    #ifdef S1
    test(Uart1tw, read);
    #endif
    
    #else
    
    test(Serial, read);
    #ifdef S1
    test(Serial1, read);
    #endif

    #endif
    ms = Time::micros() - ms;
    #ifdef FH
    Uart0tw.write("Micros: ");
    Uart0tw.write(ms);
    #else
    Serial.write("Micros: ");
    Serial.write(ms);
    #endif

    Time::delayMs(1000);
}
