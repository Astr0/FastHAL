#ifndef FH_INTERRUPTS_H_
#define FH_INTERRUPTS_H_

#include "../utils/types.hpp"
#include "../std/std_fake.hpp"

namespace fasthal{
    // default isr handlers
    namespace details{
        template <unsigned VNumber>
        struct isr_handler {
            static not_implemented handle();
        };

        template<unsigned VNumber>
        static constexpr bool has_isr = std::is_same_v<decltype(details::isr_handler<VNumber>::handle()), void>;
    }
    
    template<unsigned VNumber>
    inline void isr(){
        static_assert(details::has_isr<VNumber>, "No handler for ISR");
        details::isr_handler<VNumber>::handle();
    }
}

#endif