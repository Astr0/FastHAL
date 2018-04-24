#ifndef FH_DEV_NRF24L01
#define FH_DEV_NRF24L01

#include "../std/std_types.hpp"
#include "../mp/holder.hpp"
#include "../hal/net.hpp"
#include "../utils/functions.h"

namespace fasthal::dev{
    enum class rf24_pa: std::uint8_t{
        min = 0,
        low = 1,
        high = 2,
        max = 3
    };

    enum class rf24_power: std::uint8_t{
        min = 0,
        max = 3
    };

    enum class rf24_rate: std::uint8_t{
        mbit1 = 0,
        mbit2 = 1,
        kbit250 = 2 
    };

    // config
    enum class rf24_c: std::uint8_t{
        irq_rx_dr  = 1 << 6,
        irq_tx_ds  = 1 << 5,
        irq_max_rt = 1 << 4,
        en_crc     = 1 << 3,
        crc16      = 1 << 2,
        pwr_up     = 1 << 1,
        prim_rx    = 1 << 0
    };
    FH_BITENUM_OPS(rf24_c, std::uint8_t);
    template<rf24_c V>
    static constexpr auto rf24_c_v = integral_constant<rf24_c, V>{};
    static constexpr auto rf24_c_def = rf24_c_v<rf24_c::en_crc | rf24_c::crc16>;

    // registers
    enum class rf24_reg: std::uint8_t{
        nrf_config		=0x00,
        en_aa			=0x01,
        en_rxaddr		=0x02,
        setup_aw		=0x03,
        setup_retr		=0x04,
        rf_ch			=0x05,
        rf_setup		=0x06,
        status			=0x07,
        observe_tx		=0x08,
        cd				=0x09,
        rx_addr_p0		=0x0a,
        rx_addr_p1		=0x0b,
        rx_addr_p2		=0x0c,
        rx_addr_p3		=0x0d,
        rx_addr_p4		=0x0e,
        rx_addr_p5		=0x0f,
        tx_addr		    =0x10,
        rx_pw_p0		=0x11,
        rx_pw_p1		=0x12,
        rx_pw_p2		=0x13,
        rx_pw_p3		=0x14,
        rx_pw_p4		=0x15,
        rx_pw_p5		=0x16,
        fifo_status	    =0x17,
        dynpd			=0x1c
    };

    // commands
    enum class rf24_cmd: std::uint8_t{
        read_register			=0x00,
        rpd						=0x09,
        write_register			=0x20,
        activate				=0x50,
        read_rx_pl_wid			=0x60,
        read_rx_payload			=0x61,
        write_tx_payload		=0xa0,
        write_ack_payload		=0xa8,
        write_tx_payload_no_ack	=0xb0,
        flush_tx				=0xe1,
        flush_rx				=0xe2,
        reuse_tx_pl				=0xe3,
        nop						=0xff
    };

    template <class TSpiPtr, class TCEPin, class TCSPin, typename TDefaultConfig = decltype(rf24_c_def)>
    class nrf24l01:
     mp::holder<TSpiPtr>,
     mp::holder<TDefaultConfig> {
        static_assert(!net_async_ptr<TSpiPtr>, "Not sync SPI interface");
        auto& spi() { return *(this->mp::holder<TSpiPtr>::get()); }     
        using net_args = net_args_sync<buffer_t, false>;

        static constexpr auto cs_pin = TCSPin{};
        static constexpr auto ce_pin = TCEPin{};

        constexpr rf24_c default_config() { return this->mp::holder<TDefaultConfig>::get(); }
        
        static constexpr std::uint8_t reg_mask = 0x1F;

        void transfer(buffer_t buf, bsize_t count){
            clear_(cs_pin);
            // timing
            delay_us(10);

            auto args = net_args{};
            args.buffer(buf);
            args.count(count);
            spi().transfer(args);

            set_(cs_pin);
            // timing
            delay_us(10);
        }
    public:
        constexpr nrf24l01(TSpiPtr spi, TCSPin cs, TCEPin ce, TDefaultConfig default_config = rf24_c_def): mp::holder<TDefaultConfig>(default_config){}

        void write_reg(rf24_reg reg, std::uint8_t v){

        }

        void config(rf24_c config){

        }

        // pins should be configured to output externally!
        static constexpr auto begin(){
            return combine(
                clear(ce_pin),
                set(cs_pin)
            );
        }
        static void begin_(){ apply(begin()); }
    };
}

#endif