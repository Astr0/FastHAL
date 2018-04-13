#ifndef FH_RINGBUFFER_STREAMS_H_
#define FH_RINGBUFFER_STREAMS_H_

#include "../utils/ringbuffer.hpp"

namespace fasthal{
    template<class TTarget, unsigned VCapacity>
    struct ring_buffer_transmitter{
        static constexpr auto async = true;        
        static ring_buffer<VCapacity> buffer;

        // transmitter
        static inline auto next(){
            auto c = buffer.read_dirty();
            auto last = buffer.empty();
            return has_byte{ c, last };
        }
    };

    // buggy gcc AVR generates wierd intialization for this
    // template<class TTarget, unsigned VCapacity>
    // ring_buffer<VCapacity> ring_buffer_transmitter<TTarget, VCapacity>::buffer{};

    namespace details{
        template<class T, unsigned C>
        struct is_ostream_impl<ring_buffer_transmitter<T, C>>: std::true_type{};
    }

    // ostream
    template<class TTarget, unsigned C>
    inline void write(ring_buffer_transmitter<TTarget, C> trans, std::uint8_t c){
        // shortcut if empty buffer -> try write something sync
        if (trans.buffer.empty() && TTarget::try_write(c))           
            return;

        // wait for buffer space
        while (!trans.buffer.try_write(c))
            TTarget::try_write_sync();
        
        // start writing
        TTarget::write_async();
    }


    // commit on transmitter - ensures that everything that was written was send to target, not really written by target!
    template<class TTarget, unsigned C>
    inline void flush(ring_buffer_transmitter<TTarget, C> trans){
        TTarget::flush();
    }
}
#endif