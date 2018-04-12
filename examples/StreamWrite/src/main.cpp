//#define UART1
//#include <Arduino.h>

#define RX_SIZE 64
#define TX_SIZE 32

//#define FH_UART0_RX RX_SIZE
#define FH_UART0_TX TX_SIZE

#ifdef UART1
#define FH_UART1_RX RX_SIZE
#define FH_UART1_TX TX_SIZE
#endif

#include "fasthal.hpp"

#define _FLASH(x) x

using namespace fasthal;
using namespace fasthal::avr;

struct test{
    uint32_t test1;
    uint8_t test2;
    uint8_t test3;
    uint8_t test4;
};

test a;

template<typename T>
void testWrite(T writer, uint8_t read) {
    write(writer, _FLASH("Hello from Arduino:"));
    write(writer, read);
    write(writer, _FLASH("uint32_t: "));
    write(writer, (uint32_t)read);
    write(writer, _FLASH("float: "));
    write(writer, (float)read);
    write(writer, _FLASH("double: "));
    write(writer, (double)read);
    write(writer, _FLASH("struct: "));
    a = {PORTB, read, PORTC, read};
    write(writer, reinterpret_cast<uint8_t*>(&a), sizeof(test));
    // wait for transfer of unsafe global stuff
    flush(writer);
    //flush(writer.target);
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
	while (true){
        auto read = read_(portC);
        
        testWrite(uart0, read);
        //testWrite2(bw, read);
        #ifdef UART1
        testWrite(uart1, read);
        #endif
	}
}