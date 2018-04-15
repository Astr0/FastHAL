#define MODE 0
// 0 - sync
// 1 - irq
#include "fastduino.hpp"

using namespace fasthal;
using namespace fasthal::duino;

static constexpr auto uart0 = uart<0>{};
static constexpr auto uart0tx = uart_sync_tx<0>{};
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
#else
uint16_t bh170_last_light;
bool bh170_mode_set;

void handle_i2c(){
    switch(i2c0.state().state()){
        case i2c_s::ready:
            // continous reading
            i2c0.start();
            break;
        case i2c_s::select:
            if (bh170_mode_set)
                i2c0.select(address, i2c_mr);
            else
                i2c0.select(address, i2c_mt);
            break;
        case i2c_s::mt:
            // send mode
            i2c0.tx(0x10);
            break;
        case i2c_s::mr:


        case i2c_s::nack:
        case i2c_s::error:
            i2c0.stop();
            break;
    }
}

FH_I2C(0, handle_i2c);

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
    );

    #if (MODE == 0)
    // set mode
    bh1750_set_mode(0x10);
    #else
    i2c0.start();
    #endif

    while (1){
        #if (MODE == 0)
        auto light = bh1750_read(0x10);
        #else
        auto light = bh1750_last_light;
        #endif
        print(uart0tx, "Lux: ");
        print(uart0tx, light);
        println(uart0tx, " lx");
        delay_ms(1000);
    }
}