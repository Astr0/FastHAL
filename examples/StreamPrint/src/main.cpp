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
static constexpr auto uart0tx = ring_buffer_transmitter<uart<0>, TX_SIZE>{};
FH_UART_TX(0, uart0tx);
#ifdef UART1
static constexpr auto uart1tx = ring_buffer_transmitter<uart<1>, TX_SIZE>{};
FH_UART_TX(1, uart1tx);
#endif
#else
static constexpr auto uart0tx = sync_transmitter<uart<0>>{};
#ifdef UART1
static constexpr auto uart1tx = sync_transmitter<uart<1>>{};
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
        
        testPrint(uart0tx, read);
        //testWrite2(bw, read);
        #ifdef UART1
        testPrint(uart1tx, read);
        #endif
	}
}