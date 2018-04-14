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
        mr_done, // done operation (usually mr). can stop/start
        error, // some error occured (TODO: Detailed). can stop only
        nack // received NACK for select or MT
    };

    static constexpr auto i2c_mt = integral_constant<bool, false>{};
    static constexpr auto i2c_mr = integral_constant<bool, true>{};

    static constexpr auto i2c_more = integral_constant<bool, true>{};
    static constexpr auto i2c_last = integral_constant<bool, false>{};

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
            constexpr auto result = i2c_calc_twbr(VFreq, VPs);
            return integral_constant<std::uint8_t, result>{};
        }
        
        template<bool VRead, typename TAddress>
        inline constexpr auto i2c_build_sla(TAddress address){
            auto result = static_cast<std::uint8_t>(static_cast<std::uint8_t>(address) << 1);
            if constexpr (VRead)
                result |= 1;
            return result;
        }
        template<bool VRead, typename TAddress, TAddress VAddress>
        inline constexpr auto i2c_build_sla(integral_constant<TAddress, VAddress> address){
            return FH_CONST(i2c_build_sla<VRead>(VAddress));
        }

        // transmitter target, don't depend on transmitter!
        // template<unsigned VNum, template<typename> typename TTrans, template<typename> typename TRecv>
        // struct i2c_func{
        //     static constexpr auto _i2c = i2c_impl<VNum>{};
        //     static_assert(_i2c.available, "I2C not available");
            
        //     struct lazy{
        //         using trans_t = TTrans<i2c_func<VNum, TTrans, TRecv>>;
        //         static constexpr auto async_tx = trans_t::async;
        //         using recv_t = TRecv<i2c_func<VNum, TTrans, TRecv>>;                
        //     };




        //     static inline void wait(){
        //         wait_(_i2c);
        //     }
        //     static inline void wait_stop(){
        //         wait_lo(_i2c.stop);
        //     }

        //     static inline void stop(){
        //         control(set(_i2c.stop));
        //         wait_stop();
        //     }

        //     static i2c_state fsm(){
        //         auto s = read_(_i2c.status);
        //         using s_t = decltype(s);
        //         switch(s){
        //             case s_t::bus_fail: // HW error on bus (invalid START/STOP condition). Need for bus restart.
        //             case s_t::mt_nack: // select_w sent, received NACK. Need write or start/stop/stop_start
        //             case s_t::mt_write_nack: // MT write, received NACK. Need write or start/stop/stop_start
        //             case s_t::m_collision: // another master took of the bus unexpectedly in select_w, select_r or write/readl. Need fail or start.
        //             case s_t::mr_nack: // select_r sent, received NACK. Need read, readlast, repeated start, stop, stop_start
        //                 return i2c_state::error;
        //             case s_t::m_start: // Entered START. Need select_w or select_r
        //             case s_t::m_restart: // Entered repeated START. Need select_w or select_r
        //                 return i2c_state::select;
        //             case s_t::mt: // select_w sent, received ACK. Need write or start/stop/stop_start
        //             case s_t::mt_write: // MT write, received ACK. Need write or start/stop/stop_start
        //                 return i2c_state::mt;
        //             case s_t::mr: // select_r sent, received ACK. Need read/readl or start/stop/stop_start
        //             case s_t::mr_read: // recevied byte ok. Need read/readl
        //                 return i2c_state::mr;
        //             case s_t::mr_readl: // nack sent to slave after receiving byte, stop restart or stop/start will be transmitted, mr
        //                 return i2c_state::done;
        //             default:
        //                 return i2c_state::ready;
        //             // // TODO: Slave thingy
        //             // case s_t::sr:
        //             //     // received own sla-w, ACK returned, will receive bytes and ACK/NACK, sr
        //             //     break;
        //             // case s_t::sr_la:
        //             //     // arbitration lost in master sla-r/w, slave address matched
        //             //     break;
        //             // case s_t::sr_cast:
        //             //     // broadcast has been received, ACK returned, will receive bytes and ACK/NACK, sr
        //             //     break;
        //             // case s_t::sr_cast_la:
        //             //     // arbitration lost in master sla-r/w, sla+w broadcast, will receive bytes and ACK/NACK, sr
        //             //     break;
        //             // case s_t::sr_read:
        //             //     // own data has been received, ACK returned, will receive bytes and ACK/NACK, sr
        //             //     break;
        //             // case s_t::sr_readl:
        //             //     // own data has been received, NACK returned, reseting TWI, sr
        //             //     break;
        //             // case s_t::sr_read_cast:
        //             //     // broadcast data has been received, ACK returned, will receive bytes and ACK/NACK, sr
        //             //     break;
        //             // case s_t::sr_readl_cast:
        //             //     // broadcast data has been received, NACK returned, reseting TWI, sr
        //             //     break;
        //             // case s_t::sr_stop_restart:
        //             //     // stop or start has been received while still addressed, reseting TWI, sr
        //             //     break;
        //             // case s_t::st:
        //             //     // received own sla-r, ACK returned, will send bytes, st
        //             //     break;
        //             // case s_t::st_la:
        //             //     // arbitration lost in master sla-r/w, slave address matched
        //             //     break;
        //             // case s_t::st_write:
        //             //     // data byte was transmitted and ACK has been received, will send bytes, st
        //             //     break;
        //             // case s_t::st_writel:
        //             //     // data byte was transmitted and NACK has been received, reseting TWI, st
        //             //     break;
        //             // case s_t::st_writel_ack:
        //             //     // last data byte was transmitted and ACK has been received, reseting TWI, st
        //             //     break;
        //             // case s_t::ready:
        //             //     // no errors, ok state?
        //             //     break;
        //         }
        //     }

        //     static inline i2c_state get_state(){
        //         return fsm();
        //     }

        //     // -------------------------- target interface
        //     // write 1 byte, for transmitter communication
        //     static inline bool try_write(std::uint8_t c){
        //         // Check MT mode, but discard bytes from transmitter
        //         if (get_state() != i2c_state::mt) return true;
        //         write(c);
        //         wait();
        //         // no errors here, right?
        //         return true;
        //     }

        //     // tries to write something from transmitter
        //     static inline void try_write_sync(){
        //         static_assert(lazy::async_tx, "Not async i2c, shouldn't be here");
        //         // TODO
        //         //try_irq_force(uart_t::irq_txr);
        //     }

        //     // called by transmitter if it has some data to write next
        //     static inline void write_async(){
        //         static_assert(lazy::async_tx, "Not async i2c, shouldn't be here");
        //         // TODO
        //         //enable_(uart_t::irq_txr);
        //     }

        //     static inline void flush() {
        //         // TODO
        //     }
        //     // -------------------------- source interface
        //     // read for sync receiver
        //     static std::uint8_t read(){
        //         // Check start condition
        //         if (get_state() != i2c_state::mr) return 0;
        //         // we don't check programmer issues with not telling how many bytes to read, etc.
        //         // Ask for next byte
        //         _i2c.bytesToRead--;                
        //         control(enable(_i2c.ack, _i2c.bytesToRead != 0));
        //         wait();
                
        //         // even if state is wrong, we should return some garbage data (check what's wrong on start/stop next time)
        //         return read_(_i2c.data);
        //     }
        // };
    }

    using i2c_address_t = std::uint8_t;
    template<i2c_address_t V>
    constexpr auto i2c_address_v = integral_constant<i2c_address_t, V>{};

    template<std::uint32_t V>
    constexpr auto i2c_freq_v = integral_constant<std::uint32_t, V>{};
    constexpr auto i2c_freq_def = i2c_freq_v<100000L>;

    struct i2c_config{
        static constexpr auto ps_def = true;
    };

    template<unsigned VNum, class TConfig = i2c_config>
    class i2c{
        static constexpr auto _i2c = details::i2c_impl<VNum>{};
        static constexpr auto _status = typename std::conditional<TConfig::ps_def, decltype(_i2c.status0), decltype(_i2c.status)>::type{};
        static_assert(_i2c.available, "I2C not available");
        struct lazy{
            static constexpr bool async = details::has_isr<_i2c.irq.number>;
        };

        template<typename... T>
        static inline void i2c_control(T... actions){
            apply(
                clear(_i2c.control), 
                enable(_i2c),
                reset(_i2c),
                actions...);
        }
    public:
        // ------------------------------ begin
        template<typename TFreq = decltype(i2c_freq_def), typename TPs = decltype(avr::twps_v<avr::tw_ps::def>)>
        static inline constexpr auto begin(TFreq freq = i2c_freq_def, TPs ps = avr::twps_v<avr::tw_ps::def>){
            static_assert(!TConfig::ps_def || std::is_same_v<TPs, decltype(avr::twps_v<avr::tw_ps::def>)>, "Only TS_PS def can be used in VPsDef mode");
            return combine(
                write(_i2c.rate, details::i2c_calc_twbr(freq, ps)),
                clear(_i2c.control),
                write(_i2c.ps, ps),
                enable(_i2c)
                // enable(_i2c.irq, integral_constant<bool, lazy::async>{})
                // TODO: Slave mode
                //enable(i2c.ack)
            );
        }
        template<typename TFreq = decltype(i2c_freq_def), typename TPs = decltype(avr::tw_ps::def)>
        static inline void begin_(TFreq freq = i2c_freq_def, TPs ps = avr::tw_ps::def){
            apply(begin(freq, ps));
        }

        // ------------------------------ end
        static inline constexpr auto end(){ return clear(_i2c.control); }
        static inline void end_() { apply(end()); }

        // ------------------------------ waiting
        static inline bool ready() { return read_(_i2c.ready); }
        // todo: do we need it?
        static inline bool stopping() { return read_(_i2c.stop); }

        // ------------------------------ tx/rx
        static inline void tx(std::uint8_t b) { write_(_i2c.data, b); i2c_control(); }
        template<typename TMore = decltype(i2c_more)>
        static inline void rx_ask(TMore more = i2c_more) { i2c_control(set(_i2c.ack, more)); }
        static inline std::uint8_t rx() { return read_(_i2c.data); }

        // ------------------------------ master start/stop
        static inline void start(){ i2c_control(set(_i2c.start)); }
        static inline void stop(){ i2c_control(set(_i2c.stop)); }
        static inline void stop_start(){ i2c_control(set(_i2c.start), set(_i2c.stop)); }
        template<bool VRead, typename TAddress>
        static inline void select(integral_constant<bool, VRead> mode, TAddress address) { tx(details::i2c_build_sla<VRead>(address)); }

        // ------------------------------ state
        static i2c_state state(){
            auto s = read_(_status);
            using s_t = decltype(s);
            switch(s){
                case s_t::bus_fail: // HW error on bus (invalid START/STOP condition). Need for bus restart.
                case s_t::m_collision: // another master took of the bus unexpectedly in select_w, select_r or write/readl. Need fail or start.
                    return i2c_state::error;
                case s_t::mt_nack: // select_w sent, received NACK. Need write or start/stop/stop_start
                case s_t::mt_write_nack: // MT write, received NACK. Need write or start/stop/stop_start
                case s_t::mr_nack: // select_r sent, received NACK. Need read, readlast, repeated start, stop, stop_start
                    return i2c_state::nack;
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
                    return i2c_state::mr_done;
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

    };

    // ***************************************************** "extension methods"
    // start master operation
    template<unsigned VNum, class TConfig, bool VRead, typename TAddress>
    static void try_start(i2c<VNum, TConfig> i, integral_constant<bool, VRead> mode, TAddress address) {
        // check start state
        // select can't be here really according to our fsm
        if (details::i2c_state_any(i.state(), i2c_state::error, i2c_state::mr))
            return;

        i.start();
        while (!i.ready());
        // don't check started. there should be no errors after start, it blocks
        // if (_f.get_state() != i2c_state::select) return false;

        i.select(mode, address);
        while (!i.ready());
    }

    // stop master operation
    template<unsigned VNum, class TConfig>
    static void try_stop(i2c<VNum, TConfig> i){
        if (!details::i2c_state_any(i.state(), 
                i2c_state::mt, 
                i2c_state::mr_done,
                i2c_state::nack,
                i2c_state::error))
            return;
        i.stop();
        while (i.stopping());
    }

    // **********************************  sync mt ostream
    template <class TI2c>
    class i2c_mt_sync{
        static constexpr auto _i2c = TI2c{};
    public:
        template<typename TAddress>
        i2c_mt_sync(TAddress address){
            try_start(_i2c, i2c_mt, address);
        }

        i2c_mt_sync(const i2c_mt_sync<TI2c>&) = delete;

        static inline auto state(){ return _i2c.state(); }
        static inline auto ok(){ return state() == i2c_state::mt; };

        inline operator bool(){ return ok(); }

        static void write(std::uint8_t b){
            if (!ok()) return;
            _i2c.tx(b);
            while (!_i2c.ready());
        }

        ~i2c_mt_sync() {
            //println(uart_sync_tx<0>{}, "~");
            try_stop(_i2c);
        }
    };

    namespace details{
        template<class TI2c> struct is_ostream_impl<i2c_mt_sync<TI2c>>: std::true_type{};
    }
    template<class TI2c>
    void write(i2c_mt_sync<TI2c>& mt, std::uint8_t b){ mt.write(b); }


    template <unsigned VNum, class TConfig, typename TAddress>
    static auto start_mt_sync(i2c<VNum, TConfig> i, TAddress address){
        return i2c_mt_sync<i2c<VNum, TConfig>>{ address };
    }

    // **********************************  sync mr istream
    template <class TI2c>
    class i2c_mr_sync{
        static constexpr auto _i2c = TI2c{};
        bsize_t _bytesLeft;

    public:
        template<typename TAddress>
        i2c_mr_sync(TAddress address, bsize_t willRead): _bytesLeft(willRead){
            try_start(_i2c, i2c_mr, address);
        }

        i2c_mr_sync(const i2c_mr_sync<TI2c>&) = delete;

        static inline auto state(){ return _i2c.state(); }
        static inline auto ok(){ return details::i2c_state_any(state(), i2c_state::mr, i2c_state::mr_done); };

        inline operator bool(){
            return ok();
        }

        std::uint8_t read(){
            // return garabe
            if (state() != i2c_state::mr) return 0;
            // we don't check programmer issues with not telling how many bytes to read, etc.
            // Ask for next byte
            _i2c.rx_ask(--_bytesLeft);            
            while (!_i2c.ready());            
            
            // even if state is wrong, we should return some garbage data (check what's wrong on start/stop next time)
            return _i2c.rx();
        }

        ~i2c_mr_sync() {
            //println(uart_sync_tx<0>{}, "~");
            try_stop(_i2c);
        }
    };

    namespace details{
        template<class TI2c> struct is_istream_impl<i2c_mr_sync<TI2c>>: std::true_type{};
    }
    template<class TI2c>
    std::uint8_t read(i2c_mr_sync<TI2c>& mr){ return mr.read(); }


    template <unsigned VNum, class TConfig, typename TAddress>
    static auto start_mr_sync(i2c<VNum, TConfig> i, TAddress address, bsize_t bytes){
        return i2c_mr_sync<i2c<VNum, TConfig>>{ address, bytes };
    }


    #include "i2c_impl.hpp"
};

#endif
