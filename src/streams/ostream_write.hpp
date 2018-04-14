#ifndef FH_OSTREAM_WRITE_H_
#define FH_OSTREAM_WRITE_H_

#include "stream.hpp"
#include "../std/std_types.hpp"
#include "../std/std_fake.hpp"
#include "../std/type_traits.hpp"
#include "../utils/functions.h"
#include <string.h>

namespace fasthal{
    // uint8
    // template<class T, details::enable_if_ostream<T> dummy = nullptr>
    // void write(T& ostream, std::uint8_t n) {
    //     write(ostream, &n, 1);
    // }

    // char
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    inline void write(T& ostream, char n) {
        write(ostream, static_cast<std::uint8_t>(n));
    }

    // int
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    inline void write(T& ostream, std::int8_t n) {
        write(ostream, static_cast<std::uint8_t>(n));
    }

    // bytes
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void write(T& ostream, const std::uint8_t *buffer, std::size_t size){        
        while (size--) 
            write(ostream, *buffer++);
    }

    namespace details{
        template<class T, details::enable_if_ostream<T> dummy = nullptr>
        void write_data_only(T& ostream, const char* text) {
            //write(ostream, reinterpret_cast<const std::uint8_t*>(text), strlen(text));
            char c;
            while ((c = *text++) != '\0')
                write(ostream, c);
        }
    }

    //cstring
    template<class T, //typename TChars,
     details::enable_if_ostream<T> dummy = nullptr>
     //std::enable_if_c<std::is_same<TChars, const char*>::value> dummy2 = nullptr>
    void write(T& ostream, const char* text) {
        //write(ostream, reinterpret_cast<const std::uint8_t*>(text), strlen(text) + 1);
        details::write_data_only(ostream, text);
        write(ostream, '\0');
    }

    // // fixed string
    // template<class T, unsigned N,
    //  details::enable_if_ostream<T> dummy = nullptr>
    // void write(T& ostream, const char (&text)[N]) {
    //     //write(ostream, reinterpret_cast<const std::uint8_t*>(text), N);
    //     details::write_data_only(ostream, text);
    //     write(ostream, '\0');
    // }

    // uint16
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void write(T& ostream, std::uint16_t n) {
        auto c = reinterpret_cast <std::uint8_t*>(&n);

        //write(ostream, c, 2);
        write(ostream, *c++); 
        write(ostream, *c);
    }

    // int16
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void write(T& ostream, std::int16_t n) {
        write(ostream, static_cast<std::uint16_t>(n));
    }

    // uint32
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void write(T& ostream, std::uint32_t n) {
        auto c = reinterpret_cast<std::uint16_t*>(&n);
        
        //write(ostream, c, 4);
        write(ostream, *c++);
        write(ostream, *c);
        // write(ostream, *c++);
        // write(ostream, *c);
    }    
    
    // int32
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void write(T& ostream, std::int32_t n) {
        write(ostream, static_cast<std::uint32_t>(n));
    }

    // float
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void write(T& ostream, float n) {
        static_assert(sizeof(float) == 4, "Float is not 4 bytes");
        
        write(ostream, alias_cast<std::uint32_t>(n));
    }

    // double
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void write(T& ostream, double n) {        
        if constexpr (sizeof(double) == 4){
            write(ostream, alias_cast<std::uint32_t>(n));
        } else if constexpr(sizeof(double) == 8){
            // auto c = reinterpret_cast<std::uint8_t*>(&n);
            // write(ostream, c, 8);
            auto c = reinterpret_cast<std::uint32_t*>(&n);
            write(ostream, *c++); 
            write(ostream, *c); 
        } else{
            static_assert(true, "Strange sizeof(double)");
        }
    }
}

#endif