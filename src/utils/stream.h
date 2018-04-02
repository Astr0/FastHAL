#pragma once

#ifndef UTILS_STREAM_H_
#define UTILS_STREAM_H_

#include "../std/std_types.hpp"

namespace fasthal{
    class OutStream{
        public:
            virtual bool write(std::uint8_t c) = 0;
    };

    template<class TStream>
    class StaticStream{
    public:
        constexpr StaticStream(){}
        inline bool write(std::uint8_t c) const{ return TStream::write(c);}
    };

    template<class TStream>
    class StaticOutStream: public OutStream{
    public:
        constexpr StaticOutStream(){}
        inline bool write(std::uint8_t c) { return TStream::write(c);}
    };

    template<class TStream>
    constexpr StaticOutStream<TStream> MakeOutStream(){
        return StaticOutStream<TStream>();
    }
}

#endif
