#ifndef FH_AVR_I2C_H_
#define FH_AVR_I2C_H_

#include "registers.hpp"
#include "../../streams/sync_streams.hpp"

// don't check that programmer is stupid, check for error conditions

namespace fasthal{
    enum class i2c_state{    
        ready = 0, // nothing is happening on i2c 
        select, // after start - waiting select_w or select_r, really meaningless
        mt, // in mt mode - can write or stop/start
        mr, // in mr mode - can read only
        done, // done operation (usually mr). can stop/start
        error // some error occured (TODO: Detailed). can stop only
    };

    static constexpr auto i2c_mt = integral_constant<bool, false>{};
    static constexpr auto i2c_mr = integral_constant<bool, true>{};

    namespace details{        
        template<typename... TStates>
        static inline bool i2c_state_any(i2c_state state, TStates... states){
            return (...|| (state == states));
        }


        template<unsigned VNum>
        struct i2c_impl{
            static constexpr bool available = false;
        };

        // enable, reset, ready for i2c impl
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
        
        template<bool VRead, typename TAddress>
        inline constexpr auto i2c_build_sla(TAddress address){
            auto result = static_cast<std::uint8_t>(static_cast<std::uint8_t>(address) << 1);
            return VRead ? result | 1 : result;
        }
        template<bool VRead, typename TAddress, TAddress VAddress>
        inline constexpr auto i2c_build_sla(integral_constant<TAddress, VAddress> address){
            constexpr auto result = i2c_build_sla<VRead>(VAddress);
            return integral_constant<decltype(result), result>{};
        }

        // transmitter target, don't depend on transmitter!
        template<unsigned VNum, template<typename> typename TTrans, template<typename> typename TRecv>
        struct i2c_func{
            static constexpr auto _i2c = i2c_impl<VNum>{};
            static_assert(_i2c.available, "I2C not available");
            
            struct lazy{
                using trans_t = TTrans<i2c_func<VNum, TTrans, TRecv>>;
                static constexpr auto async_tx = trans_t::async;
                using recv_t = TRecv<i2c_func<VNum, TTrans, TRecv>>;                
            };

            template<typename... T>
            static inline void control(T... actions){
                apply(
                    clear(_i2c.control), 
                    enable(_i2c),
                    reset(_i2c),
                    actions...);
            }

            static inline void write(std::uint8_t v){
                write_(_i2c.data, v),
                control();
            }     

            static inline void wait(){
                wait_(_i2c);
            }
            static inline void wait_stop(){
                wait_lo(_i2c.stop);
            }

            static inline void stop(){
                control(set(_i2c.stop));
                wait_stop();
            }

            static i2c_state fsm(){
                auto s = read_(_i2c.status);
                using s_t = decltype(s);
                switch(s){
                    case s_t::bus_fail: // HW error on bus (invalid START/STOP condition). Need for bus restart.
                    case s_t::mt_nack: // select_w sent, received NACK. Need write or start/stop/stop_start
                    case s_t::mt_write_nack: // MT write, received NACK. Need write or start/stop/stop_start
                    case s_t::m_collision: // another master took of the bus unexpectedly in select_w, select_r or write/readl. Need fail or start.
                    case s_t::mr_nack: // select_r sent, received NACK. Need read, readlast, repeated start, stop, stop_start
                        return i2c_state::error;
                    case s_t::m_start: // Entered START. Need select_w or select_r
                    case s_t::m_restart: // Entered repeated START. Need select_w or select_r
                        return i2c_state::select;
                    case s_t::mt: // select_w sent, received ACK. Need write or start/stop/stop_start
                    case s_t::mt_write: // MT write, received ACK. Need write or start/stop/stop_start
                        return i2c_state::mt;
                    case s_t::mr: // select_r sent, received ACK. Need read/readl or start/stop/stop_start
                    case s_t::mr_read: // recevied byte ok. Need read/readl
                        return i2c_state::mr;
                    case s_t::mr_readl: // nack sent to slave after receiving byte, stop restart or stop/start will be transmitted, mr
                        return i2c_state::done;
                    default:
                        return i2c_state::ready;
                    // // TODO: Slave thingy
                    // case s_t::sr:
                    //     // received own sla-w, ACK returned, will receive bytes and ACK/NACK, sr
                    //     break;
                    // case s_t::sr_la:
                    //     // arbitration lost in master sla-r/w, slave address matched
                    //     break;
                    // case s_t::sr_cast:
                    //     // broadcast has been received, ACK returned, will receive bytes and ACK/NACK, sr
                    //     break;
                    // case s_t::sr_cast_la:
                    //     // arbitration lost in master sla-r/w, sla+w broadcast, will receive bytes and ACK/NACK, sr
                    //     break;
                    // case s_t::sr_read:
                    //     // own data has been received, ACK returned, will receive bytes and ACK/NACK, sr
                    //     break;
                    // case s_t::sr_readl:
                    //     // own data has been received, NACK returned, reseting TWI, sr
                    //     break;
                    // case s_t::sr_read_cast:
                    //     // broadcast data has been received, ACK returned, will receive bytes and ACK/NACK, sr
                    //     break;
                    // case s_t::sr_readl_cast:
                    //     // broadcast data has been received, NACK returned, reseting TWI, sr
                    //     break;
                    // case s_t::sr_stop_restart:
                    //     // stop or start has been received while still addressed, reseting TWI, sr
                    //     break;
                    // case s_t::st:
                    //     // received own sla-r, ACK returned, will send bytes, st
                    //     break;
                    // case s_t::st_la:
                    //     // arbitration lost in master sla-r/w, slave address matched
                    //     break;
                    // case s_t::st_write:
                    //     // data byte was transmitted and ACK has been received, will send bytes, st
                    //     break;
                    // case s_t::st_writel:
                    //     // data byte was transmitted and NACK has been received, reseting TWI, st
                    //     break;
                    // case s_t::st_writel_ack:
                    //     // last data byte was transmitted and ACK has been received, reseting TWI, st
                    //     break;
                    // case s_t::ready:
                    //     // no errors, ok state?
                    //     break;
                }
            }

