#define RAW
#define MODE 0
#include "fastduino.hpp"
#include "dev/nrf24l01.hpp"

using namespace fasthal;
using namespace fasthal::duino;

#ifndef RAW
static constexpr auto uart0 = uart<0>{};
static auto uart0tx = uart_buf_tx<uart<0>, 32>{};
FH_UART_TX(0, uart0tx);
#endif

static constexpr auto spi0 = spi<0>{};
static constexpr auto spi0h = spi_sync<spi<0>>{};
FH_STATIC_PTR(spi0h_ptr_t, spi0h);

static constexpr auto ce_pin = ino<9>;
static constexpr auto cs_pin = ino<10>;

static constexpr auto nrf24 = dev::nrf24l01{ spi0h_ptr_t{}, ce_pin, cs_pin };


int main(){        
    apply(        
        // init radio pins
        makeOutput(ce_pin)
        , makeOutput(cs_pin)
        , nrf24.begin()
        // SPI output
        // enable pull up on SS / set it high - this should be done before other SPI stuff init
        , set(ino<SS>)
        , makeOutput(ino<SS>)
        , makeOutput(ino<MISO>)
        , makeOutput(ino<MOSI>)
        , spi0.begin()

        //init uart
        #ifndef RAW
        , uart0.begin()
        #endif
        , enable(irq)
    );

    while (1){

    }
}