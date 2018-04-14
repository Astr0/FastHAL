#ifndef FH_OSTREAM_read_H_
#define FH_OSTREAM_read_H_

#include "stream.hpp"
#include "../std/std_types.hpp"
#include "../std/std_fake.hpp"
#include "../std/type_traits.hpp"
#include "../utils/functions.h"
#include <string.h>

namespace fasthal{
    // u0
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    inline std::uint8_t read_u8(T ostream) {
        return read(ostream);
    }

    // char
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    inline char read_char(T ostream,  n) {
        return static_cast<char>(read_u8(n));
    }

    // int
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    inline void read(T ostream, std::int8_t n) {
        read(ostream, static_cast<std::uint8_t>(n));
    }

    // bytes
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    void read(T ostream, const std::uint8_t *buffer, std::size_t size){        
        while (size--) 
            read(ostream, *buffer++);
    }

    namespace details{
        template<class T, details::enable_if_istream<T> dummy = nullptr>
        void read_data_only(T ostream, const char* text) {
            //read(ostream, reinterpret_cast<const std::uint8_t*>(text), strlen(text));
            char c;
            while ((c = *text++) != '\0')
                read(ostream, c);
        }
    }

    //cstring
    template<class T, //typename TChars,
     details::enable_if_istream<T> dummy = nullptr>
     //std::enable_if_c<std::is_same<TChars, const char*>::value> dummy2 = nullptr>
    void read(T ostream, const char* text) {
        //read(ostream, reinterpret_cast<const std::uint8_t*>(text), strlen(text) + 1);
        details::read_data_only(ostream, text);
        read(ostream, '\0');
    }

    // // fixed string
    // template<class T, unsigned N,
    //  details::enable_if_istream<T> dummy = nullptr>
    // void read(T ostream, const char (&text)[N]) {
    //     //read(ostream, reinterpret_cast<const std::uint8_t*>(text), N);
    //     details::read_data_only(ostream, text);
    //     read(ostream, '\0');
    // }

    // uint16
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    void read(T ostream, std::uint16_t n) {
        auto c = reinterpret_cast <std::uint8_t*>(&n);

        //read(ostream, c, 2);
        read(ostream, *c++); 
        read(ostream, *c);
    }

    // int16
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    void read(T ostream, std::int16_t n) {
        read(ostream, static_cast<std::uint16_t>(n));
    }

    // uint32
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    void read(T ostream, std::uint32_t n) {
        auto c = reinterpret_cast<std::uint16_t*>(&n);
        
        //read(ostream, c, 4);
        read(ostream, *c++);
        read(ostream, *c);
        // read(ostream, *c++);
        // read(ostream, *c);
    }    
    
    // int32
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    void read(T ostream, std::int32_t n) {
        read(ostream, static_cast<std::uint32_t>(n));
    }

    // float
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    void read(T ostream, float n) {
        static_assert(sizeof(float) == 4, "Float is not 4 bytes");
        
        read(ostream, alias_cast<std::uint32_t>(n));
    }

    // double
    template<class T, details::enable_if_istream<T> dummy = nullptr>
    void read(T ostream, double n) {        
        if constexpr (sizeof(double) == 4){
            read(ostream, alias_cast<std::uint32_t>(n));
        } else if constexpr(sizeof(double) == 8){
            // auto c = reinterpret_cast<std::uint8_t*>(&n);
            // read(ostream, c, 8);
            auto c = reinterpret_cast<std::uint32_t*>(&n);
            read(ostream, *c++); 
            read(ostream, *c); 
        } else{
            static_assert(true, "Strange sizeof(double)");
        }
    }
}

#endif