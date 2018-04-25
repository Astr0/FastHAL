#ifndef FH_OSTREAM_PRINT_H_
#define FH_OSTREAM_PRINT_H_

#include "stream.hpp"
#include "ostream_write.hpp"
#include "../std/std_types.hpp"
#include "../utils/functions.h"
#include <math.h>

namespace fasthal{
    template<unsigned V>
    using numberbase_t = integral_constant<unsigned, V>;

    template<unsigned V>
    static constexpr auto numberbase_v = numberbase_t<V>{};

    static constexpr auto numberbase_dec = numberbase_v<10>;
    static constexpr auto numberbase_bin = numberbase_v<2>;
    static constexpr auto numberbase_hex = numberbase_v<16>;
    static constexpr auto numberbase_def = numberbase_dec;    

    namespace details{
        template <typename T, unsigned VBase>
        constexpr auto print_number_buffer_size(){
            T max = ~T{};
            uint8_t size = 0;
            while (max != 0)
            {
                max /= VBase;
                size++;
            }
            return size + 1;
        }
       
        template<typename T, typename TNumber, unsigned VBase = 10, details::enable_if_ostream<T> dummy = nullptr>
        void print_number(T& ostream, TNumber n, integral_constant<unsigned, VBase>) {
            static constexpr auto size = print_number_buffer_size<TNumber, VBase>();
            char buf[size]; // Assumes 8-bit chars plus zero byte.
            char *str = &buf[sizeof(buf)];

            *(--str) = '\0';
            //auto len = bsize_t{0};
            do {
                char c = n % VBase;
                n /= VBase;
                if constexpr (VBase <= 10){
                    *(--str) = c + '0';
                } else{
                    *(--str) = c < 10 ? c + '0' : c + 'A' - 10;
                }
                //len++;
            } while(n); 
            details::write_data_only(ostream, str);
            //write(ostream, reinterpret_cast<uint8_t*>(str), len);
        }        
    }
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void print(T& ostream, char c){
        write(ostream, c);
    }

    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void println(T& ostream){ 
        // constexpr std::uint8_t buf[2] = {'\r', '\n'};
        // write(ostream, buf, 2);
        write(ostream, '\r');
        write(ostream, '\n');
    }

    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void print(T& ostream, const char* text){
        details::write_data_only(ostream, text);
    }

    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void println(T& ostream, const char* text){
        print(ostream, text);
        println(ostream);
    }

    template<class T, unsigned VBase = 10, details::enable_if_ostream<T> dummy = nullptr>
    void print(T& ostream, std::uint8_t n, numberbase_t<VBase> base = numberbase_def){
        details::print_number(ostream, n, base);
    }

    template<class T, unsigned VBase = 10, details::enable_if_ostream<T> dummy = nullptr>
    void println(T& ostream, std::uint8_t n, numberbase_t<VBase> base = numberbase_def){
        print(ostream, n, base);
        println(ostream);
    }

    template<class T, unsigned VBase = 10, details::enable_if_ostream<T> dummy = nullptr>
    void print(T& ostream, std::uint16_t n, numberbase_t<VBase> base = numberbase_def){
        details::print_number(ostream, n, base);
    }

    template<class T, unsigned VBase = 10, details::enable_if_ostream<T> dummy = nullptr>
    void println(T& ostream, std::uint16_t n, numberbase_t<VBase> base = numberbase_def){
        print(ostream, n, base);
        println(ostream);
    }

    template<class T, unsigned VBase = 10, details::enable_if_ostream<T> dummy = nullptr>
    void print(T& ostream, std::uint32_t n, numberbase_t<VBase> base = numberbase_def){
        details::print_number(ostream, n, base);
    }

    template<class T, unsigned VBase = 10, details::enable_if_ostream<T> dummy = nullptr>
    void println(T& ostream, std::uint32_t n, numberbase_t<VBase> base = numberbase_def){
        print(ostream, n, base);
        println(ostream);
    }

