#ifndef FH_I2C_HANDLER_H_
#define FH_I2C_HANDLER_H_

#include "i2c.hpp"
#include "../../std/std_types.hpp"
#include "../../utils/ringbuffer.hpp"
#include "interrupts.hpp"

namespace fasthal{
    enum class i2c_buf_s: std::uint8_t{
        ready = 0,
        done,
        mt,
        mt_flush,
        mr,
        mr_block,
        nack,
        error
    };

    template<class TI2c, unsigned VSize, typename TCallback = void (*)()>
    class i2c_buf{
        using i2c_t = TI2c;
        using callback_t = TCallback;

        volatile i2c_buf_s _state;
        volatile bsize_t _bytesLeft;
        ring_buffer<VSize> _buf;
        callback_t _callback;

        // should be called under lock with ok params
        bool start_impl(std::uint8_t sla, i2c_start type) {
            if (type == i2c_start::start){
                // ready for start - otherwise bus is busy
                if (_state != i2c_buf_s::ready) return false;
            } else {
                // done for restart and stop_start
                if (_state != i2c_buf_s::done) return false;
            }

            // buf should be empty in ready or done state, but just in case
            // callback should clear in stop
            // _buf.clear();

            _buf.write_dirty(sla);

            if  (type == i2c_start::stop_start){
                // stop start
                i2c_t::stop_start();
            } else { 
                // issues start for restart and start
                i2c_t::start();
            }
            
            return true;
        }

        bool start_mt_impl(std::uint8_t sla, i2c_start type){
            auto lock = no_irq{};
            if (!start_impl(sla, type))
                return false;
            _state = i2c_buf_s::mt;

            return true;
        }

        bool start_mr_impl(std::uint8_t sla, bsize_t count, callback_t callback, i2c_start type){
            // enable blocking mode?
            // if (count > _buf.capacity) 
            //     return false;

            auto lock = no_irq{};
            if (!start_impl(sla, type))
                return false;
            _callback = callback;
            _bytesLeft = count;
            _state = i2c_buf_s::mr;

            return true;
        }

        void tx_dirty(){
            i2c_t::tx(_buf.read_dirty());
        }
    public:
        constexpr i2c_buf(){}
        i2c_buf(const i2c_buf<TI2c, VSize>&) = delete;

        // state checking
        i2c_buf_s state() { return _state; }
        bool master_ok() { return _state == i2c_buf_s::done; }

        // start MT 
        template<typename TAddress>
        bool start_mt(TAddress address, i2c_start type = i2c_start::start){            
            return start_mt_impl(static_cast<const std::uint8_t>(details::i2c_build_sla<false>(address)), type);
        }

        // start MR
        template<typename TAddress>
        bool start_mr(TAddress address, bsize_t count, callback_t callback, i2c_start type = i2c_start::start){
            return start_mr_impl(
                    static_cast<const std::uint8_t>(details::i2c_build_sla<true>(address)), 
                    count, 
                    callback,
                    type);
        }
        
        // reads from buffer - called from callback no lock cause irq will wait for stop() that'll reset everything in case of error
        std::uint8_t read(){
            // we can get here in blocking or done mode. try to read some data in blocking mode
            while (true){
                // have something - great
                if (!_buf.empty())
                    return _buf.read_dirty();
                // check state - if it's not in mr_block then something went wrong
                if (_state != i2c_buf_s::mr_block)
                    return 0;
                // wait for data in blocking mode            
                i2c_t::try_irq_sync_no_irq();
            }
        }

        // buffered write - called before callback (so anything may happen on bus)
        // actual errors handled in flush 
        void write(std::uint8_t v){
            // sadly but we need lock here not to write in buffer when mode changes. 
            // or we can write, but clear buffer in cb in case of error
            //auto lock = no_irq{};
            // write buffered or blocking
            while (_state == i2c_buf_s::mt && !_buf.try_write(v))
                i2c_t::try_irq_sync_no_irq();
        }

        // end mt. callback can't be nullptr cause it has to do something 
        void flush(callback_t callback){
            {
                auto lock = no_irq{};
                if (_state == i2c_buf_s::mt){
                    _callback = callback;
                    _state = i2c_buf_s::mt_flush;
                    // try to tick irq in case we're done or run out of buf somewhere
                    i2c_t::try_irq_sync_no_irq();
                    return;
                }
            }
            // state not right... or done - invoke callback. it will stop to clean things up
            callback();
        }

        // return if everything went ok
        bool stop(){
            // no lock - it's only called from callback
            //auto lock = no_irq{};
            auto ok = _state == i2c_buf_s::done;
            if (ok){
                // really stop
                i2c_t::stop();
            }
            // clear buffer in case something was not read or error
            _buf.clear();
            // otherwise it's called from failed callback just release the bus
            _state = i2c_buf_s::ready;

            return ok;
        }

