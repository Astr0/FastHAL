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
        write(trans.target, c);
    }

    // transmitter
    template<class TTarget>
    inline auto next(sync_transmitter<TTarget> trans){
        // shouldn't be here technically
        return has_byte_none;
    }

    // commit on transmitter - ensures that everything that was written was send to target, not really written by target!
    template<class TTarget>
    inline void commit(sync_transmitter<TTarget> trans){
        //flush(trans.target);
    }

};

#endif