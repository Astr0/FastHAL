#define MODE 1
// 0 - sync
// 1 - async
#include "fasthal.hpp"

//#define _FLASH(x) FH_FLASH(x)
#define _FLASH(x) x

using namespace fasthal;
using namespace fasthal::avr;

template<typename T>
void testPrint(T& writer, uint8_t read){
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
#if (MODE == 0)
static constexpr auto uart0tx = uart_sync_tx<uart<0>>{};
#else
auto uart0tx = uart_buf_tx<uart<0>, 32>{};
FH_UART_TX(0, uart0tx);
#endif


int main(){
	apply(        
        uart0.begin()
        , enable(irq)
    );    

    std::uint8_t i = 0;
	while (true){
        auto read = i++;
        
        testPrint(uart0tx, read);

        delay_ms(1000);
	}
}