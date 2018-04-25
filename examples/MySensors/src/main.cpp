#include "fastduino.hpp"
#include "libs/mysensors.hpp"

using namespace fasthal;
using namespace fasthal::duino;
using namespace fasthal::dev;
using namespace fasthal::mysensors;

static constexpr auto uart0 = uart<0>{};
static auto uart0tx = uart_buf_tx<uart<0>, 32>{};
FH_UART_TX(0, uart0tx);

static constexpr auto spi0 = spi<0>{};
static constexpr auto spi0h = spi_sync<spi<0>>{};

static constexpr auto ce_pin = ino<9>;
static constexpr auto cs_pin = ino<SS>;

static constexpr auto radio = nrf24l01{&spi0h, ce_pin, cs_pin };
static constexpr auto transport = transport_rf24{&radio};
static constexpr auto gtransport = gtransport_uart{&uart0tx};
static constexpr auto gateway = mygateway{&transport, &gtransport};

void debug_radio(){
    #ifndef RAW
    print(uart0tx, "config: ");
    print(uart0tx, radio.config());
    print(uart0tx, "channel: ");
    print(uart0tx, radio.channel());
    print(uart0tx, " rf: ");
    println(uart0tx, radio.rf());
    #endif
}

void init(){
    if (!gateway.begin())
    {
        // can't turn led - it's on SCK pin =/
        #ifndef RAW
        print(uart0tx, "gateway error. ");
        debug_radio();
        #endif
        return;
    }
    #ifndef RAW
    println(uart0tx, "gateway ok");
    #endif
}

int main(){        
    apply(        
        // init radio pins
        makeOutput(ce_pin)
        , makeOutput(cs_pin)
        , radio.begin()
        // SPI output
        // enable pull up on SS / set it high - this should be done before other SPI stuff init
        , set(ino<SS>)
        , makeOutput(ino<SS>)
        , makeOutput(ino<SCK>)
        , makeOutput(ino<MOSI>)
        , spi0.begin(spi_clock_v<2'000'000UL>)
        //init uart
        , uart0.begin()
        , enable(irq)
    );

    init();

    while (1){

    }
}