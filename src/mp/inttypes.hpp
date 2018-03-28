#ifndef FH_INTTYPES_H_
#define FH_INTTYPES_H_

#include <inttypes.h>

namespace fasthal{
    using size_t = decltype(sizeof(unsigned));

    using uint8_t = ::uint8_t;
};

#endif