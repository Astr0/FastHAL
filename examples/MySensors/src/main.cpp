#include "fastduino.hpp"
#include "libs/mysensors.hpp"

using namespace fasthal;
using namespace fasthal::duino;
using namespace fasthal::dev;
using namespace fasthal::mysensors;

//static constexpr auto uart0 = uart<0>{};
//static auto uart0tx = uart_buf_tx<uart<0>, 32>{};
//static auto uart0rx = uart_buf_rx<uart<0>, 32>{};
//FH_UART_TX(0, uart0tx);
//FH_UART_RX(0, uart0rx);

static constexpr auto spi0 = spi<0>{};
static constexpr auto spi0h = spi_sync<spi<0>>{};

static constexpr auto ce_pin = ino<9>;
static constexpr auto cs_pin = ino<SS>;

// declare some radio on the SPI
static constexpr auto radio = nrf24l01{ FH_SPTR(spi0h), ce_pin, cs_pin };
// context
static constexpr auto context = mycontext{};
// and radio RAW transport for mysensors
static constexpr auto ntransport = ntransport_rf24{ FH_SPTR(context), FH_SPTR(radio) };
// direct transport
static constexpr auto transport = direct_transport{ FH_SPTR(context), FH_SPTR(ntransport) };
// and UART gateway transport
//static auto gtransport = gtransport_streams{ FH_SPTR(uart0tx), FH_SPTR(uart0rx) };
// and gateway with transport and gateway transport
//static constexpr auto gateway = mygateway{ FH_SPTR(transport), FH_SPTR(gtransport) };
static constexpr auto node = mynode{ FH_SPTR(transport) };

void debug_radio(){
    // print(uart0tx, "config: ");
    // print(uart0tx, radio.config());
    // print(uart0tx, "channel: ");
    // print(uart0tx, radio.channel());
    // print(uart0tx, " rf: ");
    // println(uart0tx, radio.rf());
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
        //, uart0.begin()
        , enable(irq)
    );

    if (!node.begin()) {
        // can't turn led - it's on SCK pin =/
        //print(uart0tx, "node error. ");
        //debug_radio();
        while (1);
    }

    while (1){
        auto msg = mymessage { };
        node.update(msg);
        // if (gateway.update(msg)){
        //     print(uart0tx, "got message");
        // }
    }
}