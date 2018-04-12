//#define UART1
//#include <Arduino.h>
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
}

// template<>
// ring_buffer<32> ring_buffer_transmitter<uart<0>, 32>::buffer{};

static constexpr auto uart0tx = ring_buffer_transmitter<uart<0>, 32>{};

FH_UART_TX(0, uart0tx);
//static constexpr auto uart0tx = sync_transmitter<uart<0>>{};

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
        
        testWrite(uart0tx, read);
        //testWrite2(bw, read);
        #ifdef UART1
        testWrite(uart1, read);
        #endif
	}
}