#ifndef FH_INTERRUPTS_H_
#define FH_INTERRUPTS_H_

#include "../utils/types.hpp"
#include "../std/type_traits.hpp"

namespace fasthal{
    // default isr handlers
    namespace details{
        template <unsigned VNumber>
        struct default_isr {
            using not_handler_t = char; //anything, but void
            inline static not_handler_t handle();
        };
    }
    
    template<unsigned VNumber>
    inline void isr(){
        static_assert(std::is_same<decltype(details::default_isr<VNumber>::handle()), void>::value, "No default handler for ISR");
        details::default_isr<VNumber>::handle();
    }
}

#endif