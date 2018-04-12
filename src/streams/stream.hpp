#ifndef FH_STREAM_H_
#define FH_STREAM_H_

#include "../std/type_traits.hpp"
#include "../std/std_fake.hpp"

namespace fasthal{
    namespace details{
        template<class T>
        struct is_ostream_impl: std::false_type{};

        template<class T>
        using is_ostream = is_ostream_impl<std::base_type_t<T>>;

        template<class T>
        using enable_if_ostream = std::enable_if_c<is_ostream<T>::value>;
    }

    enum class has_byte_type: std::uint8_t{
        ok = 0,
        last = 1,
        none = 2
    };

    struct has_byte {        
        constexpr has_byte(std::uint8_t __byte, has_byte_type __type): byte(__byte), type(__type) { }        

        std::uint8_t byte;
        has_byte_type type;
    };
    static constexpr auto has_byte_none = has_byte{0, has_byte_type::none};
}

#endif