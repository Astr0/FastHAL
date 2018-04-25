#ifndef FH_LIBS_MYSENSORS_TRANSPORT_RF24_H_
#define FH_LIBS_MYSENSORS_TRANSPORT_RF24_H_

#include "../../dev/nrf24l01.hpp"
#include "../../std/std_types.hpp"
#include "../../mp/holder.hpp"

namespace fasthal::mysensors{
    struct rf24_default_config{        
        static constexpr auto power_on_delay_ms = 5;
        static constexpr auto address_width = 5;
        static constexpr auto retr_delay = dev::rf24_ard::_1500us;
        static constexpr auto retr_attempts = 15;
        static constexpr auto channel = 76;
        static constexpr auto power = dev::rf24_pwr::max;
        static constexpr auto rate = dev::rf24_rate::_250kbps;
        static constexpr auto broadcast_pipe = 1;
        static constexpr auto broadcast_address = 255;
    };

    template<typename TRF24Ptr, class TConfig = rf24_default_config>
    class transport_rf24:
        mp::holder<TRF24Ptr>
    {
        using config_t = TConfig;
        static constexpr auto addr_width = config_t::address_width;
        static constexpr auto broadcast_pipe = config_t::broadcast_pipe;

        auto& rf24() const{return *(this->mp::holder<TRF24Ptr>::get()); }
    public:        
        constexpr transport_rf24(TRF24Ptr rf24, TConfig config = rf24_default_config{}):
            mp::holder<TRF24Ptr>(rf24) {}

        bool begin() const{
            using namespace dev;
            auto& radio = rf24();

            // MY Sensors sequence
            // power up
            radio.config(rf24_c::pwr_up);
            delay_ms(config_t::power_on_delay_ms);

            // set address width
            radio.aw(addr_width);
            // set retransmit
            radio.retr(config_t::retr_delay, config_t::retr_attempts);
            // set channel
            radio.channel(config_t::channel);
            // setup rf
            radio.rf(config_t::power, config_t::rate);
            // toggle features (necessary on some clones and non-P versions)
            radio.enable_features();
            // enable dynamic payload
            radio.feature(rf24_f::dynamic_payload);

            if (!radio.sanity_check(config_t::power, config_t::rate, config_t::channel))
                return false;

            // enable broadcasting pipe
            radio.rx_pipes(1 << broadcast_pipe);
            // disable AA on all pipes, activate when node pipe set
            radio.auto_ack(0x00);
            // enable dynamic payloads on used pipes
            radio.dynamic_payload((1 << broadcast_pipe) | 1 << 0);
            
            std::uint8_t base_id[addr_width] = {0x00,0xFC,0xE1,0xA8,0xA8};

            // listen to broadcast pipe
            base_id[0] = config_t::broadcast_address;
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

            return true;
        }
    };
}

#endif