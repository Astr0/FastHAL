#define FH_TIME_T std::uint16_t
#include "fastduino.hpp"

using namespace fasthal;
using namespace fasthal::duino;

auto time = timer_tov_time<timer<0>>{};
FH_TIME(0, time);
FH_USE_TIME(time);


constexpr auto led = ino<13>;
constexpr auto uart0 = uart<0>{};
auto uart0tx = uart_buf_tx<uart<0>, 32>{};
FH_UART_TX(0, uart0tx);

auto kernel = sys_kernel<3>{};

void blinkLed(){
    println(uart0tx, "blink");
    toggle_(led);
    kernel.setTimeout(500, blinkLed);
}

void printStuff1(){
    if (uart0tx.available() < 16){
        println(uart0tx, "1:cya");        
    } else {
        print(uart0tx, "time1: ");
        println(uart0tx, time_ms());
    }
    kernel.setTimeout(1000, printStuff1);
}

void printStuff2(){
     if (uart0tx.available() < 16){
        println(uart0tx, "2:cya");        
    } else {
        print(uart0tx, "time2: ");
        println(uart0tx, time_ms());
    }
    kernel.setTimeout(3000, printStuff2);
}

int main(){
    apply(
        makeOutput(led)
        , uart0.begin()
        , timer<0>::begin()
        , time.begin()        
        , enable(irq)
    );
    // auto start = time_ms();
    // while (true){
    //     toggle_(led);
    //     while (time_ms() - start < 500);
    //     start = time_ms();
    // }
    //kernel.setTimeout(0, blinkLed);
    //kernel.setTimeout(5000, printStuff);
    blinkLed();
    printStuff1();    
    printStuff2();
    kernel.run();
}