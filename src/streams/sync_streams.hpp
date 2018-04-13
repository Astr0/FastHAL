#ifndef FH_SYNC_STREAMS_H_
#define FH_SYNC_STREAMS_H_

#include "stream.hpp"
#include "../std/type_traits.hpp"
#include "../std/std_types.hpp"

namespace fasthal{
    template<class TTarget>
    struct sync_transmitter{
        static constexpr auto async = false;

        // transmitter, shouldn't be here
        static inline has_byte next();
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

};

#endif