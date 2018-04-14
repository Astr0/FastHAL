#ifndef FH_AVR_I2C_H_
#define FH_AVR_I2C_H_

// I2C should
// read/write as master with some kind of error reporting
// interrupt/fail on slave transmit/receive and call some kind of callback to handle it
// support streaming protocol
// use little to no RAM and code, buffered on non-buffered mode


// Master Trasfer state machine (start+select_w)
// ***** state: action
// ready: start
// m_start: select_w
// m_restart: select_w, select_r
// mt: write, repeated start, stop, stop_start
// mt_nack: write, repeated start, stop, stop_start
// mt_write: write, repeated start, stop, stop_start
// mt_write_nack: write, repeated start, stop, stop_start
// m_la: enter not-addressed-slave (fail), start
// ***** action > states
// start -> m_start, m_restart
// select_w -> mt, mt_nack, m_la
// write -> mt_write, mt_write_nack, m_la
// stop -> ready
// stop_start -> m_start
// fail -> ready??

// Master Receive state machine (start+select_r)
// ***** state: action
// ready: start
// m_start: select_r
// m_restart: select_w, select_r
// m_la: enter not-addressed-slave (fail), start
// mr: read, readlast, repeated start, stop, stop_start
// mr_nack: read, readlast, repeated start, stop, stop_start
// mr_read: read, readlast
// mr_readl: repeated start, stop, stop_start
// ***** action > states
// start -> m_start, m_restart
// select_r -> mr, mr_nack, m_la
// read -> mr_read
// readlast -> mr_readl, m_la
// stop -> ready
// stop_start -> m_start
// fail -> ready??

// Slave Transfer state machine (TWAR initialized, TWEN=1, TWEA = 1, TWSTA = 0, TWSTO = 0) or state=recv_sla_r_lp. Sets TWINT
// ***** state: action
// st: writelast, write
// st_lp: writelast, write
// st_write: writelast, write
// st_writel: enter not-addressed-slave disable recognition (fail_nack), enter not-addressed-slave enable recognition (fail_ack), start (ack/nack)
// st_writel_ack: fail(ack/nack), start (ack/nack)

// Slave Receive state machine (TWAR initialized, TWEN=1, TWEA = 1, TWSTA = 0, TWSTO = 0) or state=. Sets TWINT
// ***** state: action
// sr: read, readlast
// sr_la: read, readlast
// sr_cast: read, readlast
// sr_cast_la: read, readlast
// sr_read: read, readlast
// sr_readl: fail(ack/nack), start(ack/nack)
// sr_read_cast: read, readlast
// sr_readl_cast: fail(ack/nack), start(ack/nack)
// sr_stop_restart: fail(ack/nack), start(ack/nack)

// global statuses
// ready: something's ok goin on (transfer or wait), no actions
// bus_fail: Illegal start/stop condition, stop to reset TWI module (no stop is really sent on bus)


// API
// I2C can be in modes: Ready (ok or something failed and stopped), MT, MR, ST, SR
// Master API
// start() - from states ready, mt, mt_nack, write, write_nack, m_la, mr, mr_nack, mr_readl, st_writel, st_writel_ack, st_readl, st_readl_cast, sr_stop_restart
// stop() - from states bus_fail, mt, mt_nack, mt_write, mt_write_nack, mr, mr_nack, mr_readl
// write() - from states mt, mt_nack, mt_write, mt_write_nack, st, st_lp, st_write
// read() - from states mr, mr_nack, mr_read, sr, sr_la, sr_cast, sr_cast_la, sr_read, sr_read_cast
//

#include "registers.hpp"

#if 1==1//def FH_HAS_I2C0

#include "interrupts.hpp"
#include "../../std/std_types.hpp"

namespace fasthal{
    // master op status
    enum class i2c_state:: std::uint8_t{
        // nothing is happening
        ready = 0,
        // device not available
        not_available = 1,
        // lost arbitration
        lost_abritation = 2,
        // received nack from device
        nack = 3,
        // bus fail
        bus_fail = 4,
        // waiting for select
        select = 5,
        // mt going on
        mt = 6,
        // mr goint on
        mr = 7,
        // done op, waiting for stop/start/restart
        done = 8,
        // i2c is off
        off = 9
    };

    namespace details{
        template<unsigned VNum>
        struct i2c_impl{
            static constexpr bool available = false;
        };

