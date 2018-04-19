#define RAW
#define MODE 0
// 0 - net args async - static args, static buffer (610 / 9 - 1 buf index, (2 buffer + 1 count + 1 status + 2 callback) = 6 args, 2 current va)
// 1 - net args async - static args, dynamic buffer (640 / 11 - 1 buf index, (2 buffer ptr + 1 count + 1 status + 2 callback) = 6 args, 2 buffer, 2 current va)
// 2 - net args async - dynamic args, static buffer (644 / 11 - 2 args ptr, 1 buf index, (2 buffer + 1 count + 1 status + 2 callback) = 6 args, 2 current va)

// 0 - sync (478 / 1 - 1 bytes left for RX)
// 1 - irq (370 / 5 - 1 mode set, 2 current val, 2 pending val)
// 2 - buffered (960 / 12 - 4 + 2 buffer, 2 callback, 1 state, 1 bytes left, 2 current val)
// 3 - buffered, always block (910 / 9 - 1+2 buffer, 2 callback, 1 state, 1 bytes left, 2 current val)
// 4 - async on buffers (670 / 9 - 2 buffer ptr, 2 callback, 1 size, 2 current va, 2 buffer)
// 5 - pointless sync on async lib (648 / 9 - 2 buffer ptr, 2 callback, 1 size, 2 buffer, 2 current va)
#include "fastduino.hpp"
#include "dev/bh1750.hpp"

using namespace fasthal;
using namespace fasthal::duino;

#ifndef RAW
static constexpr auto uart0 = uart<0>{};
static auto uart0tx = uart_buf_tx<uart<0>, 32>{};
FH_UART_TX(0, uart0tx);
#endif
static constexpr auto i2c0 = i2c<0>{};

#if (MODE == 0 || MODE == 2)
auto args = test_args<std::uint8_t[2]>{};
#else
auto args = test_args{};
std::uint8_t bh1750_buf[2];
#endif

using args_base_t = decltype(args);

#if (MODE == 0 || MODE == 1)
FH_STATIC_PTR(args_ptr_t, args);
#else
using args_ptr_t = args_base_t*;
#endif

auto i2c0_h = i2c_net<i2c<0>, args_ptr_t>{};
FH_I2C(0, i2c0_h);
FH_STATIC_PTR(i2c0_ptr_t, i2c0_h);

auto light_sensor = dev::bh1750{i2c0_ptr_t{}};

uint16_t light_sensor_last;

void light_sensor_read(){       
    light_sensor.read(args.callback([](auto& a){
        light_sensor.read_end(a, light_sensor_last);
        // cycle
        light_sensor_read();
    }));
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
    args.buffer(bh1750_buf);
    #endif

    // set mode
    light_sensor.set_mode(dev::bh1750_mode::continuous_high_res_mode, args.callback([](auto& a) {
        light_sensor.set_mode_end(a);
        light_sensor_read();
    }));

    while (1){
        auto light = atomic_read(light_sensor_last);
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