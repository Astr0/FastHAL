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
// m_collision: enter not-addressed-slave (fail), start
// ***** action > states
// start -> m_start, m_restart
// select_w -> mt, mt_nack, m_collision
// write -> mt_write, mt_write_nack, m_collision
// stop -> ready
// stop_start -> m_start
// fail -> ready??

// Master Receive state machine (start+select_r)
// ***** state: action
// ready: start
// m_start: select_r
// m_restart: select_w, select_r
// m_collision: enter not-addressed-slave (fail), start
// mr: read, readlast, repeated start, stop, stop_start
// mr_nack: read, readlast, repeated start, stop, stop_start
// mr_read: read, readlast
// mr_readl: repeated start, stop, stop_start
// ***** action > states
// start -> m_start, m_restart
// select_r -> mr, mr_nack, m_collision
// read -> mr_read
// readlast -> mr_readl, m_collision
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
// start() - from states ready, mt, mt_nack, write, write_nack, m_collision, mr, mr_nack, mr_readl, st_writel, st_writel_ack, st_readl, st_readl_cast, sr_stop_restart
// stop() - from states bus_fail, mt, mt_nack, mt_write, mt_write_nack, mr, mr_nack, mr_readl
// write() - from states mt, mt_nack, mt_write, mt_write_nack, st, st_lp, st_write
// read() - from states mr, mr_nack, mr_read, sr, sr_la, sr_cast, sr_cast_la, sr_read, sr_read_cast
//

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

        static bsize_t bytesToRead;
        //static i2c_state state;
    };

    bsize_t i2c_impl<0>::bytesToRead{};

    // enable for i2c.ack
    template<>
    struct func_fieldbit_impl<std::base_type_t<decltype(::fasthal::avr::twea0)>>:
        func_fieldbit_enable<decltype(::fasthal::avr::twea0)>
        { };
}
//static i2c_state details::i2c_impl<0>::state = i2c_state::off;
//constexpr auto i2c0 = details::i2c_impl<0>{};

#endif