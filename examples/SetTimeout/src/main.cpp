#define FH_TIME_T std::uint16_t
#include "fastduino.hpp"

using namespace fasthal;
using namespace fasthal::duino;

auto time = timer_tov_time<timer<0>>{};
FH_TIME(0, time);
FH_USE_TIME(time);


constexpr auto led = ino<13>;
constexpr auto uart0 = uart<0>{};
// constexpr auto uart0tx = uart_sync_tx<0>{};

// struct switch_executor{
//     using task_t = std::uint8_t;
//     static inline void execute(task_t task);
// };

auto kernel = ::fasthal::sys_kernel<2>{};

void blinkLed(){
    //println(uart0tx, "blink");
    // auto v = read_(led);
    // set_(led, !v);
    // kernel.setTimeout(v ? 500 : 1000, blinkLed);
    toggle_(led);
    kernel.setTimeout(500, blinkLed);
    if (PORTC != 0)
        kernel.clearTimeout(blinkLed);
}

// void printStuff(){
//     print(uart0tx, "I'm here: ");
//     println(uart0tx, time_ms());
//     kernel.setTimeout(5000, printStuff);
// }
// void switch_executor::execute(switch_executor::task_t task){
//     switch (task)
//     {
//         case 0: blinkLed(); break;
//     }    
// }


int main(){
    apply(
        makeOutput(led)
        //, uart0.begin()
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
    kernel.run();
}