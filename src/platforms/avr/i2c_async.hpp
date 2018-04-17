#ifndef FH_I2C_ASYNC_H_
#define FH_I2C_ASYNC_H_

#include "../../std/std_types.hpp"
#include "interrupts.hpp"

// should be nice async i2c handler
namespace fasthal{
    enum class i2c_async_r: std::uint8_t{
        done = 0,
        nack = 1,
        error= 2        
    };

    template<class TI2c, typename TCallback = void(*)(i2c_async_r)>
    class i2c_async{
        static constexpr auto _i2c = TI2c{};
        volatile std::uint8_t* _buf;
        volatile bsize_t _count;
        TCallback _callback;
    public:
        // first byte in buffer should be SLA
        bool start(
            std::uint8_t* buf,
            bsize_t count,
            TCallback callback) {

            auto lock = no_irq{};
            // count should be at least 1 (SLA)
            
            // we're busy probably.. better check it
            // but don't check state since we async and control it nicely
            // !_i2c.state().can_start()
            if (_count != 0)
                return false;
            
            _buf = buf;
            _count = count;
            _callback = callback;

            _i2c.start();
            // start will call ISR, no worries :D
            return true;
        }

        inline static void stop(){
            _i2c.stop();
        }

        // ISR - just do what it takes
        void operator()(){
            auto c = _count;
            auto v = _buf;
            auto res = i2c_async_r::done;
            // return here - exit
            // break == call irq
            switch (_i2c.state().state()){
                case i2c_s::mt: // select_w sent, received ACK. Need write or start/stop/stop_start
                case i2c_s::mt_write: // MT write, received ACK. Need write or start/stop/stop_start
                    // done, goto irq
                    if (c == 0) 
                        break;
                tx:       
                    _i2c.tx(*v);
                    _count = c - 1;
                    _buf = v + 1;
                    return;
                case i2c_s::m_start: // Entered START. Need select_w or select_r
                case i2c_s::m_restart: // Entered repeated START. Need select_w or select_r
                    // SLA+W, just normal TX it
                    if (((*v) & 1) == 0)
                        goto tx;
                    // count bytes will be overriden in place of SLA
                    _i2c.tx(*v);
                    return;
                case i2c_s::mt_nack: // select_w sent, received NACK. Need write or start/stop/stop_start
                case i2c_s::mt_write_nack: // MT write, received NACK. Need write or start/stop/stop_start
                case i2c_s::mr_nack: // select_r sent, received NACK. Need read, readlast, repeated start, stop, stop_start
                    // call IRQ
                    res = i2c_async_r::nack;
                    break;
                case i2c_s::bus_fail:  // HW error on bus (invalid START/STOP condition). Need for bus restart.                    
                case i2c_s::m_la: // another master took of the bus unexpectedly in select_w, select_r or write/readl. Need fail or start.
                    // call IRQ
                    res = i2c_async_r::error;
                    break;
                case i2c_s::mr_read: // recevied byte ok. Need read/readl
                case i2c_s::mr_readl: // nack sent to slave after receiving byte, stop restart or stop/start will be transmitted, mr                    
                    *v = _i2c.rx();
                    _buf = v + 1;
                case i2c_s::mr: // select_r sent, received ACK. Need read/readl or start/stop/stop_start
                    if (c == 0) 
                        break;
                    _i2c.rx_ask(--c);
                    _count = c;
                    return;
                // slave statuses
                case i2c_s::sr: // received own sla-w, ACK returned, will receive bytes and ACK/NACK, sr
                case i2c_s::sr_la: // arbitration lost in master sla-r/w, slave address matched
                case i2c_s::sr_cast: // broadcast has been received, ACK returned, will receive bytes and ACK/NACK, sr
                case i2c_s::sr_cast_la: // arbitration lost in master sla-r/w, sla+w broadcast, will receive bytes and ACK/NACK, sr
                case i2c_s::sr_read: // own data has been received, ACK returned, will receive bytes and ACK/NACK, sr
                case i2c_s::sr_readl: // own data has been received, NACK returned, reseting TWI, sr
                case i2c_s::sr_read_cast: // broadcast data has been received, ACK returned, will receive bytes and ACK/NACK, sr
                case i2c_s::sr_readl_cast: // broadcast data has been received, NACK returned, reseting TWI, sr
                case i2c_s::sr_stop_restart: // stop or start has been received while still addressed, reseting TWI, sr
                case i2c_s::st: // received own sla-r, ACK returned, will send bytes, st
                case i2c_s::st_la: // arbitration lost in master sla-r/w, slave address matched
                case i2c_s::st_write: // data byte was transmitted and ACK has been received, will send bytes, st
                case i2c_s::st_writel: // data byte was transmitted and NACK has been received, reseting TWI, st
                case i2c_s::st_writel_ack: // last data byte was transmitted and ACK has been received, reseting TWI, st
                // we shouldn't get ISR for ready state, so don't handle it
                case i2c_s::ready: // no errors, ok state?
                default:
                    return;
            }
            _count = 0;
            _callback(res);
        }
    };
}

#endif