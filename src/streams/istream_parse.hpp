#ifndef FH_ISTREAM_PARSE_H_
#define FH_ISTREAM_PARSE_H_

#include "stream.hpp"
#include "../std/std_types.hpp"
#include "../std/std_fake.hpp"
#include "../std/type_traits.hpp"
#include "../utils/functions.h"
#include "istream_read.hpp"
#include <string.h>

namespace fasthal{
    namespace details{
        template<typename T, class TStream>
        T parse_number(TStream& stream){
            static constexpr bool allow_negative = (static_cast<T>(-1) < 0);
            auto result = T{};
            if constexpr (allow_negative){
                auto negative = false;
                while (true){
                    auto c = read_char(stream);
                    if (c == '-')
                        negative = true;
                    
                    auto v = static_cast<std::uint8_t>(c - '0');
                    if (v > 9)
                        break;
                    result = result * 10 + v;
                }
                if (negative)
                    result = -result;                
            } else {
                 while (true){
                    auto v = static_cast<std::uint8_t>(read_char(stream) - '0');
                    if (v > 9)
                        break;
                    result = result * 10 + v;
                }
            }           
            
            return result;
        }
    }

    // u0
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    auto parse_u8(T& istream) {
        return details::parse_number<std::uint8_t>(istream);
    }

    // char
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    auto parse_char(T& istream) { 
        return read_char(istream);
    }

    // int
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    auto parse_i8(T& istream) {
        return details::parse_number<std::int8_t>(istream);
    }

    // uint16
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    auto parse_u16(T& istream) {
        return details::parse_number<std::uint16_t>(istream);
    }

    // int16
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    auto parse_i16(T& istream) {
        return details::parse_number<std::int16_t>(istream);
    }

    // uint32
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    auto parse_u32(T& istream) {
        return details::parse_number<std::uint32_t>(istream);
    }    
    
    // int32
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    auto parse_i32(T& istream) {
         return details::parse_number<std::int32_t>(istream);
    }

    // float
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    auto parse_float(T& istream) {
        // TODO
        return 0.0f;
        // static_assert(sizeof(float) == 4, "Float is not 4 bytes");
        
        // return alias_cast<float>(parse_u32(istream));
    }

    // double
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    auto parse_double(T& istream) {        
        // TODO
        return 0.0;
    }

    // string
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    bsize_t parse_string(T& istream, char *str){
        // this may overflow str! no checks!
        return read_string(istream, str);
    }    

    namespace details{
        std::uint8_t from_hex_char(char c)
        {
            if (c <= '9')
                return c - '0';
            if (c >= 'a') 
                return c - 'a' + 10;
            return c - 'A' + 10;
        }
    }

    // hex
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    bsize_t parse_hex(T& istream, std::uint8_t *buffer){
        // this may overflow buffer! no checks!
        bsize_t size = 0;
        while (true){
            auto c = read_char(istream);
            if (c == '\0')
                break;
            // first byte
            auto v = details::from_hex_char(c) << 4;
            // second byte
            v |= details::from_hex_char(read_char(istream));
            ++size;
        }
        return size;
    }
}

#endif