        // enable, reset, ready for i2c
        template<unsigned VNum>
        struct func_fieldbit_impl<i2c_impl<VNum>>:
            func_fieldbit_enable<decltype(i2c_impl<VNum>::enable)>,
            func_fieldbit_ready_reset<decltype(i2c_impl<VNum>::ready)>
            { };

        inline constexpr std::uint8_t twi_ps_value(avr::tw_ps ps){
            using ps_t = avr::tw_ps;
            switch (ps){
                case ps_t::_1 : return 1;
                case ps_t::_4 : return 4;
                case ps_t::_16 : return 16;
                case ps_t::_64 : return 64;
                default: return 0;
            }
        }

        template<typename TFreq, typename TPs>
        inline constexpr std::uint8_t i2c_calc_twbr(TFreq freq, TPs ps){
            auto psv = twi_ps_value(ps);
            
            return ((F_CPU / freq) - 16) / (2 * psv);
        }

        template<typename TFreq, TFreq VFreq, avr::tw_ps VPs>
        inline constexpr auto i2c_calc_twbr(integral_constant<TFreq, VFreq> freq, integral_constant<avr::tw_ps, VPs> ps) {
            auto result = i2c_calc_twbr(VFreq, VPs);
            return integral_constant<std::uint8_t, result>{};
        }

        // general case
        template<typename TRead, typename TAddress>
        inline constexpr auto i2c_build_sla(TRead read, TAddress address){
            auto result = static_cast<std::uint8_t>(static_cast<std::uint8_t>(address) << 1);
            if (read)
                result |= 1;
            return result;
        }
        // optimized for known mode
        template<bool VRead, typename TAddress>
        inline constexpr auto i2c_build_sla(integral_constant<bool, VRead> read, TAddress address){
            auto result = static_cast<std::uint8_t>(static_cast<std::uint8_t>(address) << 1);
            if constexpr (VRead)
                result |= 1;
            return result;            
        }   
        // optimized for known mode and address
        template<bool VRead, typename TAddress, TAddress VAddress>
        inline constexpr auto i2c_build_sla(integral_constant<bool, VRead> read, integral_constant<TAddress, VAddress> address){
            constexpr auto result = i2c_build_sla(VRead, VAddress);
            return integral_constant<decltype(result), result>{};
        }   

        template<bool VStart = false, bool VStop = false, unsigned VNum>
        static constexpr auto i2c_control(i2c_impl<VNum> i2c){
            return combine(
                clear(i2c.control),
                enable(i2c),
                set(i2c.start, integral_constant<bool, VStart>),
                set(i2c.stop, integral_constant<bool, VStop>),
                reset(i2c),
            );
        }

        template<bool VStart = false, bool VStop = false, unsigned VNum>
        static void i2c_control_(i2c_impl<VNum> i2c){
            apply(i2c_control<VStart, VStop>(i2c));
        }
        
        template<unsigned VNum>
        static void i2c_stop(i2c_impl<VNum> i2c){
            i2c_control_<false, true>(i2c);
            wait_lo(i2c.stop);
        }

        template<unsigned VNum>
        static void i2c_wait(i2c_impl<VNum> i2c){
            wait_(i2c);
            i2c_fsm(i2c);
        }

        template<unsigned VNum, typename T>
        static void i2c_write(i2c_impl<VNum> i2c, T v){
            apply(
                write(i2c.data, v),
                details::i2c_control(i2c)
            );
            wait_(i2c);  
        }

