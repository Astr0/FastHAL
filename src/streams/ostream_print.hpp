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

    static constexpr auto numberbase_def = numberbase_v<10>;

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
       
        template<typename TStream, typename T, unsigned VBase = 10, details::enable_if_ostream<TStream> dummy = nullptr>
        bool print_number(TStream ostream, T n, integral_constant<unsigned, VBase>) {
            static constexpr auto size = print_number_buffer_size<T, VBase>();
            char buf[size]; // Assumes 8-bit chars plus zero byte.
            char *str = &buf[sizeof(buf)];
                        
            do {
                char c = n % VBase;
                n /= VBase;
                if constexpr (VBase <= 10){
                    *(--str) = c + '0';
                } else{
                    *(--str) = c < 10 ? c + '0' : c + 'A' - 10;
                }
            } while(n); 
            return write(ostream, reinterpret_cast<uint8_t*>(str), &buf[sizeof(buf)] - str);
        }        
    }

    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    bool println(T ostream){ 
        return write(ostream, '\r') && write(ostream, '\n');
    }

    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    bool print(T ostream, const char* text){
        return details::write_data_only(ostream, text);
    }

    // template<class T, unsigned N, details::enable_if_ostream<T> dummy = nullptr>
    // bool print(T ostream, const char (&text)[N]){
    //     return write(ostream, reinterpret_cast<std::uint8_t*>(text), N - 1);
    // }

    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    bool println(T ostream, const char* text){
        return print(ostream, text) && println(ostream);
    }

    template<class T, unsigned VBase = 10, details::enable_if_ostream<T> dummy = nullptr>
    bool print(T ostream, std::uint8_t n, numberbase_t<VBase> base = numberbase_def){
        return details::print_number(ostream, n, base);
    }

    template<class T, unsigned VBase = 10, details::enable_if_ostream<T> dummy = nullptr>
    bool println(T ostream, std::uint8_t n, numberbase_t<VBase> base = numberbase_def){
        return print(ostream, n, base) && println(ostream);
    }

    template<class T, unsigned VBase = 10, details::enable_if_ostream<T> dummy = nullptr>
    bool print(T ostream, std::uint16_t n, numberbase_t<VBase> base = numberbase_def){
        return details::print_number(ostream, n, base);
    }

    template<class T, unsigned VBase = 10, details::enable_if_ostream<T> dummy = nullptr>
    bool println(T ostream, std::uint16_t n, numberbase_t<VBase> base = numberbase_def){
        return print(ostream, n, base) && println(ostream);
    }

    template<class T, unsigned VBase = 10, details::enable_if_ostream<T> dummy = nullptr>
    bool print(T ostream, std::uint32_t n, numberbase_t<VBase> base = numberbase_def){
        return details::print_number(ostream, n, base);
    }

    template<class T, unsigned VBase = 10, details::enable_if_ostream<T> dummy = nullptr>
    bool println(T ostream, std::uint32_t n, numberbase_t<VBase> base = numberbase_def){
        return print(ostream, n, base) && println(ostream);
    }

    template<class T, unsigned VBase = 10, details::enable_if_ostream<T> dummy = nullptr>
    bool print(T ostream, std::int8_t n, numberbase_t<VBase> base = numberbase_def){
        if (n < 0){
            if (!write(ostream, '-'))
                return false;
            n = -n;
        }
        return print(ostream, static_cast<std::uint8_t>(n));
    }

    template<class T, unsigned VBase = 10, details::enable_if_ostream<T> dummy = nullptr>
    bool println(T ostream, std::int8_t n, numberbase_t<VBase> base = numberbase_def){
        return print(ostream, n, base) && println(ostream);
    }

        template<class T, unsigned VBase = 10, details::enable_if_ostream<T> dummy = nullptr>
    bool print(T ostream, std::int16_t n, numberbase_t<VBase> base = numberbase_def){
        if (n < 0){
            if (!write(ostream, '-'))
                return false;
            n = -n;
        }
        return print(ostream, static_cast<std::uint8_t>(n));
    }

    template<class T, unsigned VBase = 10, details::enable_if_ostream<T> dummy = nullptr>
    bool println(T ostream, std::int16_t n, numberbase_t<VBase> base = numberbase_def){
        return print(ostream, n, base) && println(ostream);
    }

    template<class T, unsigned VBase = 10, details::enable_if_ostream<T> dummy = nullptr>
    bool print(T ostream, std::int32_t n, numberbase_t<VBase> base = numberbase_def){
        if (n < 0){
            if (!write(ostream, '-'))
                return false;
            n = -n;
        }
        return print(ostream, static_cast<std::uint8_t>(n));
    }

    template<class T, unsigned VBase = 10, details::enable_if_ostream<T> dummy = nullptr>
    bool println(T ostream, std::int32_t n, numberbase_t<VBase> base = numberbase_def){
        return print(ostream, n, base) && println(ostream);
    }

    namespace details{
        template<typename TStream, typename T, details::enable_if_ostream<TStream> dummy = nullptr>
        bool print_float(TStream ostream, T n, std::uint8_t digits) {
            if (isnan(n)) return print(ostream, "nan");
            if (isinf(n)) return print(ostream, "inf");
            if (n > 4294967295.0) return print(ostream, "ovf");
            if (n < -4294967295.0) return print(ostream, "ovf");

            if (n < 0){
                if (!write(ostream, '-'))
                    return false;
                n = -n;
            }

            // Round correctly so that print(1.999, 2) prints as "2.00"
            T rounding = 0.5;
            for (std::uint8_t i=0; i<digits; ++i)
                rounding /= 10.0;

            n += rounding;

            // Extract the integer part of the number and print it
            std::uint32_t int_part = (std::uint32_t)n;
            if (!print(ostream, int_part))
                return false;
            n -= (T)int_part;
                
            // Print the decimal point, but only if there are digits beyond
            if (digits > 0) {
                if (!write(ostream, '.'))
                    return false;
            }

            // Extract digits from the remainder one at a time
            while (digits-- > 0)
            {
                n *= 10.0;
                auto toPrint = (std::uint8_t)(n);
                char printC = '0' + toPrint;
                if (!write(ostream, printC))
                    return false;
                n -= toPrint; 
            } 
            return true;
        }
    }

    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    bool print(T ostream, float n, std::uint8_t digits = 2) {
        return details::print_float(ostream, n, digits);
    }

    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    bool println(T ostream, float n, std::uint8_t digits = 2) {
        return print(ostream, n, digits) && println(ostream);
    }
    
    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    bool print(T ostream, double n, std::uint8_t digits = 2) {
        // TODO: Use float print if sizeof(double) == sizeof(float) should reduce code size
        return details::print_float(ostream, n, digits);
    }

    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    bool println(T ostream, double n, std::uint8_t digits = 2) {
        return print(ostream, n, digits) && println(ostream);
    }
}

#endif