#define RAW
#define MODE 0
// 0 - net args async - static args, static buffer (626 / 9 - 1 buf index, (2 buffer + 1 count + 1 status + 2 callback) = 6 args, 2 current va)
// 1 - net args async - static args, dynamic buffer (650 / 11 - 1 buf index, (2 buffer ptr + 1 count + 1 status + 2 callback) = 6 args, 2 buffer, 2 current va)
// 2 - net args async - dynamic args, static buffer (660 / 11 - 2 args ptr, 1 buf index, (2 buffer + 1 count + 1 status + 2 callback) = 6 args, 2 current va)

// 0 - sync (478 / 1 - 1 bytes left for RX)
// 1 - irq (370 / 5 - 1 mode set, 2 current val, 2 pending val)
// 2 - buffered (960 / 12 - 4 + 2 buffer, 2 callback, 1 state, 1 bytes left, 2 current val)
// 3 - buffered, always block (910 / 9 - 1+2 buffer, 2 callback, 1 state, 1 bytes left, 2 current val)
// 4 - async on buffers (670 / 9 - 2 buffer ptr, 2 callback, 1 size, 2 current va, 2 buffer)
// 5 - pointless sync on async lib (648 / 9 - 2 buffer ptr, 2 callback, 1 size, 2 buffer, 2 current va)
#include "fastduino.hpp"

using namespace fasthal;
using namespace fasthal::duino;

#ifndef RAW
static constexpr auto uart0 = uart<0>{};
static auto uart0tx = uart_buf_tx<uart<0>, 32>{};
FH_UART_TX(0, uart0tx);
#endif
static constexpr auto i2c0 = i2c<0>{};
static constexpr auto address = i2c_address_v<0x23>;

#if (MODE==0)
FH_STATIC(args, test_args<std::uint8_t[2]>{});
using args_base_t = args_t::type*;
#elif (MODE==1)
uint8_t bh1750_buf[2];
FH_STATIC(args, test_args<>{});
using args_base_t = args_t::type*;
#elif (MODE==2)
auto args_v = test_args<std::uint8_t[2]>{};
constexpr auto args = &args_v;
using args_t = decltype(args_v)*;
using args_base_t = args_t;
#endif

auto i2c0_h = i2c_net<i2c<0>, args_t>{};
FH_I2C(0, i2c0_h);

uint16_t bh1750_last_light;

void bh1750_read();

void bh1750_read_done(void*, args_base_t){
    i2c0_h.stop();

    if (args->status<i2c_result>() != i2c_result::done)
        return;

    auto result = static_cast<uint16_t>((*args)[0]);
    result <<= 8;
    result |= (*args)[1];

    bh1750_last_light = (result * 10U) / 12U;
    // something's here
    bh1750_read();
}

void bh1750_read(){
    // write sla    
    (*args)[0] = i2c_build_sla(i2c_mr, address);
    args->callback(bh1750_read_done);
    args->count(2);
    //args._count = 2;
    // address doesn't count as received byte and gets overwritten
    i2c0_h.start(*args);
}

void bh1750_set_mode(std::uint8_t mode);

void bh1750_mode_set(void*, args_base_t){    
    //println(uart0tx, "sm cb");
    // stop bus
    i2c0_h.stop();
    //println(uart0tx, "ms");
    // we don't repeat in other examples - for fairness
    bh1750_read();
    // if (done){
    //     //println(uart0tx, "mode set");
    //     bh1750_read();
    // } else {
    //     // print(uart0tx, "mode error: ");
    //     // println(uart0tx, static_cast<std::uint8_t>(i2c0_h.state()));
    //     bh1750_set_mode(0x10);
    // }
    // check if everything went fine    
}

void bh1750_set_mode(std::uint8_t mode){
    (*args)[0] = i2c_build_sla(i2c_mt, address);
    (*args)[1] = mode;
    args->count(2);
    args->callback(bh1750_mode_set);
    //args._count = 2;

    // address counts as send byte
    //println(uart0tx, "sm st");
    i2c0_h.start(*args);
}

int main(){    
    // cheat a little
    apply(        
        // activate internal pull ups for i2c
        set(ino<SDA>)
        , set(ino<SCL>)
        //Init i2c in master mode
        , i2c0.begin()
        //init uart
        #ifndef RAW
        , uart0.begin()
        #endif
        , enable(irq)
    );

    #if (MODE==1)
    args->buffer(bh1750_buf);
    #endif

    // set mode
    bh1750_set_mode(0x10);

    while (1){
        uint16_t light;
        {            
            auto mutex = no_irq{};
            light = bh1750_last_light;
        }
        #ifndef RAW
        print(uart0tx, "Lux: ");
        print(uart0tx, light);
        println(uart0tx, " lx");
        delay_ms(1000);
        #else
        TCNT0 = light;        
        #endif
    }
}