        template<unsigned VNum>
        static void i2c_fsm(i2c_impl<VNum> i2c){
            using i2c_t = i2c_impl<VNum>;
            auto s = _read(i2c_t::status);
            using s_t = typename i2c_t::status_t;
            switch(s){
                case s_t::bus_fail:
                    // HW error on bus (invalid START/STOP condition). Need for bus restart.
                    i2c_stop(i2c);
                    i2c_t::state = i2c_state::bus_fail;
                    break;
                case s_t::m_start:
                case s_t::m_restart:
                    // Entered START. Need select_w or select_r
                    // Entered repeated START. Need select_w or select_r
                    i2c_t::state = i2c_state::select;
                    break;
                case s_t::mt:
                    // select_w sent, received ACK. Need write or start/stop/stop_start
                    i2c_t::state = i2c_state::mt;
                    // TODO: write something?
                    break;
                case s_t::mt_nack:                    
                    // select_w sent, received NACK. Need write or start/stop/stop_start
                    i2c_stop();
                    i2c_t::state = i2c_state::not_available;
                    break;
                case s_t::mt_write:
                    // MT write, received ACK. Need write or start/stop/stop_start
                    break;
                case s_t::mt_write_nack:
                    // MT write, received NACK. Need write or start/stop/stop_start
                    i2c_stop();
                    i2c_t::state = i2c_state::nack;
                    break;
                case s_t::m_la:
                    // another master took of the bus unexpectedly in select_w, select_r or write/readl. Need fail or start.
                    i2c_stop();
                    i2c_t::state = i2c_state::lost_abritation;
                    break;
                case s_t::mr:
                    // select_r sent, received ACK. Need read/readl or start/stop/stop_start
                    i2c_t::state = i2c_state::mr;
                    // TODO: read something
                    break;
                case s_t::mr_nack:
                    // select_r sent, received NACK. Need read, readlast, repeated start, stop, stop_start
                    i2c_stop();
                    i2c_t::state = i2c_state::not_available;
                    break;
                case s_t::mr_read:
                    // TODO: Read something
                    // recevied byte ok. ACK or NACK will be send, mr
                    break;
                case s_t::mr_readl:                    
                    // nack sent to slave after receiving byte, stop restart or stop/start will be transmitted, mr
                    i2c_t::state = i2c_state::done;
                    break;
                // TODO: Slave thingy
                case s_t::sr:
                    // received own sla-w, ACK returned, will receive bytes and ACK/NACK, sr
                    break;
                case s_t::sr_la:
                    // arbitration lost in master sla-r/w, slave address matched
                    break;
                case s_t::sr_cast:
                    // broadcast has been received, ACK returned, will receive bytes and ACK/NACK, sr
                    break;
                case s_t::sr_cast_la:
                    // arbitration lost in master sla-r/w, sla+w broadcast, will receive bytes and ACK/NACK, sr
                    break;
                case s_t::sr_read:
                    // own data has been received, ACK returned, will receive bytes and ACK/NACK, sr
                    break;
                case s_t::sr_readl:
                    // own data has been received, NACK returned, reseting TWI, sr
                    break;
                case s_t::sr_read_cast:
                    // broadcast data has been received, ACK returned, will receive bytes and ACK/NACK, sr
                    break;
                case s_t::sr_readl_cast:
                    // broadcast data has been received, NACK returned, reseting TWI, sr
                    break;
                case s_t::sr_stop_restart:
                    // stop or start has been received while still addressed, reseting TWI, sr
                    break;
                case s_t::st:
                    // received own sla-r, ACK returned, will send bytes, st
                    break;
                case s_t::st_la:
                    // arbitration lost in master sla-r/w, slave address matched
                    break;
                case s_t::st_write:
                    // data byte was transmitted and ACK has been received, will send bytes, st
                    break;
                case s_t::st_writel:
                    // data byte was transmitted and NACK has been received, reseting TWI, st
                    break;
                case s_t::st_writel_ack:
                    // last data byte was transmitted and ACK has been received, reseting TWI, st
                    break;
                case s_t::ready:
                    // no errors, ok state?
                    break;
            }
        }
    }

    using i2c_address_t = std::uint8_t;
    template<i2c_address_t V>
    constexpr auto i2c_address_v = integral_constant<i2c_address_t, V>{};

    template<std::uint32_t V>
    constexpr auto i2c_freq_v = integral_constant<std::uint32_t, V>{};
    constexpr auto i2c_freq_def = i2c_freq_v<100000L>;

    constexpr auto i2c_write = integral_constant<bool, false>{};
    constexpr auto i2c_read = integral_constant<bool, true>{};
    
    constexpr auto i2c_more = integral_constant<bool, true>{};
    constexpr auto i2c_last = integral_constant<bool, false>{};

