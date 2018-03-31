#ifndef FH_NULLFIELD_H_
#define FH_NULLFIELD_H_

#include "../std/std_types.hpp"
#include "../std/type_traits.hpp"
#include "info.hpp"
#include "fieldbit.hpp"

namespace fasthal{    
    struct null_field
	{
        static void write(std::uint8_t value) {}
        static constexpr std::uint8_t read() {return 0;}
	};

    namespace details{
        template<>
        struct is_field_impl<null_field>: std::true_type {};
    }

    static constexpr auto nullField = null_field{};
    static constexpr auto nullBit = fieldBit<0>(nullField);
}

#endif