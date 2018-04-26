#ifndef FH_MP_STATIC_PTR_H_
#define FH_MP_STATIC_PTR_H_

#include "const_list.hpp"
#include "../std/std_types.hpp"
#include "../utils/functions.h"

namespace fasthal::mp{
    template<auto* VPtr>
    struct static_ptr{
        using type_t = typename std::remove_reference<decltype(*VPtr)>::type;
        constexpr static_ptr(){}

        constexpr type_t& get()const { return *VPtr; }

        constexpr type_t& operator*()const {return get();}
        constexpr type_t* operator->()const {return &get();}
        // auto gives segmentation fault here..
        constexpr operator type_t*()const {return &get();}
    };

    namespace details{
        template<auto* VPtr>
        struct is_static_impl<static_ptr<VPtr>>: std::true_type{};
    }

    // static ptr
    #define FH_SPTR(VALUE) (::fasthal::mp::static_ptr<&VALUE>{})
}

#endif