#ifndef FH_I2C_SYNC_H_
#define FH_I2C_SYNC_H_

#include "i2c.hpp"

// simple sync i2c master handler.
// doesn't check for recusion reentry!!!
namespace fasthal {    
    template <class TI2c, class TArgs>
    class i2c_sync{
        static constexpr auto _i2c = TI2c{};
        
        static void do_master(TArgs& args, bsize_t i = 0){
            auto c = args.count();
            auto buf = args.buffer();
            auto write = _i2c.state().mt_ok();
            i2c_result res;
            while (true){
                switch(_i2c.state().state())
                {
                    case i2c_s::bus_fail:
                    case i2c_s::m_la:
                        res = i2c_result::error;
                        goto end;
                    case i2c_s::mr_nack:
                    case i2c_s::mt_nack:
                    case i2c_s::mt_write_nack:
                        res = i2c_result::nack;
                        goto end;
                    default:
                        break;
                }
                if (c == i){
                    res = i2c_result::done;
                    goto end;
                }
                
                auto& p = buf[i];
                ++i;       

                if (write)
                    _i2c.tx(p);
                else
                    _i2c.rx_ask(i != c);                

                while (!_i2c.ready());

                if (!write)
                    p = _i2c.rx();                
            }
        end:
            args.status(res);
            args();
        }
    public:
        static void start(TArgs& args, i2c_start type = i2c_start::start){            
            fasthal::start(_i2c, type);
            while (!_i2c.ready());

            _i2c.tx(args[0]);
            while (!_i2c.ready());
            
            do_master(args, (args[0] & 1) ? 0 : 1);
        }

        static void more(TArgs& args){
            do_master(args);
        }

        static void stop(){
            _i2c.stop();
            while (_i2c.stopping());
        }
    };    
}

#endif