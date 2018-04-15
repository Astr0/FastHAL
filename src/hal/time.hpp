#ifndef FH_TIME_H_
#define FH_TIME_H_

#include "../std/std_types.hpp"

namespace fasthal{
    using time_t = std::uint32_t;

    namespace details{
        template<bool VImplemented = true>
        struct global_time_impl;
    }

    template<class TTime = details::global_time_impl<>>
    inline time_t time_ms(){ return TTime::ms(); }
	template<class TTime = details::global_time_impl<>>
    inline time_t time_us(){ return TTime::us(); }
	template<class TTime = details::global_time_impl<>>
    inline void delay_us(std::uint16_t us){ TTime::delay_us(us);}
    template<class TTime = details::global_time_impl<>>
    inline void delay_ms(time_t ms){ TTime::delay_ms(ms);}
    template<class TTime = details::global_time_impl<>>
    constexpr inline auto global_time() { return TTime{}; }
}

#define FH_USE_TIME(TIMER)\
namespace fasthal::details{\
    template<>\
    struct global_time_impl<true>{\
        static inline time_t ms() { return TIMER.ms(); }\
        static inline time_t us() { return TIMER.us(); }\
        static inline void delay_us(std::uint16_t us) { TIMER.delay_us(us); }\
        static inline void delay_ms(time_t ms) { TIMER.delay_ms(ms); }\
    };\
}

#endif