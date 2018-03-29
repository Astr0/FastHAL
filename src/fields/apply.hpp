#ifndef FH_APPLY_H_
#define FH_APPLY_H_

#include "../mp/brigand.hpp"
#include "actions.hpp"

namespace fasthal{
    namespace details{
        template<class TAction>
        struct select_action_field_bits{
            using type = typename TAction::FieldBits;
        };

        template<class TFieldBit>
        struct select_field_bit_field{
            using type = typename TFieldBit::Field;
        };
    };

    template<typename ...TActions>
    void apply(TActions... actions){
        // generic algorithm:
        // get distinct Fields with Actions that touch that field
        // for each field calculate what we need to clear, set and toggle and read
        // execute everything 
        // ?
        // return result; // profit
        using ActionsList = brigand::list<TActions...>;
        // TODO
        // using ActionFieldBits = mp::select_t<ActionsList, details::select_action_field_bits>;
        // using FieldBits = mp::flatten_t<ActionFieldBits>;        
        // using Fields = mp::distinct_t<mp::select_t<FieldBits, details::select_field_bit_field>>;
        
    }
};

#endif