    // master begin
    template<unsigned VNum, typename TFreq = decltype(i2c_freq_def), typename TPs = decltype(avr::tw_ps::def)>
    inline constexpr auto begin(details::i2c_impl<VNum> i2c, TFreq freq = i2c_freq_def, TPs ps = avr::tw_ps::def){
        details::i2c_impl<VNum>::state = i2c_state::ready;
        return combine(
            write(i2c.rate, details::i2c_calc_twbr(freq, ps)),
            clear(i2c.control),
            write(i2c.ps, ps),
            enable(i2c)
            //enable(i2c.irq),
            //enable(i2c.ack)
        );
    }
    template<unsigned VNum, typename TFreq = decltype(i2c_freq_def), typename TPs = decltype(avr::tw_ps::def)>
    inline void begin_(details::i2c_impl<VNum> i2c, TFreq freq = i2c_freq_def, TPs ps = avr::tw_ps::def){
        apply(begin(i2c, freq, ps));
    }

    // end i2c
    template<unsigned VNum>
    inline constexpr auto end(details::i2c_impl<VNum> i2c){
        details::i2c_impl<VNum>::state = i2c_state::off;
        return clear(i2c.control);
    }
    template<unsigned VNum>
    inline void end_(details::i2c_impl<VNum> i2c) { 
        apply(end(i2c)); 
    }

    // master start 
    template<unsigned VNum>
    i2c_state start(details::i2c_impl<VNum> i2c){
        // check start condition        
        if (i2c.state == i2c_state::off || i2c.state == i2c_state::select)
            return i2c.state;

        details::i2c_control_<true, false>(i2c);
        details::i2c_wait(i2c);

        // check end condition
        return i2c.state;
    }

    // master stop 
    template<unsigned VNum>
    i2c_state stop(details::i2c_impl<VNum> i2c){
        // TODO: Check stop condition?
        apply(
            details::i2c_control(i2c),
            set(i2c.stop)
        );
        wait_lo(i2c.stop);
        // TODO: Check for error?
    }

    // master stop & start
    template<unsigned VNum>
    void stop_start(details::i2c_impl<VNum> i2c){
        // TODO: Check stop condition?
        apply(
            details::i2c_control(i2c),
            set(i2c.stop),
            set(i2c.start)
        );
        wait_lo(i2c.stop);
        // TODO: Check for error?
    }

    // master select device and mode
    template<unsigned VNum, typename TRead, typename TAddress>
    void select(details::i2c_impl<VNum> i2c, TRead read, TAddress address){
        // TODO: Check select condition?
        auto sla = details::i2c_build_sla(read, address);        
        details::i2c_write(i2c, sla);
        // TODO: Check for error?
    }

    // write
    template<unsigned VNum, typename T>
    void write(details::i2c_impl<VNum> i2c, T v){        
        // TODO: Check start conditions
        details::i2c_write(i2c, v);
        // TODO: Check for error?
    }

    // read - more - more reads expected, last - it was last read
    template<unsigned VNum, typename TMore = decltype(i2c_more)>
    auto read(details::i2c_impl<VNum> i2c, TMore more = i2c_more){
        // TODO: Check start condition
        // Ask for next byte
        apply(
            details::i2c_control(i2c),
            enable(i2c.ack, more)
        );
        wait_(i2c);
        // TODO: Check status
        return read_(i2c.data);
    }

    #ifdef FH_HAS_I2C0

    namespace details{
        template<>
        struct i2c_impl<0>{
            static constexpr bool available = true;
            using status_t = avr::tw_s;

            static constexpr auto ps = ::fasthal::avr::twps0;
            static constexpr auto rate = ::fasthal::avr::twbr0;
            static constexpr auto status = ::fasthal::avr::tws0;
            static constexpr auto data = ::fasthal::avr::twdr0;
            static constexpr auto control = ::fasthal::avr::twcr0;
            
            static constexpr auto enable = ::fasthal::avr::twen0;
            static constexpr auto ready = ::fasthal::avr::twint0;
            static constexpr auto ack = ::fasthal::avr::twea0;
            static constexpr auto start = ::fasthal::avr::twsta0;
            static constexpr auto stop = ::fasthal::avr::twsto0;
            
            static constexpr auto irq = irq_i2c0;

            static i2c_state state;
        };

        // enable for i2c.ack
        template<>
        struct func_fieldbit_impl<std::base_type_t<decltype(::fasthal::avr::twea0)>>:
            func_fieldbit_enable<decltype(::fasthal::avr::twea0)>
            { };
    }
    static i2c_state details::i2c_impl<0>::state = i2c_state::off;
    constexpr auto i2c0 = details::i2c_impl<0>{};
    
    #endif
}

#endif

#endif