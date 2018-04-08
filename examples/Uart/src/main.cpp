//#define FH_UART0_RX 1
//#define FH_UART0_TX 1

#include "fasthal.hpp"
//#include <Arduino.h>

using namespace fasthal;

int main(){
    enable_(irq);
    begin_(uart0, baud_v<9600>);
    while (true){
        if (available(uart0)){
            auto v = read_dirty(uart0);
            write(uart0, v);
        }
        
        // for(auto x = 'a'; x <= 'z'; ++x)
        //     write(uart0, x);
        // write(uart0, 10);
        // write(uart0, 13);
        // end_(uart0);
        //flush(uart0);
    }

    // Serial.begin(9600);
    // while (true){
    //     for(auto x = 'a'; x <= 'z'; ++x)
    //         Serial.write(x);
    //     Serial.println();
    // }
}

// constexpr auto RxSize = 64;
// constexpr auto TxSize = 64;
// #define FH
// //#define OS
// //#define S1

// #ifdef FH
// FASTHAL_UART0(RxSize, TxSize);
// #ifdef S1
// FASTHAL_UART1(RxSize, TxSize);
// #endif
// #endif

// void setup(){
//     Adc::begin();
//     #ifdef FH
//     Uart0::begin(9600);
//     Uart0tx::begin();
//     Uart0rx::begin();
//     #ifdef S1
//     Uart1::begin(115000);
//     Uart1tx::begin();
//     Uart1rx::begin();
//     #endif
//     #else
//     Serial.begin(9600);    
//     #ifdef S1
//     Serial1.begin(115000);
//     #endif
//     #endif

// }

// #ifdef FH

// #ifdef OS
// auto Uart0os = MakeOutStream<Uart0tx>();
// auto Uart0tw = MakeTextWriter(Uart0os);
// #ifdef S1
// auto Uart1os = MakeOutStream<Uart1tx>();
// auto Uart1tw = MakeTextWriter(Uart1os);
// #endif

// #else

// auto Uart0tw = MakeTextWriter<Uart0tx>();
// #ifdef S1
// auto Uart1tw = MakeTextWriter<Uart1tx>();
// #endif

// #endif

// #endif

// template<typename T>
// void test(T& writer, uint8_t read){
//     writer.print("Hello from Arduino:");
//     writer.println(read);
//     writer.print("float: ");
//     writer.println((float)read, 2);
//     writer.print("double: ");
//     writer.println((double)read, 2);
// }

// void loop(){
//     auto read = arduino::AdcA0::read();
    
//     auto ms = Time::micros();
//     #ifdef FH
    
//     test(Uart0tw, read);
//     #ifdef S1
//     test(Uart1tw, read);
//     #endif
    
//     #else
    
//     test(Serial, read);
//     #ifdef S1
//     test(Serial1, read);
//     #endif

//     #endif
//     ms = Time::micros() - ms;
//     #ifdef FH
//     Uart0tw.print("Micros: ");
//     Uart0tw.println(ms);
//     #else
//     Serial.print("Micros: ");
//     Serial.println(ms);
//     #endif

//     Time::delayMs(1000);
// }
