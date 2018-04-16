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
        nack,
        error
    };

    enum class i2c_start: std::uint8_t{
        start = 0,
        stop_start = 1,
        restart = 2
    };

    constexpr auto i2c_buf_start = integral_constant<std::uint8_t, 0>{};
    constexpr auto i2c_buf_stop_start = integral_constant<std::uint8_t, 1>{};
    constexpr auto i2c_buf_restart = integral_constant<std::uint8_t, 2>{};

    template<class TI2c, unsigned VSize, typename TCallback = void (*)()>
    class i2c_buf{
        using i2c_t = TI2c;
        using callback_t = TCallback;

        volatile i2c_buf_s _state;
        volatile bsize_t _bytesLeft;
        ring_buffer<VSize> _buf;
        callback_t _callback;    

        void handle_master_done(){
            _state = i2c_buf_s::done;

            _callback();
        }

        void handle_master_error(i2c_buf_s state){
            // we can't get here from ready state, so something clearly wen't wrong
            {
                // this is called from irq or lock, don't need to tripple lock
                // auto lock = no_irq{};

                // stop i2c asap to free HW line
                i2c_t::stop();            
                // TODO...
                _state = state;
                // in case of MT flush will handle it        
                _buf.clear();
                
                // we're in MT and callback's not set yet. flush will call it
                if (_state == i2c_buf_s::mt)
                    return;
            }
            // signal callback that something wen't wrong and allow it to reset thing up
            _callback();
        }

        // should be called under lock with ok params
        bool start_impl(std::uint8_t sla, i2c_start type){
            if (type == i2c_start::start){
                // ready for start - otherwise bus is busy
                if (_state != i2c_buf_s::ready) return false;
            } else {
                // done for restart and stop_start
                if (_state != i2c_buf_s::done) return false;
            }

            // buf should be empty in ready state, but just in case
            _buf.clear();
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
            // can't handle blocking mode yet...
            if (count > _buf.capacity) 
                return false;

            auto lock = no_irq{};
            if (!start_impl(sla, type))
                return false;
            _callback = callback;
            _bytesLeft = count;
            _state = i2c_buf_s::mr;

            return true;
        }
    public:
        // state checking
        i2c_buf_s state() { return _state; }
        bool master_done() { return _state == i2c_buf_s::done; }

        // start MT 
        template<typename TAddress>
        bool start_mt(TAddress address, i2c_start type = i2c_start::start){            
            return start_mt_impl(static_cast<std::uint8_t>(details::i2c_build_sla<false>(address)), type);
        }

        // start MR
        template<typename TAddress>
        bool start_mr(TAddress address, bsize_t count, callback_t callback, i2c_start type = i2c_start::start){            
            return start_mr_impl(
                    static_cast<std::uint8_t>(details::i2c_build_sla<true>(address)), 
                    count, 
                    callback,
                    type);
        }
        
        std::uint8_t read(){
            // need noirq for state?
            // auto lock = no_irq{};
            // blocking read not supported, so just read...
            return _state == i2c_buf_s::done && !_buf.empty() ? _buf.read_dirty() : 0;
        }

        // buffered write
        void write(std::uint8_t v){
            // sadly but we need lock here not to write in buffer when mode changes
            // todo: think how to avoid it?
            auto lock = no_irq{};
            // write buffered or blocking
            while (_state == i2c_buf_s::mt && !_buf.try_write(v))
                i2c_t::try_irq_sync();
        }

        // end mt. callback can't be nullptr cause it has to do something 
        void flush(callback_t callback){            
            {
                auto lock = no_irq{};            
                if (_state == i2c_buf_s::mt){
                    _state = i2c_buf_s::mt_flush;
                    _callback = callback;
                    // try to tick irq in case we're done or run out of buf somewhere
                    i2c_t::try_irq_sync();
                    return;
                }
            }
            // state not right... or done - invoke callback
            callback();
        }

        void stop(){
            //auto lock = no_irq{};
            if (_state == i2c_buf_s::done){
                // really stop
                i2c_t::stop();
                // clear buffer just in case
                // _buf.clear();
            }
            // otherwise it's called from failed callback, just release the bus
            // we don't count for programmer errors in fasthal
            _state = i2c_buf_s::ready;
        }

        // fsm
        void operator()(){
            auto s = i2c_t::state();
            switch (s.state()){
                case i2c_s::m_start: // Entered START. Need select_w or select_r
                case i2c_s::m_restart: // Entered repeated START. Need select_w or select_r
                    // we shoule have something in buffer cause of start, right?
                    // send raw "select"
                    i2c_t::tx(_buf.read_dirty());
                    break;
                case i2c_s::mt: // select_w sent, received ACK. Need write or start/stop/stop_start
                case i2c_s::mt_write: // MT write, received ACK. Need write or start/stop/stop_start
                    // ok.. here we might not have anything in buffer... in this case END command should do something or buffer will overflow and we'll get in business again
                    // typically this should never happen in normal app, just for slow handlers or fast i2c's
                    // oh - and can't stop IRQ cause of slave mode
                    if (!_buf.empty())
                        i2c_t::tx(_buf.read_dirty());
                    else if (_state==i2c_buf_s::mt_flush)
                        handle_master_done();
                    break;
                case i2c_s::mt_nack: // select_w sent, received NACK. Need write or start/stop/stop_start
                case i2c_s::mt_write_nack: // MT write, received NACK. Need write or start/stop/stop_start
                case i2c_s::mr_nack: // select_r sent, received NACK. Need read, readlast, repeated start, stop, stop_start
                    // stop implicit
                    handle_master_error(i2c_buf_s::nack);
                    break;
                case i2c_s::bus_fail:  // HW error on bus (invalid START/STOP condition). Need for bus restart.                    
                case i2c_s::m_la: // another master took of the bus unexpectedly in select_w, select_r or write/readl. Need fail or start.
                    handle_master_error(i2c_buf_s::error);
                    break;
                case i2c_s::mr_read: // recevied byte ok. Need read/readl
                case i2c_s::mr_readl: // nack sent to slave after receiving byte, stop restart or stop/start will be transmitted, mr
                    _buf.write_dirty(i2c_t::rx());
                    // fall to next case
                case i2c_s::mr: // select_r sent, received ACK. Need read/readl or start/stop/stop_start
                    if (_bytesLeft == 0)
                        handle_master_done();
                    else
                        i2c_t::rx_ask((--_bytesLeft) != 0);
                    break;
                // slave statuses
                case i2c_s::sr: // received own sla-w, ACK returned, will receive bytes and ACK/NACK, sr
                    break;
                case i2c_s::sr_la: // arbitration lost in master sla-r/w, slave address matched
                    break;
                case i2c_s::sr_cast: // broadcast has been received, ACK returned, will receive bytes and ACK/NACK, sr
                    break;
                case i2c_s::sr_cast_la: // arbitration lost in master sla-r/w, sla+w broadcast, will receive bytes and ACK/NACK, sr
                    break;
                case i2c_s::sr_read: // own data has been received, ACK returned, will receive bytes and ACK/NACK, sr
                    break;
                case i2c_s::sr_readl: // own data has been received, NACK returned, reseting TWI, sr
                    break;
                case i2c_s::sr_read_cast: // broadcast data has been received, ACK returned, will receive bytes and ACK/NACK, sr
                    break;
                case i2c_s::sr_readl_cast: // broadcast data has been received, NACK returned, reseting TWI, sr
                    break;
                case i2c_s::sr_stop_restart: // stop or start has been received while still addressed, reseting TWI, sr
                    break;
                case i2c_s::st: // received own sla-r, ACK returned, will send bytes, st
                    break;
                case i2c_s::st_la: // arbitration lost in master sla-r/w, slave address matched
                    break;
                case i2c_s::st_write: // data byte was transmitted and ACK has been received, will send bytes, st
                    break;
                case i2c_s::st_writel: // data byte was transmitted and NACK has been received, reseting TWI, st
                    break;
                case i2c_s::st_writel_ack: // last data byte was transmitted and ACK has been received, reseting TWI, st
                    break;
                // we shouldn't get ISR for ready state, so don't handle it
                case i2c_s::ready: // no errors, ok state?
                    break;
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