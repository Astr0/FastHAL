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
}

int main(){
	apply(
        enable(irq),
        uart0.begin()
    );    
	while (true){
        auto read = read_(portC);
        
        testWrite(uart_sync_tx<0>{}, read);
	}
}