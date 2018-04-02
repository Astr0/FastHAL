#pragma once

#ifndef UTILS_BINARYWRITER_H_
#define UTILS_BINARYWRITER_H_

#include "../std/std_types.hpp"
#include "stream.h"
#include "functions.h"

namespace fasthal{
    template<typename TStream>
    class BinaryWriter{
    private:
        TStream _stream;

        bool writeByte(const std::uint8_t byte) const{
            return _stream.write(byte);
        }

    public:
        constexpr BinaryWriter(const TStream stream): _stream(stream) { }

        bool write(const std::uint8_t *buffer, std::size_t size) const
        {
            // TODO: Same as in TextWriter, maybe move it somewhere?
            while (size--) {
                if (!writeByte(*buffer++))
                    return false;
            }
            return true;
        }

        bool write(const char* text) const{
            //return write((const uint8_t*)text, strlen(text)) && write((uint8_t)0);;
            char c;
            while ((c = *text++) != '\0'){
                if (!writeByte(c))
                    return false;
            }
            return writeByte(0);
        }

        bool write(std::uint8_t byte) const{
            return writeByte(byte);
        }

        bool write(std::uint16_t n) const{
            std::uint8_t* c = (std::uint8_t*)&n;
            return writeByte(*c) && 
                writeByte(*(c + 1));
        }

        bool write(std::uint32_t n) const{
            // return 
            //     writeByte(n) &&
            //     writeByte(n >> 8) &&
            //     writeByte(n >> 16) &&
            //     writeByte(n >> 24);
            std::uint8_t* c = (std::uint8_t*)&n;
            return 
                writeByte(*c) &&
                writeByte(*(c + 1)) &&
                writeByte(*(c + 2)) &&
                writeByte(*(c + 3));
        }

        bool write(std::int8_t n) const{
            return write((std::uint8_t)n);
        }

        bool write(std::int16_t n) const{
            return write((std::uint16_t)n);
        }

        bool write(std::int32_t n) const{
            return write((std::uint32_t)n);
        }

        bool write(float n) const {
            static_assert(sizeof(float)==sizeof(std::uint32_t), "Float is not 4 bytes");
            
            return write(alias_cast<std::uint32_t>(n));
        }

        bool write(double n) const {
            static_assert((sizeof(double) == sizeof(std::uint32_t)) || (sizeof(double) == 2 * sizeof(std::uint32_t)), "Strange sizeof(double)");
            if (sizeof(double) == sizeof(std::uint32_t)){
                return write(alias_cast<std::uint32_t>(n));
            } else if (sizeof(double) == 2 * sizeof(std::uint32_t)){
                return write(*alias_cast<std::uint32_t*>(&n)) && write(*alias_cast<std::uint32_t*>(&n + sizeof(std::uint32_t))); 
            } else{
                // shouldn't compile it cause of static_assert
                return false;
            }
        }
    };

    template<class TStream>
    constexpr BinaryWriter<StaticStream<TStream> > MakeBinaryWriter(){
        return BinaryWriter<StaticStream<TStream> >(StaticStream<TStream>());
    }

    BinaryWriter<OutStream&> MakeBinaryWriter(OutStream& stream){
        return BinaryWriter<OutStream&>(stream);
    }

}

#endif