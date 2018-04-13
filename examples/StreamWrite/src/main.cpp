//#define UART1
//#include <Arduino.h>
#include "fasthal.hpp"

//#define TX_SIZE 32

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

#ifdef TX_SIZE
static constexpr auto uart0 = uart<0, ring_buffer_transmitter<TX_SIZE>::type>{};
FH_UART_TX(0, uart0);
#ifdef UART1
static constexpr auto uart1 = uart<0, ring_buffer_transmitter<TX_SIZE>::type>{};
FH_UART_TX(1, uart1);
#endif
#else
static constexpr auto uart0 = uart<0>{};
#ifdef UART1
static constexpr auto uart1 = uart<1>{};
#endif
#endif

int main(){
	apply(
        enable(irq),
        uart0.begin()
        #ifdef UART1
        , uart1.begin(baud_v<115000>)
        #endif
    );    
	while (true){
        auto read = read_(portC);
        
        testWrite(uart0.tx, read);
        #ifdef UART1
        testWrite(uart1.tx, read);
        #endif
	}
}