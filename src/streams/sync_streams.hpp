#ifndef FH_SYNC_STREAMS_H_
#define FH_SYNC_STREAMS_H_

#include "stream.hpp"
#include "../std/type_traits.hpp"
#include "../std/std_types.hpp"

namespace fasthal{
    // ************************* Transmitter *************************
    template<class TTarget>
    struct sync_transmitter{
        static constexpr auto async = false;

        // transmitter, shouldn't be here
        // static inline has_byte next();
    };

    namespace details{
        template<class T>
        struct is_ostream_impl<sync_transmitter<T>>: std::true_type{};
    }

    // ostream
    template<class TTarget>
    inline void write(sync_transmitter<TTarget> trans, std::uint8_t c){
        while (!TTarget::try_write(c))
        {
            // nop
        }
    }

    // ostream nice stuff
    template<class TTarget>
    inline void flush(sync_transmitter<TTarget> trans){
        TTarget::flush();
    }

    // ************************* Receiver *************************
    template<class TSource>
    struct sync_receiver{
        // TODO
        // sync and async are completely different!
        // sync - calls read on target
        // async - calls try_read_sync on target and receives callback
    };

    namespace details{
        template<class T>
        struct is_istream_impl<sync_receiver<T>>: std::true_type{};
    }    
    
    // istream
    template<class TSource>
    inline std::uint8_t read(sync_receiver<TSource> recv) {
        return TSource::read();
    }
};

#endif