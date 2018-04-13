//#define UART1
//#include <Arduino.h>

//#define RX_SIZE 64
//#define TX_SIZE 64

#include "fasthal.hpp"

//#define _FLASH(x) FH_FLASH(x)
#define _FLASH(x) x

using namespace fasthal;
using namespace fasthal::avr;

#ifdef TX_SIZE
static constexpr auto uart0 = uart<0, ring_buffer_transmitter<TX_SIZE>::type>{};
FH_UART_TX(0, uart0);
#ifdef UART1
static constexpr auto uart1 = uart<1, ring_buffer_transmitter<TX_SIZE>::type>{};
FH_UART_TX(1, uart1);
#endif
#else
static constexpr auto uart0 = uart<0>{};
#ifdef UART1
static constexpr auto uart1 = uart<1>{};
#endif
#endif

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

int main(){
	apply(
        enable(irq),
        uart0.begin()
        #ifdef UART1
        , uart1.begin(baud_v<115000>)
        #endif
    );    

    std::uint8_t i = 0;
	while (true){
        auto read = i++;
        
        testPrint(uart0.tx, read);
        #ifdef UART1
        testPrint(uart1.tx, read);
        #endif
	}
}