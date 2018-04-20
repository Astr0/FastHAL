#ifndef FH_UART_ASYNC_H_
#define FH_UART_ASYNC_H_

#include "../../mp/holder.hpp"

namespace fasthal{
    template<class TUart, typename TArgsPtr>
    class uart_async_tx: mp::holder<TArgsPtr>{
        static constexpr auto _uart= TUart{};
        using args_ptr_t = TArgsPtr;
        using args_t = decltype(*std::declval<args_ptr_t>());
        using args_holder_t = mp::holder<args_ptr_t>;
        
        volatile bsize_t _index;

        struct lazy{
            static constexpr auto async = details::has_isr<_uart.irq_txr.number>;
        };

        args_t& args() { return *(this->args_holder_t::get());}
        void set_args(args_t& args) {
            if constexpr(!mp::is_static_v<TArgsPtr>)
                this->args_holder_t::set(&args);
        }        
    public:
        constexpr uart_async_tx(){}

        void tx(args_t& args){
            _index = 0;
            set_args(args);
            enable_(_uart.irq_txr);
        }

        void operator()(){
            _uart.tx(args().buffer()[_index]);
            if (++_index == args().count()){
                args()();
                if (_index != 0)
                    disable_(_uart.irq_txr);
            }
        }
    };

    template<class TUart, typename TArgsPtr>
    class uart_async_rx: mp::holder<TArgsPtr>{
        static constexpr auto _uart= TUart{};
        using args_ptr_t = TArgsPtr;
        using args_t = decltype(*std::declval<args_ptr_t>());
        using args_holder_t = mp::holder<args_ptr_t>;
        
        volatile bsize_t _index;

        struct lazy{
            static constexpr auto async = details::has_isr<_uart.irq_rxc.number>;
        };

        args_t& args() { return *(this->args_holder_t::get());}
        void set_args(args_t& args) {
            if constexpr(!mp::is_static_v<TArgsPtr>)
                this->args_holder_t::set(&args);
        }        
    public:
    constexpr uart_async_rx(){}

        void rx(args_t& args){
            set_args(args);
            _index = 0;
            enable_(_uart.irq_rxc);
        }

        void operator()(){            
            auto ok = _uart.rx_ok();
            auto c = _uart.rx();
            if (!ok)
                return;                        
            args().buffer()[_index] = c;
            if (++_index == args().count()){
                args()();
                if (_index != 0)
                    disable_(_uart.irq_rxc);
            } 
        }
    };
}

#endif