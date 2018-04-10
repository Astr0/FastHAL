//#define UART1
//#include <Arduino.h>

#define RX_SIZE 64
#define TX_SIZE 64

#define FH_UART0_RX RX_SIZE
#define FH_UART0_TX TX_SIZE

#ifdef UART1
#define FH_UART1_RX RX_SIZE
#define FH_UART1_TX TX_SIZE
#endif

#include "fasthal.hpp"

#define _FLASH(x) FH_FLASH(x)
//#define _FLASH(x) x

using namespace fasthal;
using namespace fasthal::avr;

template<typename T>
void testPrint(T writer, uint8_t read){
    print(writer, _FLASH("Hello from Arduino:"));
    println(writer, read);
    print(writer, _FLASH("uint32_t: "));
    println(writer, (uint32_t)read);
    print(writer, _FLASH("float: "));
    println(writer, (float)read);
    print(writer, _FLASH("double: "));
    println(writer, (double)read);
}

// template<typename T>
// void testWrite2(T writer, uint8_t read){
//     writer.write("Hello from Arduino:");
//     writer.write(read);
//     writer.write("uint32_t: ");
//     writer.write((uint32_t)read);
//     writer.write("float: ");
//     writer.write((float)read);
//     writer.write("double: ");
//     writer.write((double)read);
// }

// template<class TUart>
// struct Test2Adapter{
//     static bool write(std::uint8_t b){
//         return fasthal::write(TUart{}, b);
//     }
// };

int main(){
	apply(
        enable(irq),
        begin(uart0)
        #ifdef UART1
        , begin(uart1, baud_v<115000>)
        #endif
    );    
    //begin_(uart1, baud_v<115000>);
    //auto bw = BinaryWriter{Test2Adapter<decltype(uart0)>{}};
    std::uint8_t i = 0;
	while (true){
        auto read = i++;
        
        testPrint(uart0, read);
        //testWrite2(bw, read);
        #ifdef UART1
        testPrint(uart1, read);
        #endif
	}
}