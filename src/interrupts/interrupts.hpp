#ifndef FH_INTERRUPTS_H_
#define FH_INTERRUPTS_H_

#include "../utils/types.hpp"
#include "../std/std_fake.hpp"

namespace fasthal{
    // default isr handlers
    namespace details{
        template <unsigned VNumber>
        struct default_isr {
            static not_implemented handle();
        };

        template<unsigned VNumber>
        static constexpr bool has_defaut_isr = std::is_same_v<decltype(details::default_isr<VNumber>::handle()), void>;
    }
    
    template<unsigned VNumber>
    inline void isr(){
        static_assert(details::has_defaut_isr<VNumber>, "No default handler for ISR");
        details::default_isr<VNumber>::handle();
    }
}

#endif