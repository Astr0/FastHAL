#ifndef FH_AVR_PSTRING_H_
#define FH_AVR_PSTRING_H_

#include "../../streams/stream.hpp"
#include "../../streams/ostream_print.hpp"
#include "../../streams/ostream_write.hpp"
#include <avr/pgmspace.h>

namespace fasthal{
    namespace details{
        class flash_str;

        template<class T, details::enable_if_ostream<T> dummy = nullptr>
        void write_data_only(T ostream, const flash_str* text) {
            auto p = reinterpret_cast<PGM_P>(text);
            char c;
            while ((c = pgm_read_byte(p++)) != '\0')
                write(ostream, c);
        }
    }

    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void write(T ostream, const details::flash_str* str) {
        details::write_data_only(ostream, str);
        write(ostream, '\0');
    }

    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void print(T ostream, const details::flash_str* str) {
        details::write_data_only(ostream, str);
    }

    template<class T, details::enable_if_ostream<T> dummy = nullptr>
    void println(T ostream, const details::flash_str* str) {
        print(ostream, str);
        println(ostream);
    }
}

#define FH_FLASH(string_literal) (reinterpret_cast<const ::fasthal::details::flash_str *>(PSTR(string_literal)))

#endif