#pragma once

#ifndef UTILS_TEXTWRITER_H_
#define UTILS_TEXTWRITER_H_

namespace fasthal{
    namespace priv{
        template<uint8_t TSize>
        struct PrintBufferSize{
            const static uint8_t decSize = 2 * TSize + 3;
            const static uint8_t binSize = 8 * TSize + 1;
        };

        template<>
        struct PrintBufferSize<4>{
            const static uint8_t decSize = 11;
            const static uint8_t binSize = 4 * 8 + 1;
        };

        template<>
        struct PrintBufferSize<2>{
            const static uint8_t decSize = 6;
            const static uint8_t binSize = 2 * 8 +1;
        };

        template<>
        struct PrintBufferSize<1>{
            const static uint8_t decSize = 4;
            const static uint8_t binSize = 8 + 1;
        };
    }


    template<class TStream>
    class TextWriter{
    private:
        TStream& _stream;

        // template <uint8_t size>
        // constexpr uint8_t printBufferSize(uint8_t size){
        //     switch(size)
        //     {
        //         case 4:
        //             return 11;
        //         case 2:
        //             return 6;
        //         case 1: 
        //             return 4;
        //         default:
        //             return size * 2 + 3;
        //     }
        // }
        
        template<typename T>
        bool printNumber(T n){
            char buf[priv::PrintBufferSize<sizeof(T)>::decSize]; // Assumes 8-bit chars plus zero byte.
            char *str = &buf[sizeof(buf) - 1];
          
            *str = '\0';

            do {
                char c = n % 10;
                n /= 10;
                *--str = c + '0';
            } while(n); 
            return print(str);
        }

        template<typename T>
        bool printNumber(T n, uint8_t base){
            char buf[priv::PrintBufferSize<sizeof(T)>::binSize]; // Assumes 8-bit chars plus zero byte.
            char *str = &buf[sizeof(buf) - 1];
          
            *str = '\0';

            do {
                char c = n % base;
                n /= base;
                *--str = c < 10 ? c + '0' : c + 'A' - 10;
            } while(n); 
            return print(str);
        }
    public:
        TextWriter(TStream& stream): _stream(stream){

        }

        bool write(const uint8_t byte){
            return _stream.write(byte);
        }

        bool write(const uint8_t *buffer, size_t size)
        {
          while (size--) {
            if (!write(*buffer++))
                return false;
          }
          return true;
        }

        bool println(){
            return write('\r') && write('\n');
        }

        bool print(const char* text){
            char c;
            while ((c = *text++) != '\0'){
                if (!write(c))
                    return false;
            }
            return true;
        }

        bool println(const char* text){
            return print(text) && println();
        }

        bool print(uint8_t n){
            return printNumber(n);
        }

        bool println(uint8_t n){
            return printNumber(n) && println();
        }

        bool print(uint8_t n, uint8_t base){
            return printNumber(n, base);
        }

        bool println(uint8_t n, uint8_t base){
            return printNumber(n, base) && println();
        }
        
    };
}

#define FASTHAL_TEXTWRITER(Stream, Name) ::fasthal::TextWriter<decltype(Stream)> Name(Stream);

#endif