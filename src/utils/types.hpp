#ifndef FH_TYPES_H_
#define FH_TYPES_H_

#include "../std/type_traits.hpp"
#include "../std/std_types.hpp"

namespace fasthal{
    using std::integral_constant;

    #define FH_CONST(x) (integral_constant<decltype(x), x>{})
    
    // buffer size
    using bsize_t = std::uint8_t;

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

    class buffer_view{
        std::uint8_t* _buf;
        bsize_t _len;
    public:
        constexpr buffer_view(std::uint8_t* buf, bsize_t len): _buf(buf), _len(len) { }

        constexpr auto empty(){ return _len == 0; }
        auto next(){ --_len; return *_buf++; }
    };

    // used somewhere
    namespace details{
        struct not_implemented{};
    }
}
#endif