#ifndef FH_NULLFIELD_H_
#define FH_NULLFIELD_H_

#include "../std/std_types.hpp"
#include "fieldbit.hpp"

namespace fasthal{    
    struct null_field
	{
        static void write(std::uint8_t value) {}
        static constexpr std::uint8_t read() {return 0;}
	};

    static constexpr auto nullField = null_field{};
    static constexpr auto nullBit = fieldBit<0>(nullField);
}

#endif