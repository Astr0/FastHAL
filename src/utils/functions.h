#pragma once

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

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
}

#endif