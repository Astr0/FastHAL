#pragma once

#ifndef UTILS_FUNCTIONS_H_
#define UTILS_FUNCTIONS_H_

// TODO: remove
#define fh_cbi(reg, bit) (reg &= ~(1 << bit))
#define fh_sbi(reg, bit) (reg |= (1 << bit))
#define fh_wbi(reg, bit, v) (v ? fh_sbi(reg, bit) : fh_cbi(reg, bit)) 


#define FH_WRAPVARIABLE(CLASSNAME, VARNAME) struct CLASSNAME{ static decltype(VARNAME)& value(){return (VARNAME);} };

#define FH_BITENUM_OPS(NAME, BASE)\
inline constexpr NAME operator|(NAME x, NAME y){ return static_cast<NAME>(static_cast<BASE>(x) | static_cast<BASE>(y)); }\
inline constexpr NAME operator&(NAME x, NAME y){ return static_cast<NAME>(static_cast<BASE>(x) & static_cast<BASE>(y)); }

#define FH_JOIN_(x, y) x ## y
#define FH_JOIN(x, y) FH_JOIN_(x, y)

namespace fasthal{
    template<int val>
    int constInt() {return val; }

    namespace details{
        template<typename T, typename F>
        struct alias_cast_impl
        {
            constexpr alias_cast_impl(F __raw): raw(__raw) { }
            union
            {
                F raw;
                T data;
            };
        };
    }
    
    template<typename T, typename F>
    constexpr inline T alias_cast(F raw_data)
    {
        static_assert(sizeof(T) == sizeof(F), "Cannot cast types of different sizes");
        auto ac = details::alias_cast_impl<T, F>{raw_data};
        //ac.raw = raw_data;
        return ac.data;
    }

    template<typename T, typename... TValues>
    static inline bool is_any(T test, TValues... values){
        return (...|| (test == values));
    }

}

#endif