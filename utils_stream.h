#pragma once

#ifndef UTILS_STREAM_H_
#define UTILS_STREAM_H_

#include <inttypes.h>

namespace fasthal{
    class OutStream{
        public:
            virtual bool write(uint8_t c) = 0;
    };

    template<class TStream>
    class StaticStream{
    public:
        constexpr StaticStream(){}
        inline bool write(uint8_t c) const{ return TStream::write(c);}
    };

    template<class TStream>
    class StaticOutStream: public OutStream{
    public:
        constexpr StaticOutStream(){}
        inline bool write(uint8_t c) { return TStream::write(c);}
    };

    template<class TStream>
    constexpr StaticOutStream<TStream> MakeStaticOutStream(){
        return StaticOutStream<TStream>();
    }
}

#endif
