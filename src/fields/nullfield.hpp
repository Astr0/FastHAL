#ifndef FH_NULLFIELD_H_
#define FH_NULLFIELD_H_

#include "../mp/std_types.hpp"
#include "fieldbit.hpp"

namespace fasthal{    
    struct NullField
	{
        static void write(uint8_t value) {}
        static constexpr uint8_t read() {return 0;}
	};

    static constexpr auto nullField = NullField{};
    static constexpr auto nullBit = fieldBit<0>(nullField);
}

#endif