#define MODE 1

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

constexpr auto uart0 = uart<0>{};
#if (MODE == 0)
static constexpr auto uart0tx = uart_sync_tx<decltype(uart0)>{};
#else
static auto uart0tx = uart_buf_tx<decltype(uart0), 32>{};
FH_UART_TX(0, uart0tx);
#endif

template<typename T>
void testWrite(T& writer, uint8_t read) {
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
        uart0.begin()
        , enable(irq)
    );    
	while (true){
        auto read = read_(portC);
        
        testWrite(uart0tx, read);
	}
}