            static inline i2c_state get_state(){
                return fsm();
            }

            // -------------------------- target interface
            // write 1 byte, for transmitter communication
            static inline bool try_write(std::uint8_t c){
                // Check MT mode, but discard bytes from transmitter
                if (get_state() != i2c_state::mt) return true;
                write(c);
                wait();
                // no errors here, right?
                return true;
            }

            // tries to write something from transmitter
            static inline void try_write_sync(){
                static_assert(lazy::async_tx, "Not async i2c, shouldn't be here");
                // TODO
                //try_irq_force(uart_t::irq_txr);
            }

            // called by transmitter if it has some data to write next
            static inline void write_async(){
                static_assert(lazy::async_tx, "Not async i2c, shouldn't be here");
                // TODO
                //enable_(uart_t::irq_txr);
            }

            static inline void flush() {
                // TODO
            }
            // -------------------------- source interface
            // read for sync receiver
            static std::uint8_t read(){
                // Check start condition
                if (get_state() != i2c_state::mr) return 0;
                // we don't check programmer issues with not telling how many bytes to read, etc.
                // Ask for next byte
                _i2c.bytesToRead--;                
                control(enable(_i2c.ack, _i2c.bytesToRead != 0));
                wait();

                return read_(_i2c.data);
            }
        };
    }

    using i2c_address_t = std::uint8_t;
    template<i2c_address_t V>
    constexpr auto i2c_address_v = integral_constant<i2c_address_t, V>{};

    template<std::uint32_t V>
    constexpr auto i2c_freq_v = integral_constant<std::uint32_t, V>{};
    constexpr auto i2c_freq_def = i2c_freq_v<100000L>;

    template<unsigned VNum, template<typename> typename TTrans = sync_transmitter, template<typename> typename TRecv = sync_receiver>
    struct i2c{
        static constexpr auto _i2c = details::i2c_impl<VNum>{};
        static_assert(_i2c.available, "I2C not available");
        using func_t = details::i2c_func<VNum, TTrans, TRecv>;

        static constexpr auto _f = func_t{};

        static constexpr auto tx = TTrans<func_t>{};
        static constexpr auto rx =  TRecv<func_t>{};

        // begin
        template<typename TFreq = decltype(i2c_freq_def), typename TPs = decltype(avr::tw_ps::def)>
        static inline constexpr auto begin(TFreq freq = i2c_freq_def, TPs ps = avr::tw_ps::def){
            return combine(
                write(_i2c.rate, details::i2c_calc_twbr(freq, ps)),
                clear(_i2c.control),
                write(_i2c.ps, ps),
                enable(_i2c)
                //enable(i2c.irq),
                //enable(i2c.ack)
            );
        }
        template<typename TFreq = decltype(i2c_freq_def), typename TPs = decltype(avr::tw_ps::def)>
        static inline void begin_(TFreq freq = i2c_freq_def, TPs ps = avr::tw_ps::def){
            apply(begin(freq, ps));
        }

        // end i2c
        static inline constexpr auto end(){            
            return clear(_i2c.control);
        }
        static inline void end_() { 
            apply(end()); 
        }

        // start MT or MR
        template<bool VRead, typename TAddress>
        static i2c_state start(TAddress address, integral_constant<bool, VRead> mode, bsize_t willRead = 0) {
            // check start state
            // select can't be here really according to our fsm
            auto state = _f.get_state();
            if (details::i2c_state_any(state, i2c_state::error, i2c_state::mr))
                return state;

            _f.control(set(_i2c.start));
            _f.wait();
            // don't check started. there should be no errors after start, it blocks
            // if (_f.get_state() != i2c_state::select) return false;

            if constexpr(VRead)
                _i2c.bytesToRead = willRead;
            
            auto sla = details::i2c_build_sla<VRead>(address);
            _f.write(sla);
            _f.wait();
            
            // return state
            return _f.get_state();
        }

        // stop master operation
        static i2c_state stop(){
            auto state = _f.get_state();
            if (!details::i2c_state_any(state, 
                    i2c_state::mt, 
                    i2c_state::done,
                    i2c_state::error))
                return state;
            _f.stop();
            // End condition should be fine 
            // return false if error happended somewhere up there
            return state == i2c_state::error ? i2c_state::error : i2c_state::done;
        }
    };

    #include "i2c_impl.hpp"
};

#endif