    template<class T, unsigned VBase = 10, details::enable_if_ostream<T> dummy = nullptr>
    void print(T& ostream, std::int8_t n, numberbase_t<VBase> base = numberbase_def){
        if (n < 0){
            write(ostream, '-');
            n = -n;
        }
        print(ostream, static_cast<std::uint8_t>(n), base);
    }

    template<class T, unsigned VBase = 10, details::enable_if_ostream<T> dummy = nullptr>
    void println(T& ostream, std::int8_t n, numberbase_t<VBase> base = numberbase_def){
        print(ostream, n, base);
        println(ostream);
    }

    template<class T, unsigned VBase = 10, details::enable_if_ostream<T> dummy = nullptr>
    void print(T& ostream, std::int16_t n, numberbase_t<VBase> base = numberbase_def){
        if (n < 0){
            write(ostream, '-');
            n = -n;
        }
        print(ostream, static_cast<std::uint8_t>(n), base);
    }

    template<class T, unsigned VBase = 10, details::enable_if_ostream<T> dummy = nullptr>
    void println(T& ostream, std::int16_t n, numberbase_t<VBase> base = numberbase_def){
        print(ostream, n, base);
        println(ostream);
    }

    template<class T, unsigned VBase = 10, details::enable_if_ostream<T> dummy = nullptr>
    void print(T& ostream, std::int32_t n, numberbase_t<VBase> base = numberbase_def){
        if (n < 0){
            write(ostream, '-');
            n = -n;
        }
        print(ostream, static_cast<std::uint8_t>(n), base);
    }

    template<class T, unsigned VBase = 10, details::enable_if_ostream<T> dummy = nullptr>
    void println(T& ostream, std::int32_t n, numberbase_t<VBase> base = numberbase_def){
        print(ostream, n, base);
        println(ostream);
    }

    namespace details{
        template<typename TStream, typename T, details::enable_if_ostream<TStream> dummy = nullptr>
        void print_float(TStream& ostream, T n, std::uint8_t digits) {
            if (isnan(n)) print(ostream, "nan");
            else if (isinf(n)) print(ostream, "inf");
            else if (n > 4294967295.0) print(ostream, "ovf");
            else if (n < -4294967295.0) print(ostream, "ovf");
            else {
                if (n < 0){
                    write(ostream, '-');
                    n = -n;
                }
                
                // Round correctly so that print(1.999, 2) prints as "2.00"
                auto rounding = T{ 0.5 };
                for (std::uint8_t i=0; i < digits; ++i)
                    rounding *= 0.1;
                
                n += rounding;

                // Extract the integer part of the number and print it
                auto int_part = static_cast<std::uint32_t>(n);
                print(ostream, int_part);
                n -= (T)int_part;
                    
                // Print the decimal point, but only if there are digits beyond
                if (digits) 
                    write(ostream, '.');                

                // Extract digits from the remainder one at a time
                while (digits-- > 0)
                {
                    n *= 10;
                    auto toPrint = static_cast<std::uint8_t>(n) % 10;
                    char printC = '0' + toPrint;
                    write(ostream, printC);
                    //n -= toPrint; 
                } 
            }
        }
    }

    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void print(T& ostream, float n, std::uint8_t digits = 2) {
        details::print_float(ostream, n, digits);
    }

    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void println(T& ostream, float n, std::uint8_t digits = 2) {
        print(ostream, n, digits);
        println(ostream);
    }
    
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void print(T& ostream, double n, std::uint8_t digits = 2) {
        details::print_float(ostream, n, digits);
    }

    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void println(T& ostream, double n, std::uint8_t digits = 2) {
        print(ostream, n, digits);
        println(ostream);
    }

    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void print_hex(T& ostream, std::uint8_t v){
        auto c0 = (v >> 4);
        write(ostream, c0 <= 0x09 ? ('0' + c0)  : ('A' - 10 + c0));
        auto c1 = v & 0x0F;
        write(ostream, c1 <= 0x09 ? ('0' + c1)  : ('A' - 10 + c1));
    }

    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void print_hex(T& ostream, const std::uint8_t* buf, bsize_t size) {
        while (size--){
            print_hex(ostream, *buf++);
        }
    }

}

#endif