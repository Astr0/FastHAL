#define RAW
#define MODE 4
// 0 - sync (478 / 1 - 1 bytes left for RX)
// 1 - irq (370 / 5 - 1 mode set, 2 current val, 2 pending val)
// 2 - buffered (960 / 12 - 4 + 2 buffer, 2 callback, 1 state, 1 bytes left, 2 current val)
// 3 - buffered, always block (910 / 9 - 1+2 buffer, 2 callback, 1 state, 1 bytes left, 2 current val)
// 4 - async on buffers (670 / 9 - 2 tx buffer, 2 callback, 1 size, 2 current va, 2 buffer)
// 5 - pointless sync on async lib (648 / 9 - 2 tx buffer, 2 callback, 1 size, 2 current va, 2 buffer)
#include "fastduino.hpp"

using namespace fasthal;
using namespace fasthal::duino;

static constexpr auto uart0 = uart<0>{};
#ifndef RAW
static auto uart0tx = uart_buf_tx<uart<0>, 32>{};
FH_UART_TX(0, uart0tx);
#endif
static constexpr auto i2c0 = i2c<0>{};
static constexpr auto address = i2c_address_v<0x23>;

#if (MODE == 0)
auto i2c0_h = i2c_sync<i2c<0>>{};

void bh1750_set_mode(std::uint8_t mode){
    i2c0_h.start_mt(address);
    write(i2c0_h, mode);
    i2c0_h.stop();
}

std::uint16_t bh1750_read(std::uint8_t mode){
    i2c0_h.start_mr(address, 2);
    auto result = read_u16(i2c0_h);
    auto ok = i2c0_h.stop().mr_ok_done();
    return ok ? ((result * 10U) / 12U) : 0U;
}
#elif (MODE == 1)
uint16_t bh1750_last_light;
bool bh1750_mode_set;
uint16_t bh1750_current;

void handle_i2c(){
    switch(i2c0.state().state()){
        // case i2c_s::ready:
        //     // continous reading
        //     i2c0.start();
        //     break;
        case i2c_s::m_start:
            if (bh1750_mode_set)
                i2c0.select(i2c_mr, address);
            else
                i2c0.select(i2c_mt, address);
            break;
        case i2c_s::mt:
            // send mode
            i2c0.tx(0x10);
            break;
        case i2c_s::mt_write:
            bh1750_mode_set = true;
            i2c0.stop_start();
            break;
        case i2c_s::mr:
            i2c0.rx_ask(i2c_more);
            break;
        case i2c_s::mr_read:
            bh1750_current = i2c0.rx();
            i2c0.rx_ask(i2c_last);
            break;
        case i2c_s::mr_readl:
            bh1750_current = (bh1750_current << 8) | i2c0.rx();
            bh1750_last_light = bh1750_current;
            i2c0.stop_start();
            break;
        default:
            i2c0.stop();
            while (i2c0.stopping());
            i2c0.start();     
            break;
    }
}

FH_I2C(0, handle_i2c);

#elif (MODE == 2) || (MODE == 3)

#if (MODE == 2)
auto i2c0_h = i2c_buf<decltype(i2c0), 4>{};
#else
auto i2c0_h = i2c_buf<decltype(i2c0), 1>{};
#endif
FH_I2C(0, i2c0_h);

uint16_t bh1750_last_light;

void bh1750_read();

void bh1750_read_done(){
    // if (i2c0_h.state() == i2c_buf_s::mr_block)
    //     println(uart0tx, "block");
    // check ok        
    auto result = read_u16(i2c0_h);

    auto done = i2c0_h.stop();

    bh1750_last_light = done ? ((result * 10U) / 12U) : 0U;
    // something's here
    bh1750_read();
}

void bh1750_read(){
     i2c0_h.start_mr(address, 2, bh1750_read_done);
}

void bh1750_set_mode(std::uint8_t mode);

void bh1750_mode_set(){
    // stop bus
    i2c0_h.stop();
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
    i2c0_h.start_mt(address);
    write(i2c0_h, mode);    
    i2c0_h.flush(bh1750_mode_set);
}

#elif (MODE == 4) || (MODE == 5)
auto i2c0_h = i2c_async<decltype(i2c0)>{};
#if (MODE == 4)
FH_I2C(0, i2c0_h);
#endif

uint16_t bh1750_last_light;
uint8_t bh1750_buf[2];

void bh1750_read();

void bh1750_read_done(i2c_result r){
    i2c0_h.stop();

    // print(uart0tx, "got: ");
    // print(uart0tx, static_cast<std::uint8_t>(r));
    // print(uart0tx, ' ');
    // print(uart0tx, static_cast<std::uint8_t>(bh1750_buf[0]));
    // print(uart0tx, ' ');
    // println(uart0tx, static_cast<std::uint8_t>(bh1750_buf[1]));
    if (r != i2c_result::done)
        return;
    
    auto result = static_cast<uint16_t>(bh1750_buf[0]);
    result <<= 8;
    result |= bh1750_buf[1];

    bh1750_last_light = (result * 10U) / 12U;
    #if (MODE==4)
    // something's here
    bh1750_read();
    #endif
}

void bh1750_read(){
    // write sla    
    bh1750_buf[0] = i2c_build_sla(i2c_mr, address);
    // address doesn't count as received byte and gets overwritten
    i2c0_h.start(bh1750_buf, 2, bh1750_read_done);
}

void bh1750_set_mode(std::uint8_t mode);

void bh1750_mode_set(i2c_result r){
    // stop bus
    i2c0_h.stop();
    // we don't repeat in other examples - for fairness
    #if (MODE==4)
    bh1750_read();
    #endif
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
    bh1750_buf[0] = i2c_build_sla(i2c_mt, address);
    bh1750_buf[1] = mode;
    // address counts as send byte
    i2c0_h.start(bh1750_buf, 2, bh1750_mode_set);
}

#endif

int main(){    
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

    #if (MODE == 1)
    i2c0.start();
    #else
    // set mode
    bh1750_set_mode(0x10);
    #endif

    while (1){
        uint16_t light;
        #if (MODE == 0)
        light = bh1750_read(0x10);
        #else
        #if (MODE==5)
        bh1750_read();
        #endif
        {            
            auto mutex = no_irq{};
            light = bh1750_last_light;
        }
        #endif
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