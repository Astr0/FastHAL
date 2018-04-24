#ifndef FH_DEV_NRF24L01
#define FH_DEV_NRF24L01

#include "../std/std_types.hpp"
#include "../mp/holder.hpp"
#include "../hal/net.hpp"
#include "../utils/functions.h"

namespace fasthal::dev{
    // power
    enum class rf24_pwr: std::uint8_t{
        low    = 0b001,  // -18dBm 7mA
        medium = 0b011,  // -12dBm 7.5mA
        high   = 0b101,  //  -6dBM 9mA
        max    = 0b111   //   0dBm 11.3mA
    };

    // rate
    enum class rf24_rate: std::uint8_t{
        // we "avoid" PLL_LOCK here when making mask
        _1mbps   = 0b000 << 3,
        _2mbps   = 0b001 << 3,
        _250kbps = 0b100 << 3 
    };

    // address width
    enum class rf24_aw: std::uint8_t{
        _3 = 0b01,
        _4 = 0b10,
        _5 = 0b11
    };

    // auto retry delay
    enum class rf24_ard: std::uint8_t{
        _250us = 0 << 4,
        _500us = 1 << 4,
        _750us = 2 << 4,
        _1000us = 3 << 4,
        _1250us = 4 << 4,
        _1500us = 5 << 4,
        _1750us = 6 << 4,
        _2000us = 7 << 4,
        _2250us = 8 << 4,
        _2500us = 9 << 4,
        _2750us = 10 << 4,
        _3000us = 11 << 4,
        _3250us = 12 << 4,
        _3500us = 13 << 4,
        _3750us = 14 << 4,
        _4000us = 15 << 4
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

    // features
    enum class rf24_f: std::uint8_t{
        dynamic_payload = 1 << 2,
        ack_payload     = 1 << 1,
        dynamic_ack     = 1 << 0
    };
    FH_BITENUM_OPS(rf24_f, std::uint8_t);

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
        dynpd			=0x1c,
        feature         =0x1d
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
        auto& spi() const { return *(this->mp::holder<TSpiPtr>::get()); }     
        using net_args = net_args_sync<buffer_t, false>;

        static constexpr auto cs_pin = TCSPin{};
        static constexpr auto ce_pin = TCEPin{};

        static constexpr auto cmd_nop = static_cast<std::uint8_t>(rf24_cmd::nop);

        constexpr rf24_c default_config() const{ return this->mp::holder<TDefaultConfig>::get(); }
        
        // we don't need this cause we use ENUM :D
        // static constexpr std::uint8_t reg_mask = 0x1F;

        void transfer(buffer_t buf, bsize_t count) const{
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

        std::uint8_t transfer(std::uint8_t cmd, std::uint8_t v) const{
            std::uint8_t buf[2];
            // prepare write register command
            buf[0] = cmd;
            buf[1] = v;
            transfer(buf, 2);
            return buf[1];
        }

        // std::uint8_t read_raw(std::uint8_t cmd) const{
        //     std::uint8_t buf[2];
        //     // prepare read register command
        //     buf[0] = cmd;
        //     buf[1] = cmd_nop;
        //     transfer(buf, 2);
        //     // discard first byte (status)
        //     return buf[1];
        // }
        
        void write_reg(rf24_reg reg, std::uint8_t v) const{
            transfer(
                static_cast<std::uint8_t>(rf24_cmd::write_register) | static_cast<std::uint8_t>(reg),
                v
            );
        }

        std::uint8_t read_reg(rf24_reg reg) const{
            return transfer(static_cast<std::uint8_t>(rf24_cmd::read_register) | static_cast<std::uint8_t>(reg), cmd_nop);
        }
    public:
        constexpr nrf24l01(TSpiPtr spi, TCEPin ce, TCSPin cs, TDefaultConfig default_config = rf24_c_def): mp::holder<TDefaultConfig>(default_config){}

        void config(rf24_c config) const{
            // default config get's written every time
            write_reg(rf24_reg::nrf_config, static_cast<std::uint8_t>(default_config() | config));
        }
        std::uint8_t config() const{
            return read_reg(rf24_reg::nrf_config);
        }        

        // set address width
        void aw(rf24_aw v) const{
            write_reg(rf24_reg::setup_aw, static_cast<std::uint8_t>(v));
        }

        // set auto-retransmit, count = 0 == disabled)
        void retr(rf24_ard delay, std::uint8_t count) const{
            write_reg(rf24_reg::setup_retr, static_cast<std::uint8_t>(delay) | count);
        }

        // set channel
        void channel(std::uint8_t channel) const{
            write_reg(rf24_reg::rf_ch, channel);
        }
        std::uint8_t channel() const{
            return read_reg(rf24_reg::rf_ch);
        }


        // set rf power and data rate
        void rf(rf24_pwr power, rf24_rate rate) const{
            write_reg(rf24_reg::rf_setup, static_cast<std::uint8_t>(power) | static_cast<std::uint8_t>(rate));
        }
        std::uint8_t rf() const{
            return read_reg(rf24_reg::rf_setup);
        }

        // copied from my_sensors, purpose and functionality unknown
        void enable_features() const{
            transfer(static_cast<std::uint8_t>(rf24_cmd::activate), 0x73);
        }

        // set features
        void feature(rf24_f f) const{
            write_reg(rf24_reg::feature, static_cast<std::uint8_t>(f));
        }
        

        bool sanity_check(rf24_pwr power, rf24_rate rate, std::uint8_t channel) const{
            return (read_reg(rf24_reg::rf_setup) == (static_cast<std::uint8_t>(power) | static_cast<std::uint8_t>(rate))) &&
                (this->channel() == channel);
        }

        // pins should be configured to output externally!
        static constexpr auto begin() {
            return combine(
                clear(ce_pin),
                set(cs_pin)
            );
        }
        static void begin_() { apply(begin()); }
    };
}

#endif