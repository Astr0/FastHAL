#ifndef FH_MP_STATIC_PTR_H_
#define FH_MP_STATIC_PTR_H_

#include "const_list.hpp"
#include "../std/std_types.hpp"
#include "../utils/functions.h"

namespace fasthal::mp{
    template<class TGet>
    struct static_ptr{
        constexpr auto& operator*()const {return TGet::value();}
        constexpr auto* operator->()const {return &TGet::value();}
        constexpr operator auto*()const {return &TGet::value();}
    };

    namespace details{
        template<class TGet>
        struct is_static_impl<static_ptr<TGet>>: std::true_type{};
    }

    #define FH_STATIC_PTR(NAME, VALUE)\
    FH_WRAPVARIABLE(__get ## NAME, VALUE);\
    using NAME = ::fasthal::mp::static_ptr<__get ## NAME>;
}

#endif