#ifndef FH_AVR_I2C_H_
#define FH_AVR_I2C_H_

#include "registers.hpp"
#include "interrupts.hpp"
#include "../../streams/sync_streams.hpp"
#include "../../hal/i2c.hpp"

// don't check that programmer is stupid, check for error conditions
#define FH_I2C(NUM, HANDLER) FH_ISR(FH_IRQ_I2C ## NUM, HANDLER);

namespace fasthal{
    // prescaler
    enum class i2c_ps: std::uint8_t{
        _1 = 0
        , _4 =  (0 << TWPS1) | (1 << TWPS0)
        , _16 = (1 << TWPS1) | (0 << TWPS0)
        , _64 = (1 << TWPS1) | (1 << TWPS0)
        , def = _1
    };
    template<i2c_ps V>
    static constexpr auto i2c_ps_v = integral_constant<i2c_ps, V>{};

    template<std::uint32_t V>
    constexpr auto i2c_freq_v = integral_constant<std::uint32_t, V>{};
    constexpr auto i2c_freq_def = i2c_freq_v<100000L>;
        
    // status
    enum class i2c_s: std::uint8_t{
        // master statuses 
        // mt = master transmit
        // mr = master receive
        bus_fail          = 0x00, // HW error on bus (invalid START/STOP condition). Need for bus restart.
        m_start           = 0x08, // Entered START. Need select_w or select_r
        m_restart         = 0x10, // Entered repeated START. Need select_w or select_r
        mt                = 0x18, // select_w sent, received ACK. Need write or start/stop/stop_start
        mt_nack           = 0x20, // select_w sent, received NACK. Need write or start/stop/stop_start
        mt_write          = 0x28, // MT write, received ACK. Need write or start/stop/stop_start
        mt_write_nack     = 0x30, // MT write, received NACK. Need write or start/stop/stop_start
        m_la              = 0x38, // another master took of the bus unexpectedly in select_w, select_r or write/readl. Need fail or start.
        mr                = 0x40, // select_r sent, received ACK. Need read/readl or start/stop/stop_start
        mr_nack           = 0x48, // select_r sent, received NACK. Need read, readlast, repeated start, stop, stop_start
        mr_read           = 0x50, // recevied byte ok. Need read/readl
        mr_readl          = 0x58, // nack sent to slave after receiving byte, stop restart or stop/start will be transmitted, mr
        // slave statuses
        // st = slave transmit
        // sr = slave receive
        sr                = 0x60, // received own sla-w, ACK returned, will receive bytes and ACK/NACK, sr
        sr_la             = 0x68, // arbitration lost in master sla-r/w, slave address matched
        sr_cast           = 0x70, // broadcast has been received, ACK returned, will receive bytes and ACK/NACK, sr
        sr_cast_la        = 0x78, // arbitration lost in master sla-r/w, sla+w broadcast, will receive bytes and ACK/NACK, sr
        sr_read           = 0x80, // own data has been received, ACK returned, will receive bytes and ACK/NACK, sr
        sr_readl          = 0x88, // own data has been received, NACK returned, reseting TWI, sr
        sr_read_cast      = 0x90, // broadcast data has been received, ACK returned, will receive bytes and ACK/NACK, sr
        sr_readl_cast     = 0x98, // broadcast data has been received, NACK returned, reseting TWI, sr
        sr_stop_restart   = 0xA0, // stop or start has been received while still addressed, reseting TWI, sr
        st                = 0xA8, // received own sla-r, ACK returned, will send bytes, st
        st_la             = 0xB0, // arbitration lost in master sla-r/w, slave address matched
        st_write          = 0xB8, // data byte was transmitted and ACK has been received, will send bytes, st
        st_writel         = 0xC0, // data byte was transmitted and NACK has been received, reseting TWI, st
        st_writel_ack     = 0xC8, // last data byte was transmitted and ACK has been received, reseting TWI, st
        ready             = 0xF8  // no errors, ok state?
    };

