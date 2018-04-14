#ifndef FH_istream_read_H_
#define FH_istream_read_H_

#include "stream.hpp"
#include "../std/std_types.hpp"
#include "../std/std_fake.hpp"
#include "../std/type_traits.hpp"
#include "../utils/functions.h"
#include <string.h>

namespace fasthal{
    // u0
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    inline auto read_u8(T& istream) {
        return static_cast<std::uint8_t>(read(istream));
    }

    // char
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    auto read_char(T& istream) {
        return static_cast<char>(read_u8(istream));
    }

    // int
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    auto read_i8(T& istream) {
        return static_cast<std::int8_t>(read_u8(istream));
    }

    // bytes
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    void read_buf(T& istream, std::uint8_t *buffer, std::size_t size){        
        while (size--)
            *buffer++ = read_u8(istream);
    }

    // uint16
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    auto read_u16(T& istream) {
        auto c = std::uint16_t { read_u8(istream) };
        c = (c << 8) | read_u8(istream);
        return c;
    }

    // int16
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    auto read_i16(T& istream) {
        return static_cast<std::uint16_t>(read_u16(istream));
    }

    // uint32
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    auto read_u32(T& istream) {
        auto c = std::uint32_t{ read_u16(istream) };
        c  = (c << 16) | read_u16(istream);
        return c;
    }    
    
    // int32
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    auto read_i32(T& istream) {
         return static_cast<std::int32_t>(read_u32(istream));
    }

    // float
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    auto read_float(T& istream) {
        static_assert(sizeof(float) == 4, "Float is not 4 bytes");
        
        return alias_cast<float>(read_u32(istream));
    }

    // double
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    auto read_double(T& istream) {        
        if constexpr (sizeof(double) == 4){
            return alias_cast<double>(read_u32(istream));
        } else if constexpr(sizeof(double) == 8){
            // auto c = reinterpret_cast<std::uint8_t*>(&n);
            // read(istream, c, 8);
            double result;
            auto c = reinterpret_cast<std::uint32_t*>(&result);
            *c++ = read_u32(istream);
            *c = read_u32(istream);
            return result;
        } else{
            static_assert(true, "Strange sizeof(double)");
        }
    }
}

#endif