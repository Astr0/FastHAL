#ifndef FH_I2C_SYNC_H_
#define FH_I2C_SYNC_H_

#include "i2c.hpp"

namespace fasthal {    
    template <class TI2c>
    class i2c_sync{
        static constexpr auto _i2c = TI2c{};
        bsize_t _bytesLeft;

        static bool start_impl(std::uint8_t sla){
            // check start state
            // select can't be here really according to our fsm
            if (!_i2c.state().can_start()) 
                return false;

            _i2c.start();
            while (!_i2c.ready());
            // don't check started. there should be no errors after start, it blocks
            // if (_f.get_state() != i2c_s::select) return false;

            _i2c.tx(sla);
            while (!_i2c.ready());

            return true;
        }
    public:
        constexpr i2c_sync(){}
        i2c_sync(const i2c_sync<TI2c>&) = delete;

        static inline auto state(){ return _i2c.state(); }

        template<typename TAddress>
        static void start_mt(TAddress address){
            start_impl(static_cast<const std::uint8_t>(details::i2c_build_sla<false>(address)));
        }

        template<typename TAddress>
        void start_mr(TAddress address, bsize_t count){
            if (start_impl(static_cast<const std::uint8_t>(details::i2c_build_sla<true>(address))))
                _bytesLeft = count;
        }

        static void write(std::uint8_t b){
            if (!state().mt_ok()) return;
            _i2c.tx(b);
            while (!_i2c.ready());
        }

        static i2c_state stop(){
            auto s = state();
            if (s.can_stop()){
                _i2c.stop();
                while (_i2c.stopping());
            }
            return s;
        }

        std::uint8_t read(){
            // return garabe
            if (!state().mr_ok()) return 0;
            // we don't check programmer issues with not telling how many bytes to read, etc.
            // Ask for next byte
            _i2c.rx_ask(--_bytesLeft);            
            while (!_i2c.ready());            
            
            // even if state is wrong, we should return some garbage data
            return _i2c.rx();
        }
    };

    // **********************************  sync mt ostream
    namespace details{
        template<class TI2c> struct is_ostream_impl<i2c_sync<TI2c>>: std::true_type{};
        template<class TI2c> struct is_istream_impl<i2c_sync<TI2c>>: std::true_type{};
    }
    template<class TI2c>
    void write(i2c_sync<TI2c>& mt, std::uint8_t b){ mt.write(b); }
    template<class TI2c>
    std::uint8_t read(i2c_sync<TI2c>& mr){ return mr.read(); }
}

#endif