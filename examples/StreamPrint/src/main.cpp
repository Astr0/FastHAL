#include "fasthal.hpp"

//#define _FLASH(x) FH_FLASH(x)
#define _FLASH(x) x

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

static constexpr auto uart0 = uart<0>{};

int main(){
	apply(
        enable(irq),
        uart0.begin()
    );    

    std::uint8_t i = 0;
	while (true){
        auto read = i++;
        
        testPrint(uart_sync_tx<0>{}, read);
	}
}