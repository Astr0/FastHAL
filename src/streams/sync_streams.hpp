#ifndef FH_SYNC_STREAMS_H_
#define FH_SYNC_STREAMS_H_

#include "stream.hpp"
#include "../std/type_traits.hpp"
#include "../std/std_types.hpp"

namespace fasthal{
    template<class TTarget>
    struct sync_transmitter{
        //using target_t = TTarget;
        static constexpr TTarget target = TTarget{};
        static constexpr auto async = false;
    };

    namespace details{
        template<class T>
        struct is_ostream_impl<sync_transmitter<T>>: std::true_type{};
    }

    // ostream
    template<class TTarget>
    inline void write(sync_transmitter<TTarget> trans, std::uint8_t c){
        while (!try_write(trans.target, c))
        {
            // nop
        }
        //write(trans.target, c);
    }

    // transmitter, shouldn't be here
    template<class TTarget>
    inline has_byte next(sync_transmitter<TTarget> trans);
    
    template<class TTarget>
    inline void flush(sync_transmitter<TTarget> trans){
        flush(trans.target);
        //flush(trans.target);
    }

};

#endif