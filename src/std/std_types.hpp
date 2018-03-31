#ifndef STD_TYPES_H_
#define STD_TYPES_H_

#include <inttypes.h>

namespace std{

using uint8_t = ::uint8_t;
using uint16_t = ::uint16_t;
using uint32_t = ::uint32_t;
using uint64_t = ::uint64_t;

using int8_t = ::int8_t;
using int16_t = ::int16_t;
using int32_t = ::int32_t;
using int64_t = ::int64_t;

using size_t = decltype(sizeof(unsigned));
using ptrdiff_t = decltype((char*)0 - (char*)0);
using nullptr_t = decltype(nullptr);

};
#endif