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

        template<class T>
        struct is_istream_impl: std::false_type{};

        template<class T>
        using is_istream = is_istream_impl<std::base_type_t<T>>;

        template<class T>
        using enable_if_istream = std::enable_if_c<is_istream<T>::value>;        
    }
}

#endif