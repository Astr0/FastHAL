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

namespace avr{
// clock speed
#ifdef TWBR
FH_DECLARE_REGISTER_ONLY(twbr0, TWBR);
#endif

// data register
#ifdef TWDR
FH_DECLARE_REGISTER_ONLY(twdr0, TWDR);
#endif

// slave address register, 7 bits address + TWGCE - enable recognition of broadcast (general call)
#ifdef TWAR
FH_DECLARE_REGISTER_ONLY(twar0, TWAR);
#endif

// status register
#ifdef TWSR
FH_DECLARE_REGISTER_ONLY(twsr0, TWSR);

// prescaler
constexpr auto twps0 = mField<(1 << TWPS0) | (1 << TWPS1), i2c_ps>(twsr0);
// status
constexpr auto tws0 = mField<(std::uint8_t)(~((1 << TWPS0) | (1 << TWPS1))), tw_s>(twsr0);
constexpr auto tws0_ps0 = mField<(std::uint8_t)(0xFF), tw_s>(twsr0);
#endif

// control register
#ifdef TWCR
FH_DECLARE_REGISTER_ONLY(twcr0, TWCR);

// interrupt flag, SCL is stretched while it's set, not automatically reset, reset by writing 1
#ifdef TWINT
constexpr auto twint0 = fieldBit<TWINT>(twcr0);
#endif

// Enable Acknowledge (disabling it virtually disconnects device from TWI)
#ifdef TWEA
constexpr auto twea0 = fieldBit<TWEA>(twcr0);
#endif

// Start condition will be generated (becomes master) when available. Must be cleared by software after transmitting
#ifdef TWSTA
constexpr auto twsta0 = fieldBit<TWSTA>(twcr0);
#endif

// Stop condition will be generated if master, in slave used to reset TWI and recover from error. Cleared automatically.
#ifdef TWSTO
constexpr auto twsto0 = fieldBit<TWSTO>(twcr0);
#endif

// Write collision flag - attempt to write to TWDR when TWINT is low. Cleared when writing TWDR when TWINT is high.
#ifdef TWWC
constexpr auto twwc0 = fieldBit<TWWC>(twcr0);
#endif

// enable bit
#ifdef TWEN
constexpr auto twen0 = fieldBit<TWEN>(twcr0);
#endif

// interrupt enable
#ifdef TWIE
constexpr auto twie0 = fieldBit<TWIE>(twcr0);
#endif

#endif

#if defined(TWBR) && defined(TWDR) && defined(TWCR)
    #define FH_HAS_I2C0
#endif
}

#if defined(TWI_vect_num)
#define FH_IRQ_I2C0 TWI_vect_num

constexpr auto irq_i2c0 = interrupt<FH_IRQ_I2C0>{};
namespace details{
    template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_i2c0)>>: 
        func_fieldbit_enable<decltype(avr::twie0)>,
        func_fieldbit_ready_reset<decltype(avr::twint0)>
        {};
}    
#endif

#ifdef FH_HAS_I2C0
namespace details{
    template<>
    struct i2c_impl<0>{
        static constexpr bool available = true;
        using status_t = avr::tw_s;

        static constexpr auto ps = ::fasthal::avr::twps0;
        static constexpr auto rate = ::fasthal::avr::twbr0;
        static constexpr auto status = ::fasthal::avr::tws0;
        static constexpr auto status0 = ::fasthal::avr::tws0_ps0;
        static constexpr auto data = ::fasthal::avr::twdr0;
        static constexpr auto control = ::fasthal::avr::twcr0;
        
        static constexpr auto enable = ::fasthal::avr::twen0;
        static constexpr auto ready = ::fasthal::avr::twint0;
        static constexpr auto ack = ::fasthal::avr::twea0;
        static constexpr auto start = ::fasthal::avr::twsta0;
        static constexpr auto stop = ::fasthal::avr::twsto0;
        
        static constexpr auto irq = irq_i2c0;
    };

    // enable for i2c.ack
    template<>
    struct func_fieldbit_impl<std::base_type_t<decltype(::fasthal::avr::twea0)>>:
        func_fieldbit_enable<decltype(::fasthal::avr::twea0)>
        { };
}
#endif