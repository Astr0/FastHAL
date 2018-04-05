#pragma once

#ifndef UTILS_FUNCTIONS_H_
#define UTILS_FUNCTIONS_H_

#define fh_cbi(reg, bit) (reg &= ~(1 << bit))
#define fh_sbi(reg, bit) (reg |= (1 << bit))
#define fh_wbi(reg, bit, v) (v ? fh_sbi(reg, bit) : fh_cbi(reg, bit)) 


#define FH_WRAPVARIABLE(CLASSNAME, VARNAME)\
struct CLASSNAME\
{\
static decltype(VARNAME)& value(){return VARNAME;}\
};

#define FH_BITENUM_OPS(NAME, BASE)\
inline constexpr NAME operator|(NAME x, NAME y){ return static_cast<NAME>(static_cast<BASE>(x) | static_cast<BASE>(y)); }\
inline constexpr NAME operator&(NAME x, NAME y){ return static_cast<NAME>(static_cast<BASE>(x) & static_cast<BASE>(y)); }

#define FH_FIELDBIT_ENABLE_ACTIONS(PARAM_T, PARAM, BIT)\
constexpr auto enable(PARAM_T PARAM) { return set(BIT); }\
void enable_(PARAM_T PARAM){ apply(enable(PARAM)); }\
constexpr auto disable(PARAM_T PARAM){ return clear(BIT); }\
void disable_(PARAM_T PARAM){apply(disable(PARAM));}\
constexpr auto enabled_(PARAM_T PARAM){ return read_(BIT); }\



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