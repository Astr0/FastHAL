#ifndef FH_BUFFER_STREAMS_H_
#define FH_BUFFER_STREAMS_H_

#include "stream.hpp"

namespace fasthal{
    class buffer_istream{
        const std::uint8_t* _buf;
        bsize_t _count;
    public:
        buffer_istream(const std::uint8_t* buf, const bsize_t count): _buf(buf), _count(count){}
        
        std::uint8_t read(){
            if (_count == 0) return 0;
            --_count;
            return *_buf++;
        }
    };

    namespace details{
        template<>
        struct is_istream_impl<buffer_istream>: std::true_type{};
    }

    std::uint8_t read(buffer_istream& stream){ return stream.read(); }
}

#endif