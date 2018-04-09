#ifndef FH_OSTREAM_WRITE_H_
#define FH_OSTREAM_WRITE_H_

#include "stream.hpp"
#include "../std/std_types.hpp"
#include "../utils/functions.h"

namespace fasthal{
    // int8
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void write(T ostream, char n) {
        write(ostream, static_cast<std::uint8_t>(n));
    }

    // bytes
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void write(T ostream, const std::uint8_t *buffer, std::size_t size){        
        while (size--) 
            write(ostream, *buffer++);
    }

    namespace details{
        template<class T, details::enable_if_ostream<T> dummy = nullptr>
        void write_data_only(T ostream, const char* text) {
            char c;
            while ((c = *text++) != '\0')
                write(ostream, c);
        }
    }

    //cstring
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void write(T ostream, const char* text) {
        details::write_data_only(ostream, text);
        write(ostream, char{0});
    }

    // uint16
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void write(T ostream, std::uint16_t n) {
        auto c = reinterpret_cast <std::uint8_t*>(&n);
        
        write(ostream, *c++);
        write(ostream, *c);
    }

    // int16
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void write(T ostream, std::int16_t n) {
        write(ostream, static_cast<std::uint16_t>(n));
    }

    // uint32
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void write(T ostream, std::uint32_t n) {
        auto c = reinterpret_cast<std::uint8_t*>(&n);
        
        write(ostream, *c++);
        write(ostream, *c++);
        write(ostream, *c++);
        write(ostream, *c);
    }    
    
    // int32
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void write(T ostream, std::int32_t n) {
        write(ostream, static_cast<std::uint32_t>(n));
    }

    // float
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void write(T ostream, float n) {
        static_assert(sizeof(float)==sizeof(std::uint32_t), "Float is not 4 bytes");
        
        write(ostream, alias_cast<std::uint32_t>(n));
    }

    // double
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void write(T ostream, double n) {        
        if constexpr (sizeof(double) == sizeof(std::uint32_t)){
            write(ostream, alias_cast<std::uint32_t>(n));
        } else if constexpr(sizeof(double) == 2 * sizeof(std::uint32_t)){
            auto c = reinterpret_cast<std::uint32_t*>(&n);
            write(ostream, *c++);
            write(ostream, *c); 
        } else{
            static_assert(true, "Strange sizeof(double)");
        }
    }
}

#endif