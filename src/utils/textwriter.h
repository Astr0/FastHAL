// #pragma once

// #ifndef UTILS_TEXTWRITER_H_
// #define UTILS_TEXTWRITER_H_

// #include "../std/std_types.hpp"
// #include "stream.h"

// namespace fasthal{
//     namespace priv{
//         template<std::uint8_t TSize>
//         struct PrintBufferSize{
//             // shouldn't be here actually
//             //const static uint8_t size = 2 * TSize + 3;
//             //const static uint8_t binSize = 8 * TSize + 1;
//         };

//         template<>
//         struct PrintBufferSize<4>{
//             const static std::uint8_t decSize = 11;
//             const static std::uint8_t binSize = 4 * 8 + 1;
//         };

//         template<>
//         struct PrintBufferSize<2>{
//             const static std::uint8_t decSize = 6;
//             const static std::uint8_t binSize = 2 * 8 +1;
//         };

//         template<>
//         struct PrintBufferSize<1>{
//             const static std::uint8_t decSize = 4;
//             const static std::uint8_t binSize = 8 + 1;
//         };
//     }


//     template<typename TStream>
//     class TextWriter{
//     private:
//         TStream _stream;

//         template<typename T>
//         bool printNumber(T n) const{
//             char buf[priv::PrintBufferSize<sizeof(T)>::decSize]; // Assumes 8-bit chars plus zero byte.
//             char *str = &buf[sizeof(buf) - 1];
          
//             *str = '\0';

//             do {
//                 char c = n % 10;
//                 n /= 10;
//                 *--str = c + '0';
//             } while(n); 
//             return print(str);
//         }

//         template<typename T>
//         bool printNumber(T n, std::uint8_t base) const{
//             char buf[priv::PrintBufferSize<sizeof(T)>::binSize]; // Assumes 8-bit chars plus zero byte.
//             char *str = &buf[sizeof(buf) - 1];
          
//             *str = '\0';

//             do {
//                 char c = n % base;
//                 n /= base;
//                 *--str = c < 10 ? c + '0' : c + 'A' - 10;
//             } while(n); 
//             return print(str);
//         }

//         template<typename T>
//         bool printFloat(T n, std::uint8_t digits) const{
//             if (isnan(n)) return print("nan");
//             if (isinf(n)) return print("inf");
//             if (n > 4294967295.0) return print ("ovf");  
//             if (n < -4294967295.0) return print ("ovf");

//             if (n < 0){
//                 if (!write('-'))
//                     return false;
//                 n = -n;
//             }

//             // Round correctly so that print(1.999, 2) prints as "2.00"
//             T rounding = 0.5;
//             for (std::uint8_t i=0; i<digits; ++i)
//                 rounding /= 10.0;

//             n += rounding;

//             // Extract the integer part of the number and print it
//             std::uint32_t int_part = (std::uint32_t)n;
//             if (!print(int_part))
//                 return false;
//             n -= (T)int_part;
                
//             // Print the decimal point, but only if there are digits beyond
//             if (digits > 0) {
//                 if (!write('.'))
//                     return false;
//             }

//             // Extract digits from the remainder one at a time
//             while (digits-- > 0)
//             {
//                 n *= 10.0;
//                 std::uint8_t toPrint = (std::uint8_t)(n);
//                 if (!print(toPrint))
//                     return false;
//                 n -= toPrint; 
//             } 
//             return true;
//         }
//     public:
//         constexpr TextWriter(const TStream stream): _stream(stream) { }
        
//         bool write(const std::uint8_t byte) const{
//             return _stream.write(byte);
//         }

//         bool write(const std::uint8_t *buffer, std::size_t size) const
//         {
//           while (size--) {
//             if (!write(*buffer++))
//                 return false;
//           }
//           return true;
//         }

//         bool println() const{ 
//             return write('\r') && write('\n');
//         }

//         bool print(const char* text) const{
//             //return write((const uint8_t*)text, strlen(text));
//             char c;
//             while ((c = *text++) != '\0'){
//                 if (!write(c))
//                     return false;
//             }
//             return true;
//         }

//         bool println(const char* text) const{
//             return print(text) && println();
//         }

//         bool print(std::uint8_t n) const{
//             return printNumber(n);
//         }

//         bool println(std::uint8_t n) const {
//             return print(n) && println();
//         }

//         bool print(std::uint8_t n, std::uint8_t base) const{
//             return printNumber(n, base);
//         }

//         bool println(std::uint8_t n, std::uint8_t base) const {
//             return print(n, base) && println();
//         }

//         bool print(std::uint16_t n) const{
//             return printNumber(n);
//         }

//         bool println(std::uint16_t n) const {
//             return print(n) && println();
//         }

//         bool print(std::uint16_t n, std::uint8_t base) const{
//             return printNumber(n, base);
//         }

//         bool println(std::uint16_t n, std::uint8_t base) const {
//             return print(n, base) && println();
//         }

//         bool print(std::uint32_t n) const{
//             return printNumber(n);
//         }

//         bool println(std::uint32_t n) const {
//             return print(n) && println();
//         }

//         bool print(std::uint32_t n, std::uint8_t base) const{
//             return printNumber(n, base);
//         }

//         bool println(std::uint32_t n, std::uint8_t base) const {
//             return print(n, base) && println();
//         }

//         bool print(std::int8_t n) const{
//             if (n < 0){
//                 if (!write('-'))
//                     return false;
//                 n = -n;
//             }
//             return print((std::uint8_t)n);
//         }

//         bool println(std::int8_t n) const {
//             return print(n) && println();
//         }

//         bool print(std::int16_t n) const{
//             if (n < 0){
//                 if (!write('-'))
//                     return false;
//                 n = -n;
//             }
//             return print((std::uint16_t)n);
//         }

//         bool println(std::int16_t n) const {
//             return print(n) && println();
//         }

//         bool print(std::int32_t n) const{
//             if (n < 0){
//                 if (!write('-'))
//                     return false;
//                 n = -n;
//             }
//             return print((std::uint32_t)n);
//         }

//         bool println(std::int32_t n) const {
//             return print(n) && println();
//         }

//         bool print(float n, std::uint8_t digits = 2) const {
//             return printFloat(n, digits);
//         }

//         bool println(float n, std::uint8_t digits = 2) const {
//             return print(n, digits) && println();
//         }
        
//         bool print(double n, std::uint8_t digits = 2) const {
//             // TODO: Use float print if sizeof(double) == sizeof(float) should reduce code size
//             return printFloat(n, digits);
//         }

//         bool println(double n, std::uint8_t digits = 2) const {
//             return print(n, digits) && println();
//         }
//     };

//     template<class TStream>
//     constexpr TextWriter<StaticStream<TStream> > MakeTextWriter(){
//         return TextWriter<StaticStream<TStream> >(StaticStream<TStream>());
//     }

//     TextWriter<OutStream&> MakeTextWriter(OutStream& stream){
//         return TextWriter<OutStream&>(stream);
//     }

// }

// #endif