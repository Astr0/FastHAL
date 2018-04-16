#define MODE 2
// 0 - sync
// 1 - irq
// 2 - buffered
#include "fastduino.hpp"

using namespace fasthal;
using namespace fasthal::duino;

static constexpr auto uart0 = uart<0>{};
static constexpr auto uart0tx = uart_sync_tx<uart<0>>{};
static constexpr auto i2c0 = i2c<0>{};
static constexpr auto address = i2c_address_v<0x23>;

#if (MODE == 0)
bool bh1750_set_mode(std::uint8_t mode){
    auto mt = start_mt_sync(i2c0, address);
    write(mt, mode);    
    return mt;
}

std::uint16_t bh1750_read(std::uint8_t mode){
    // if (!bh1750_set_mode(mode))
    //      return 0;

    auto mr = start_mr_sync(i2c0, address, 2);
    auto result = read_u16(mr);
    return mr ? ((result * 10U) / 12U) : 0U;
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

#else

auto i2c0_h = i2c_buf<decltype(i2c0), 16>{};
FH_I2C(0, i2c0_h);

uint16_t bh1750_last_light;

// void bh1750_read();

// void bh1750_read_done(){
//     // check ok        
//     auto result = read_u16(i2c0_h);
//     i2c0_h.stop();
//     bh1750_last_light = i2c0_h ? ((result * 10U) / 12U) : 0U;
//     bh1750_read();
// }

// void bh1750_read(){
//     i2c0_h.start_mr(address, 2, bh1750_read_done);
// }

void bh1750_set_mode(std::uint8_t mode);

void bh1750_mode_set(){
    if (!i2c0_h.master_done()){
        print(uart0tx, "mode error: ");
        println(uart0tx, static_cast<std::uint8_t>(i2c0_h.state()));
        // stop bus
        i2c0_h.stop();
        bh1750_set_mode(0x10);
    } else {
        i2c0_h.stop();
        println(uart0tx, "mode set");
    }
    // check if everything went fine
    //bh1750_read();
}

void bh1750_set_mode(std::uint8_t mode){
    i2c0_h.start_mt(address);
    write(i2c0_h, mode);    
    i2c0_h.flush(bh1750_mode_set);
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
        , uart0.begin()
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
        {
            auto mutex = no_irq{};
            light = bh1750_last_light;
        }
        #endif
        print(uart0tx, "Lux: ");
        print(uart0tx, light);
        println(uart0tx, " lx");
        delay_ms(1000);
    }
}