        // fsm
        void operator()(){
            auto s = i2c_t::state();
            auto state = _state;
            switch (s.state()){
                case i2c_s::m_start: // Entered START. Need select_w or select_r
                case i2c_s::m_restart: // Entered repeated START. Need select_w or select_r
                    // we shoule have something in buffer cause of start, right?
                    // send raw "select"
                    tx_dirty();
                    break;
                case i2c_s::mt: // select_w sent, received ACK. Need write or start/stop/stop_start
                case i2c_s::mt_write: // MT write, received ACK. Need write or start/stop/stop_start
                    // ok.. here we might not have anything in buffer... in this case END command should do something or buffer will overflow and we'll get in business again
                    // typically this should never happen in normal app, just for slow handlers or fast i2c's
                    // oh - and can't stop IRQ cause of slave mode
                    if (!_buf.empty()) {
                        tx_dirty();
                    } else if (state==i2c_buf_s::mt_flush)
                    {
                        _state = i2c_buf_s::done;
                        _callback();
                    }
                    // else send not done, but no buffer - flush or blocking write will trigger us again
                    break;
                case i2c_s::mt_nack: // select_w sent, received NACK. Need write or start/stop/stop_start
                case i2c_s::mt_write_nack: // MT write, received NACK. Need write or start/stop/stop_start
                case i2c_s::mr_nack: // select_r sent, received NACK. Need read, readlast, repeated start, stop, stop_start
                    // stop implicit
                    _state = i2c_buf_s::nack;
                    // goto error handling
                    goto error;
                case i2c_s::bus_fail:  // HW error on bus (invalid START/STOP condition). Need for bus restart.                    
                case i2c_s::m_la: // another master took of the bus unexpectedly in select_w, select_r or write/readl. Need fail or start.
                    // another stop
                    _state = i2c_buf_s::error;
                error:
                    i2c_t::stop();
                    if (state != i2c_buf_s::mt)
                        _callback();
                    break;
                case i2c_s::mr_read: // recevied byte ok. Need read/readl
                case i2c_s::mr_readl: // nack sent to slave after receiving byte, stop restart or stop/start will be transmitted, mr                    
                    if (_buf.full()) {
                        // out of buffer - run sync
                        // already running sync? impossible, but better not call callbacks recursively
                        _state = i2c_buf_s::mr_block;
                        goto mr_block_cb;
                        // if (state == i2c_buf_s::mr_block)
                        //     break;                        
                        // _callback();
                        //     //handle_master_done(i2c_buf_s::mr_block);                        
                        // break;
                    }
                    _buf.write_dirty(i2c_t::rx());
                    // fall to next case
                case i2c_s::mr: // select_r sent, received ACK. Need read/readl or start/stop/stop_start
                    if (_bytesLeft == 0) {
                        _state = i2c_buf_s::done;
                mr_block_cb:
                        if (state != i2c_buf_s::mr_block)
                            _callback();   
                        break;                     
                    } 
                    i2c_t::rx_ask(--_bytesLeft);
                    break;
                // slave statuses
                // case i2c_s::sr: // received own sla-w, ACK returned, will receive bytes and ACK/NACK, sr
                //     break;
                // case i2c_s::sr_la: // arbitration lost in master sla-r/w, slave address matched
                //     break;
                // case i2c_s::sr_cast: // broadcast has been received, ACK returned, will receive bytes and ACK/NACK, sr
                //     break;
                // case i2c_s::sr_cast_la: // arbitration lost in master sla-r/w, sla+w broadcast, will receive bytes and ACK/NACK, sr
                //     break;
                // case i2c_s::sr_read: // own data has been received, ACK returned, will receive bytes and ACK/NACK, sr
                //     break;
                // case i2c_s::sr_readl: // own data has been received, NACK returned, reseting TWI, sr
                //     break;
                // case i2c_s::sr_read_cast: // broadcast data has been received, ACK returned, will receive bytes and ACK/NACK, sr
                //     break;
                // case i2c_s::sr_readl_cast: // broadcast data has been received, NACK returned, reseting TWI, sr
                //     break;
                // case i2c_s::sr_stop_restart: // stop or start has been received while still addressed, reseting TWI, sr
                //     break;
                // case i2c_s::st: // received own sla-r, ACK returned, will send bytes, st
                //     break;
                // case i2c_s::st_la: // arbitration lost in master sla-r/w, slave address matched
                //     break;
                // case i2c_s::st_write: // data byte was transmitted and ACK has been received, will send bytes, st
                //     break;
                // case i2c_s::st_writel: // data byte was transmitted and NACK has been received, reseting TWI, st
                //     break;
                // case i2c_s::st_writel_ack: // last data byte was transmitted and ACK has been received, reseting TWI, st
                //     break;
                // // we shouldn't get ISR for ready state, so don't handle it
                // case i2c_s::ready: // no errors, ok state?
                //     break;
                default:
                    break;
            }
        }
    };

    // ostream & istream
    namespace details{
        template<class TI2c, unsigned VSize, typename TCallback>
        struct is_ostream_impl<i2c_buf<TI2c, VSize, TCallback>>: std::true_type{};

        template<class TI2c, unsigned VSize, typename TCallback>
        struct is_istream_impl<i2c_buf<TI2c, VSize, TCallback>>: std::true_type{};
    }
    template<class TI2c, unsigned VSize, typename TCallback>
    inline void write(i2c_buf<TI2c, VSize, TCallback>& i, std::uint8_t b){ i.write(b); }
    template<class TI2c, unsigned VSize, typename TCallback>
    inline std::uint8_t read(i2c_buf<TI2c, VSize, TCallback>& i){ return i.read(); }
}
#endif