#ifndef FH_AVR_I2C_SYNC_H_
#define FH_AVR_I2C_SYNC_H_

#include "i2c.hpp"

// simple sync i2c master handler.
// doesn't check for recusion reentry!!!
namespace fasthal {    
    template <class TI2c>
    class i2c_sync{
        static constexpr auto _i2c = TI2c{};
        
        static i2c_result do_master(buffer_t buf, bsize_t c, bool write){
            while (true){
                switch(_i2c.state().state())
                {
                    case i2c_s::bus_fail:
                    case i2c_s::m_la:
                        return i2c_result::error;
                    case i2c_s::mr_nack:
                    case i2c_s::mt_nack:
                    case i2c_s::mt_write_nack:
                        return i2c_result::nack;
                    default:
                        break;
                }
                if (c == 0)
                    return i2c_result::done;
                
                --c;
                if (write)
                    _i2c.tx(*buf++);
                else
                    _i2c.rx_ask(c != 0);                

                while (!_i2c.ready());

                if (!write)
                    *buf++ = _i2c.rx();
                
                //++buf;
            }
        }

        static i2c_result start(buffer_t buf, bsize_t count, i2c_start type = i2c_start::start){
            fasthal::start(_i2c, type);
            while (!_i2c.ready());

            auto a0 = *buf;
            _i2c.tx(a0);
            auto write = (a0 & 1) == 0;
            // we've written 1 byte already
            if (write){
                ++buf;
                --count;
            }
            while (!_i2c.ready());            
            
            return do_master(buf, count, write);
        }
    public:
        static constexpr auto async() { return false; }

        template<class TArgs>
        static void start(TArgs& args, i2c_start type = i2c_start::start){
            // don't template everything!
            auto status = start(args.buffer(), args.count(), type);
            args.status(status);
        }

        template<class TArgs>
        static void more(TArgs& args){
            auto status = do_master(args.buffer(), args.count(), _i2c.state().mt_ok());
            args.status(status);
        }

        static void stop(){
            _i2c.stop();
            while (_i2c.stopping());
        }
    };    
}

#endif