#ifndef FH_AVR_SPI_ASYNC_H_
#define FH_AVR_SPI_ASYNC_H_

#include "../../mp/holder.hpp"

namespace fasthal{
    template<class TSpi, typename TArgsPtr>
    class spi_async: mp::holder<TArgsPtr>{
        static constexpr auto _spi= TSpi{};
        using args_ptr_t = TArgsPtr;
        using args_t = decltype(*std::declval<args_ptr_t>());
        using args_holder_t = mp::holder<args_ptr_t>;
        
        volatile bsize_t _index;

        args_t& args() { return *(this->args_holder_t::get());}
        void set_args(args_t& args) {
            if constexpr(!mp::is_static_v<TArgsPtr>)
                this->args_holder_t::set(&args);
        }        
    public:
        constexpr spi_async(){ static_assert(details::has_isr<_spi.irq.number>, "IRQ not set! Use FH_SPI(*this*)"); }

        static constexpr bool async() { return true; }

        void transfer(args_t& args){
            _index = 0;
            set_args(args);
            enable_(_spi.irq);
        }

        void operator()(){
            // do RX first
            auto rx = _spi.rx();
            auto& p = args().buffer()[_index];
            // transfer next byte
            _spi.tx(p);
            // and set byte to RX
            p = rx;
            // callback
            if (++_index == args().count()){
                args()();
                if (_index != 0)
                    disable_(_spi.irq);
            }
        }
    };
}

#endif