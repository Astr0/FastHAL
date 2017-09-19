#pragma once

#ifndef UTILS_STREAM_H_
#define UTILS_STREAM_H_

#include <inttypes.h>

namespace fasthal{
    class OutStream{
        public:
            virtual bool write(uint8_t c) = 0;
    };

    template<class T>
    class OutStreamRef: public OutStream{
        private:
            T& _stream;
        public:
            constexpr OutStreamRef(T& stream): _stream(stream){                
            }

            virtual bool write(uint8_t c) const{
                return _stream.write(c);
            }
    };

    template<class T>
    class OutStreamWrap{
        private:
            T& _stream;
        public:
            constexpr OutStreamWrap(T& stream): _stream(stream){                
            }

            inline bool write(uint8_t c) const{
                return _stream.write(c);
            }
    };
}

#endif
