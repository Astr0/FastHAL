#ifndef FH_I2C_NET_H_
#define FH_I2C_NET_H_

#include "../../std/std_types.hpp"
#include "../../hal/i2c.hpp"
#include "i2c.hpp"
#include "interrupts.hpp"
#include "../../mp/holder.hpp"
#include "../../mp/static_func.hpp"

// should be nice async i2c handler
namespace fasthal{

    template<typename TBuf = buffer_t>
    class test_args {
        using args_t = test_args<TBuf>;

        using callback_t = void(*)(args_t&); //mp::static_func<void(args_t&)>;
        // size of buffer
        bsize_t _count;
        // operation status
        std::uint8_t _status;
        // buffer
        TBuf _buf;
        callback_t _callback;
    public: 
        bsize_t count()const {return _count;}
        void count(bsize_t c) { _count = c; }
        
        template<typename... T>
        bool status_any(T... s)const { return is_any(_status, static_cast<std::uint8_t>(s)...); }
        template<typename T>
        void status(T s) { _status = static_cast<std::uint8_t>(s); } 
        
        void buffer(TBuf buf) { _buf = buf; }
        std::uint8_t& operator[](bsize_t i) { return _buf[i]; }
        std::uint8_t operator[](bsize_t i)const { return _buf[i]; }

        void operator()() { _callback(*this); }
        void callback(callback_t cb){ _callback = cb; } 
    };

    template<class TI2c, typename TArgsPtr>
    class i2c_net: mp::holder<TArgsPtr> {        
        static constexpr auto _i2c = TI2c{};

        using args_ptr_t = TArgsPtr;
        using args_t = decltype(*std::declval<args_ptr_t>());
        using args_holder_t = mp::holder<args_ptr_t>;

        volatile bsize_t _index;        

        args_t& args() { return *(this->args_holder_t::get());}
        void set_args(args_t& args) {
            if constexpr(!mp::is_static_v<TArgsPtr>)
                this->args_holder_t::set(&args); 
        }
        
        struct lazy{
            static constexpr auto async = details::has_isr<_i2c.irq.number>;
        };

        bool fsm(i2c_result& res){
            auto i = _index;
            auto c = args().count();
            uint8_t b = args()[i];
            // return here - exit
            // break == call irq
            switch (_i2c.state().state()){
                case i2c_s::mt: // select_w sent, received ACK. Need write or start/stop/stop_start
                case i2c_s::mt_write: // MT write, received ACK. Need write or start/stop/stop_start
                    // done, goto irq
                    if (i == c) {
                        res = i2c_result::done;
                        break;
                    }
                tx:
                    _index = ++i;
                txo:
                    _i2c.tx(b);
                    return false;
                case i2c_s::m_start: // Entered START. Need select_w or select_r
                case i2c_s::m_restart: // Entered repeated START. Need select_w or select_r
                    // SLA+W, just normal TX it
                    if ((b & 1) == 0)
                        goto tx;
                    // count bytes will be overriden in place of SLA
                    goto txo;
                case i2c_s::mt_nack: // select_w sent, received NACK. Need write or start/stop/stop_start
                case i2c_s::mt_write_nack: // MT write, received NACK. Need write or start/stop/stop_start
                case i2c_s::mr_nack: // select_r sent, received NACK. Need read, readlast, repeated start, stop, stop_start
                    // call IRQ
                    res = i2c_result::nack;
                    break;
                case i2c_s::bus_fail:  // HW error on bus (invalid START/STOP condition). Need for bus restart.                    
                case i2c_s::m_la: // another master took of the bus unexpectedly in select_w, select_r or write/readl. Need fail or start.
                    // call IRQ
                    res = i2c_result::error;
                    break;
                case i2c_s::mr_read: // recevied byte ok. Need read/readl
                case i2c_s::mr_readl: // nack sent to slave after receiving byte, stop restart or stop/start will be transmitted, mr                    
                    args()[i] = _i2c.rx();
                    _index = ++i;
                case i2c_s::mr: // select_r sent, received ACK. Need read/readl or start/stop/stop_start
                    if (c == i) {
                        res = i2c_result::done;
                        break;
                    }
                    // i < c
                    _i2c.rx_ask(++i != c);
                    return false;
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
                    return false;
            }
            return true;
        }
    public:
        i2c_net(){}

        // first byte in buffer should be SLA
        bool start(args_t& args, i2c_start type = i2c_start::start) {            
            auto lock = no_irq{};

            _index = 0;
            set_args(args);
   
            // count should be at least 1 (SLA)
            if (type != i2c_start::stop_start)
                _i2c.start();
            else
                _i2c.stop_start();
            
            // we're busy probably.. better check it
            // but don't check state since we async and control it nicely
            // and we can restart master op after slave is done or throw error in CB, right?
            // !_i2c.state().can_start()

            return false;
        }

        // adds more data to last operation, should be called from ISR
        bool more(args_t& args) {
            _index = 0;
            set_args(args);
            // tick the isr
            if (ready_(_i2c.irq))
                run(_i2c.irq);
            return false;
        }

        inline static void stop(){
            _i2c.stop();
        }

        // ISR - just do what it takes
        void operator()(){
            static_assert(lazy::async, "only for async op");
            i2c_result res;
            if (fsm(res)){
                //_index = 0;
                args().status(res);
                args()();
            }
        }
    };
}

#endif