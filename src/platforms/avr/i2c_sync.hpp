#ifndef FH_I2C_SYNC_H_
#define FH_I2C_SYNC_H_

#include "i2c.hpp"

// simple sync i2c master handler.
// doesn't check for recusion reentry!!!
namespace fasthal {    
    template <class TI2c, class TArgs>
    class i2c_sync{
        static constexpr auto _i2c = TI2c{};
        
        static bool check_fail(TArgs& args){
            i2c_result res;
            if (_i2c.state().error())
                res = i2c_result::error;
            else if (_i2c.state().m_nack())
                res = i2c_result::nack;
            else 
                return false;
            args.status(res);
            args();
            return true;
        }

        static void do_read(TArgs& args){
            auto c = args.count();
            for(bsize_t i = 0; i != c;)
            {
                if (check_fail(args))
                    return;
                
                auto& p = args[i];
                _i2c.rx_ask(++i != c);
                while (!_i2c.ready());            
            
                // even if state is wrong, we should return some garbage data
                p = _i2c.rx();
            }
            args.status(i2c_result::done);
            args();
        }

        static void do_write(TArgs& args, bsize_t start = 0){
            auto c = args.count();
            for(auto i = start; i != c;)
            {
                if (check_fail(args))
                    return;
                
                _i2c.tx(args[i++]);
                while (!_i2c.ready());
            }
            args.status(i2c_result::done);
            args();
        }
    public:
        static void start(TArgs& args, i2c_start type = i2c_start::start){            
            fasthal::start(_i2c, type);
            while (!_i2c.ready());

            _i2c.tx(args[0]);
            while (!_i2c.ready());
            
            if (_i2c.state().mt_ok())
                do_write(args, 1);
            else
                do_read(args);               
        }

        static void more(TArgs& args){
            if (_i2c.state().mt_ok())
                do_write(args);
            else
                do_read(args);                
        }

        static void stop(){
            _i2c.stop();
            while (_i2c.stopping());
        }
    };    
}

#endif