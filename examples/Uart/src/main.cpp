#define MODE 5
// 1 - echo sync (222/0/0)
// 2 - a...z sync (230/0/0)
// 3 - echo async (242/0/0)
// 4 - a...z asyc (276/0/1 - 1 char)
// 5 - echo net async (418/0/3)
// 6 - a..z net async (330/0/29)
// 10 - echo arduino (1088/18/157)
// 11 - a..z arduino (1104/22/157)


#include "fasthal.hpp"

#if (MODE == 10 || MODE == 11)
#include <Arduino.h>
#endif

using namespace fasthal;

static constexpr auto uart0 = uart<0>{};


#if (MODE == 3)
void uart0_rx(){
    auto ok = uart0.rx_ok();
    auto v = uart0.rx();
    if (ok)
        tx_sync(uart0, v);
}
FH_UART_RX(0, uart0_rx);
#elif (MODE == 4)
char c;
void uart0_tx(){
    switch(c){
        case 'z' + 1: 
            uart0.tx(10);
            break;
        case 'z' + 2:
            uart0.tx(13);
            c = 'a';
            return;
        default:
            uart0.tx(c);
            break;
    }
    c++;
}
FH_UART_TX(0, uart0_tx);
#elif (MODE==5)

std::uint8_t _c;
struct one_byte_args{
    //std::uint8_t _c;
    static buffer_t buffer() { return &_c; }
    static bsize_t count() { return 1; }
};

struct tx_args: one_byte_args{
    void operator()(){}
};

struct rx_args: one_byte_args{
    void operator()();
};

auto txargs = tx_args{};
FH_STATIC_PTR(tx_args_ptr_t, txargs);
auto uart0_tx = uart_async_tx<uart<0>, tx_args_ptr_t>{};
FH_UART_TX(0, uart0_tx);

auto rxargs = rx_args{};
FH_STATIC_PTR(rx_args_ptr_t, rxargs);
auto uart0_rx = uart_async_rx<uart<0>, rx_args_ptr_t>{};
FH_UART_RX(0, uart0_rx);

void rx_args::operator()(){
    //tx_sync(uart0, _c);
    while (!ready_(uart0.irq_txr));
    uart0_tx.tx(txargs);
    uart0_rx.rx(rxargs);
}

#elif (MODE==6)

struct my_args{
    static constexpr auto size = ('z' - 'a' + 1) + 2;
    std::uint8_t _buf[size];

    void init(){
        std::uint8_t i = 0;
        for (; i <= ('z' - 'a'); ++i)
            _buf[i] = 'a' + i;
        _buf[i++] = 10;
        _buf[i++] = 13;
    }
    
    decltype(_buf)& buffer() { return _buf; }
    bsize_t count() { return size; }
    void operator()();
};

auto args = my_args{};
FH_STATIC_PTR(args_ptr_t, args);

auto uart0_tx = uart_async_tx<uart<0>, args_ptr_t>{};
FH_UART_TX(0, uart0_tx);

void my_args::operator()(){
    uart0_tx.tx(args);
}

#endif

int main(){
    enable_(irq);
    #if (MODE == 10 || MODE == 11)
    Serial.begin(9600);
    #else
    uart0.begin_(baud_v<9600>);
    #endif

    #if (MODE == 5)
    uart0_rx.rx(rxargs);
    #elif (MODE==6)    
    args.init();
    args();
    #elif (MODE == 4)
    c = 'a';
    uart0.tx_start();
    #endif

    while (true){
        #if (MODE == 1)
        try_rx(uart0, [](auto c){ tx_sync(uart0, c); });
        #elif (MODE == 2)
        for(auto x = 'a'; x <= 'z'; ++x)
            tx_sync(uart0, x);
        tx_sync(uart0, 10);
        tx_sync(uart0, 13);
        #elif (MODE == 10)
        if (Serial.available())
            Serial.write(Serial.read());
        #elif (MODE==11)
        for(auto x = 'a'; x <= 'z'; ++x)
             Serial.write(x);
         Serial.println();        
        #endif
    }
}