    static constexpr auto i2c_more = integral_constant<bool, true>{};
    static constexpr auto i2c_last = integral_constant<bool, false>{};

    namespace details{        
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

        inline constexpr std::uint8_t i2c_ps_value(i2c_ps ps){
            switch (ps){
                case i2c_ps::_1 : return 1;
                case i2c_ps::_4 : return 4;
                case i2c_ps::_16 : return 16;
                case i2c_ps::_64 : return 64;
                default: return 0;
            }
        }

        template<typename TFreq, typename TPs>
        inline constexpr std::uint8_t i2c_calc_twbr(TFreq freq, TPs ps){
            auto psv = i2c_ps_value(ps);
            
            return ((F_CPU / freq) - 16) / (2 * psv);
        }

        template<typename TFreq, TFreq VFreq, i2c_ps VPs>
        inline constexpr auto i2c_calc_twbr(integral_constant<TFreq, VFreq> freq, integral_constant<i2c_ps, VPs> ps) {
            constexpr auto result = i2c_calc_twbr(VFreq, VPs);
            return integral_constant<std::uint8_t, result>{};
        }      

    }

    class i2c_state{
        i2c_s _state;
    public:
        i2c_state(i2c_s state): _state(state) {}

        i2c_s state(){ return _state; }
        template<typename... TStates>
        bool state_any(TStates... states) { return is_any(state(), states...); }

        bool mt_ok() { return state_any(i2c_s::mt_write, i2c_s::mt); }
        bool mr_ok() { return state_any(i2c_s::mr_read, i2c_s::mr); }
        bool mr_done() { return state_any(i2c_s::mr_readl); }
        bool mr_ok_done() { return mr_ok() || mr_done(); }
        bool master_ok() { return mt_ok() || mr_ok_done(); }
        bool m_nack() { return state_any(i2c_s::mr_nack, i2c_s::mt_nack, i2c_s::mt_write_nack); }
        bool error() { return state_any(i2c_s::bus_fail, i2c_s::m_la); }

        bool can_start() { return !(error() || mr_ok()); }
        bool can_stop() { return !mr_ok(); }
        //bool can_stop() { return mt_ok() || mr_done() || m_nack() || error(); }
    };

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
                enable(_i2c.irq, integral_constant<bool, lazy::async>{}),
                actions...);
        }
    public:
        // ------------------------------ begin
        template<typename TFreq = decltype(i2c_freq_def), typename TPs = decltype(i2c_ps_v<i2c_ps::def>)>
        static inline constexpr auto begin(TFreq freq = i2c_freq_def, TPs ps = i2c_ps_v<i2c_ps::def>){
            static_assert(!TConfig::ps_def || std::is_same_v<TPs, decltype(i2c_ps_v<i2c_ps::def>)>, "Only default presclar can be used in VPsDef mode");
            return combine(
                write(_i2c.rate, details::i2c_calc_twbr(freq, ps)),
                clear(_i2c.control),
                write(_i2c.ps, ps),
                enable(_i2c),
                enable(_i2c.irq, integral_constant<bool, lazy::async>{})
                // TODO: Slave mode
                //enable(i2c.ack)
            );
        }
        template<typename TFreq = decltype(i2c_freq_def), typename TPs = decltype(i2c_ps_v<i2c_ps::def>)>
        static inline void begin_(TFreq freq = i2c_freq_def, TPs ps = i2c_ps_v<i2c_ps::def>){
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

        // ------------------------------ irq
        static constexpr auto irq = _i2c.irq;

        // ------------------------------ state
        static i2c_state state(){ return i2c_state { read_(_status) }; }
    };

    template<unsigned VNum, class TConfig = i2c_config>
    inline void start(const i2c<VNum, TConfig> i, i2c_start type = i2c_start::start){
        if (type != i2c_start::stop_start)
            i.start();
        else
            i.stop_start();
    }

    #include "i2c_impl.hpp"
};

#include "i2c_async.hpp"
#include "i2c_sync.hpp"

#endif
