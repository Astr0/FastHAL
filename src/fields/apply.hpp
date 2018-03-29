#ifndef FH_APPLY_H_
#define FH_APPLY_H_

#include "../mp/brigand_ex.hpp"
#include "actions.hpp"
#include "execute.hpp"

namespace fasthal{
    namespace details{
        template<class TAction>
        struct select_action_bit_field{
            using type = typename TAction::BitField;
        };

        template<class TBitField>
        struct select_bit_field_field_bits{
            using type = typename TBitField::FieldBits;
        };

        template<class TFieldBit>
        struct select_field_bit_field{
            using type = typename TFieldBit::Field;
        };        
    };

    template<typename ...TActions>
    void apply(const TActions... actions){
        // generic algorithm:
        // get distinct Fields with Actions that touch that field
        // for each field calculate what we need to clear, set and toggle and read
        // execute everything 
        // ?
        // return result; // profit
        // make list from actions
        using ActionsList = brigand::list<TActions...>;
        // select list<fieldbit> for each action
        using ActionFieldBits = brigand::transform<
                brigand::transform<
                    ActionsList, 
                    details::select_action_bit_field<brigand::_1>>, 
                details::select_bit_field_field_bits<brigand::_1>>;
        // list<list<fieldbit>> to list<fieldbit>
        using FieldBits = brigand::flatten<ActionFieldBits>;
        // make distinct list<fields>
        using Fields = brigand::no_duplicates<brigand::transform<FieldBits, details::select_field_bit_field<brigand::_1>>>;
        // concat actions to fields
        //using FieldActions = brigand::transform<Fields, typename details::make_field_actions<ActionsList>::template impl<brigand::_1>>;

        details::execute_fields_actions<Fields, ActionsList>::execute(actions...);
        // execute actions for separate port
        // brigand::for_each<FieldActions>(details::execute_actions{});
    }
};

#endif