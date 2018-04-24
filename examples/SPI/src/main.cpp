#define RAW
#include "fastduino.hpp"
#include "dev/nrf24l01.hpp"

using namespace fasthal;
using namespace fasthal::duino;
using namespace fasthal::dev;

#ifndef RAW
static constexpr auto uart0 = uart<0>{};
static auto uart0tx = uart_buf_tx<uart<0>, 32>{};
FH_UART_TX(0, uart0tx);
#endif

static constexpr auto spi0 = spi<0>{};
static constexpr auto spi0h = spi_sync<spi<0>>{};
FH_STATIC_PTR(spi0h_ptr_t, spi0h);

static constexpr auto ce_pin = ino<9>;
static constexpr auto cs_pin = ino<SS>;

static constexpr auto radio = nrf24l01{ spi0h_ptr_t{}, ce_pin, cs_pin };

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

void init_radio(){
    // MY Sensors sequence
    // power up
    radio.config(rf24_c::pwr_up);
    delay_ms(5);
    debug_radio();

    constexpr auto addr_width = 5;

    // set address width
    radio.aw(addr_width);
    // set retransmit
    radio.retr(rf24_ard::_1500us, 15);
    // set channel
    radio.channel(76);
    // setup rf
    radio.rf(rf24_pwr::medium, rf24_rate::_250kbps);
    // toggle features (necessary on some clones and non-P versions)
    radio.enable_features();
    // enable dynamic payload
    radio.feature(rf24_f::dynamic_payload);

    if (!radio.sanity_check(rf24_pwr::medium, rf24_rate::_250kbps, 76))
    {
        // can't turn led - it's on SCK pin =/
        #ifndef RAW
        print(uart0tx, "radio error. ");
        debug_radio();
        #endif
        return;
    }
    #ifndef RAW
    println(uart0tx, "radio ok");
    #endif
    static constexpr auto broadcast_pipe = 1;
    // enable broadcasting pipe
    radio.rx_pipes(1 << broadcast_pipe);
	// disable AA on all pipes, activate when node pipe set
	radio.auto_ack(0x00);
	// enable dynamic payloads on used pipes
	radio.dynamic_payload((1 << broadcast_pipe) | 1 << 0);
    
    static constexpr std::uint8_t broadcast_address = 255;

    std::uint8_t base_id[addr_width] = {0x00,0xFC,0xE1,0xA8,0xA8};

    // listen to broadcast pipe
	base_id[0] = broadcast_address;
    radio.rx_address(broadcast_pipe, base_id, broadcast_pipe > 1 ? 1 : addr_width);
    
	// pipe 0, set full address, later only LSB is updated
    radio.rx_address(0, base_id, addr_width);
    // set tx address
    radio.tx_address(base_id, addr_width);
	
	// reset FIFO
	radio.rx_flush();
	radio.tx_flush();

	// reset interrupts
	radio.status(rf24_s::tx_ds | rf24_s::max_rt | rf24_s::rx_dr);
}

int main(){        

    // SPI output
    // enable pull up on SS / set it high - this should be done before other SPI stuff init
    // set_(ino<SS>);
    // makeOutput_(ino<SS>);
    
    // //SPCR = _BV(MSTR) | _BV(SPE) | 1;
    // //SPSR = 1;

    // spi0.begin_(spi_clock_v<2'000'000UL>);
    // makeOutput_(ino<SCK>);
    // makeOutput_(ino<MOSI>);

    // // init radio pins
    // makeOutput_(ce_pin);
    // makeOutput_(cs_pin);
    // radio.begin_();

    // //init uart
    // #ifndef RAW
    // uart0.begin_();
    // #endif
    // enable_(irq);

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
        #ifndef RAW
        , uart0.begin()
        #endif
        , enable(irq)
    );

    init_radio();

    while (1){

    }
}