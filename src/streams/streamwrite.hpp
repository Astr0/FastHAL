#ifndef FH_STREAM_WRITE_H_
#define FH_STREAM_WRITE_H_

#include "stream.hpp"
#include "../std/std_types.hpp"
#include "../utils/functions.h"

namespace fasthal{
    // // int8
    // template<class T, details::enable_if_ostream<T> dummy = nullptr>
    // bool write(T ostream, std::int8_t n) {
    //     return write(ostream, static_cast<std::uint8_t>(n));
    // }

    // bytes
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    bool write(T ostream, const std::uint8_t *buffer, std::size_t size){        
        while (size--) {
            if (!write(ostream, *buffer++))
                return false;
        }
        return true;
    }

    //cstring
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    bool write(T ostream, const char* text) {
        char c;
        while ((c = *text++) != '\0'){
            if (!write(ostream, c))
                return false;
        }
        return write(ostream, char{});
    }

    // uint16
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    bool write(T ostream, std::uint16_t n) {
        auto c = alias_cast<std::uint8_t*>(&n);
        return 
            write(ostream, *c) && 
            write(ostream, *(c + 1));
    }

    // int16
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    bool write(T ostream, std::int16_t n) {
        return write(ostream, static_cast<std::uint16_t>(n));
    }

    // uint32
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    bool write(T ostream, std::uint32_t n) {
        auto c = alias_cast<std::uint8_t*>(&n);
        return 
            write(ostream, *c) && 
            write(ostream, *(c + 1)) &&
            write(ostream, *(c + 2)) &&
            write(ostream, *(c + 3));
    }    
    
    // int32
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    bool write(T ostream, std::int32_t n) {
        return write(ostream, static_cast<std::uint32_t>(n));
    }

    // float
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    bool write(T ostream, float n) {
        static_assert(sizeof(float)==sizeof(std::uint32_t), "Float is not 4 bytes");
        
        return write(ostream, alias_cast<std::uint32_t>(n));
    }

    // double
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    bool write(T ostream, double n) {        
        if constexpr (sizeof(double) == sizeof(std::uint32_t)){
            return write(ostream, alias_cast<std::uint32_t>(n));
        } else if constexpr(sizeof(double) == 2 * sizeof(std::uint32_t)){
            return write(ostream, *alias_cast<std::uint32_t*>(&n)) && write(ostream, *alias_cast<std::uint32_t*>(&n + sizeof(std::uint32_t))); 
        } else{
            static_assert(true, "Strange sizeof(double)");
        }
    }
}

#endif