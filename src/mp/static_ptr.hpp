#ifndef FH_MP_STATIC_PTR_H_
#define FH_MP_STATIC_PTR_H_

#include "const_list.hpp"
#include "../std/std_types.hpp"

namespace fasthal::mp{
    template<typename T, unsigned VNum>
    struct static_ptr{
        using type = T;
        static /*constexpr*/ T _value;
        constexpr T& operator*()const {return _value;}
        constexpr T* operator->()const {return &_value;}
        constexpr operator T*()const {return &_value;}
    };

    namespace details{
        template<typename T, unsigned VNum>
        struct is_static_element_impl<static_ptr<T, VNum>>: std::true_type{};
    }

    #define FH_STATIC(NAME, VALUE) \
    using NAME ## _t = ::fasthal::mp::static_ptr<decltype(VALUE), __LINE__>;\
    template<> decltype(VALUE) NAME ## _t::_value = (VALUE);\
    static constexpr auto NAME = NAME ## _t{};
}

#endif