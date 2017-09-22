#pragma once

#ifndef UTILS_FUNCTIONS_H_
#define UTILS_FUNCTIONS_H_

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define FASTHAL_WRAPVARIABLE(CLASSNAME, VARNAME)\
struct CLASSNAME\
{\
static decltype(VARNAME)& value(){return VARNAME;}\
};

namespace fasthal{
    template<int val>
    int constInt() {return val; }

    template<typename T, typename F>
    struct alias_cast_t
    {
        union
        {
            F raw;
            T data;
        };
    };
    
    template<typename T, typename F>
    T alias_cast(F raw_data)
    {
        static_assert(sizeof(T) == sizeof(F), "Cannot cast types of different sizes");
        alias_cast_t<T, F> ac;
        ac.raw = raw_data;
        return ac.data;
    }
}

#endif