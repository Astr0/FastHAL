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

// prescaler
enum class tw_ps: std::uint8_t{
    _1 = 0
    , _4 =  (0 << TWPS1) | (1 << TWPS0)
    , _16 = (1 << TWPS1) | (0 << TWPS0)
    , _64 = (1 << TWPS1) | (1 << TWPS0)
    , def = _1
};

template<tw_ps V>
static constexpr auto twps_v = integral_constant<tw_ps, V>{};

// status
enum class tw_s: std::uint8_t{
    // master statuses 
    // mt = master transmit
    // mr = master receive
    bus_fail            = 0x00, // hw error on bus    
    start               = 0x08, // start transmited, (mode) will be transmitted and waiting for ACK or NACK, mt, mr
    restart             = 0x10, // restart ok, (mode) will be transmitted and waiting for ACK or NACK, mt, mr
    sla_w_ack           = 0x18, // slave ack'ed for start write, mt
    sla_w_nack          = 0x20, // slave nack'ed for start write, mt
    byte_ack            = 0x28, // byte send ok, mt
    byte_nack           = 0x30, // byte send no ack, mt
    collision           = 0x38, // another master took of the bus unexpectedly, mt, mr
    sla_r_ack           = 0x40, // slave ack'ed for start read, mr
    sla_r_nack          = 0x48, // slave nack'ed for start read, mr
    recv_byte           = 0x50, // recevied byte ok. ACK or NACK will be send, mr
    recv_byte_nack      = 0x58, // nack sent to slave after receiving byte and sending nack, stop restart or stop/start will be transmitted, mr
    // slave statuses
    // st = slave transmit
    // sr = slave receive
    recv_sla_w          = 0x60, // received own sla-w, ACK returned, will receive bytes and ACK/NACK, sr
    recv_sla_w_lp       = 0x68, // arbitration lost in sla-w, will receive bytes and ACK/NACK, sr
    recv_sla_w_cast     = 0x70, // broadcast has been received, ACK returned, will receive bytes and ACK/NACK, sr
    recv_sla_w_lp_cast  = 0x78, // arbitration lost in broadcast, will receive bytes and ACK/NACK, sr
    recv_sla_byte       = 0x80, // own data has been received, ACK returned, will receive bytes and ACK/NACK, sr
    recv_sla_lbyte      = 0x88, // own data has been received, NACK returned, reseting TWI, sr
    recv_sla_byte_cast  = 0x90, // broadcast data has been received, ACK returned, will receive bytes and ACK/NACK, sr
    recv_sla_lbyte_cast = 0x98, // broadcast data has been received, NACK returned, reseting TWI, sr
    recv_restart        = 0xA0, // stop or start has been received while still addressed, reseting TWI, sr
    recv_sla_r          = 0xA8, // received own sla-r, ACK returned, will send bytes, st
    recv_sla_r_lp       = 0xB0, // arbitration lost in sla-r, will send bytes, st
    sla_byte_ack        = 0xB8, // data byte was transmitted and ACK has been received, will send bytes, st
    sla_byte_nack       = 0xC0, // data byte was transmitted and NACK has been received, reseting TWI, st
    sla_lbyte_ack       = 0xC8, // last data byte was transmitted and ACK has been received, reseting TWI, st
};

template<tw_s V>
static constexpr auto tws_v = integral_constant<tw_s, V>{};

// status register
#ifdef TWSR
FH_DECLARE_REGISTER_ONLY(twsr0, TWSR);

// prescaler
constexpr auto twps0 = mField<(1 << TWPS0) | (1 << TWPS1), tw_ps>(twsr0);
// status
constexpr auto tws0 = mField<(std::uint8_t)(~((1 << TWPS0) | (1 << TWPS1))), tw_s>(twsr0);
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