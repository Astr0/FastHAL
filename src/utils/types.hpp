#ifndef FH_TYPES_H_
#define FH_TYPES_H_

#include "../std/type_traits.hpp"
#include "../std/std_types.hpp"

namespace fasthal{
    using std::integral_constant;

    template<std::uint8_t V>
    static constexpr auto uint8_v = integral_constant<std::uint8_t, V>{};
    template<std::uint16_t V>
    static constexpr auto uint16_v = integral_constant<std::uint16_t, V>{};
    template<std::uint32_t V>
    static constexpr auto uint32_v = integral_constant<std::uint32_t, V>{};

    // using brigand::size_t;
    // using brigand::uint8_t;
    // using brigand::uint16_t;
    // using brigand::uint32_t;
    // using brigand::uint64_t;
}
#endif