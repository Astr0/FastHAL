#ifndef FH_FIELDS_EXECUTE_H_
#define FH_FIELDS_EXECUTE_H_

#include "../mp/brigand.hpp"
#include "fields.hpp"
#include "actions.hpp"

namespace fasthal{
namespace details{
        template <class TField>
        struct execute_field_actions{
            using DataType = typename get_field_datatype<TField>::type;

            template <class TAction>
            struct get_action_masks{
            };

            template <class... TActions>
            // lots of actions
            static constexpr decltype(auto) getToggleMask(){
                return 1;
            }

            // execute actions on this field
            template <class... TActions>
            static void execute(TActions... actions){
                auto value = TField::read();
                
                constexpr auto toggleMask = getToggleMask<TActions...>();
                value ^= toggleMask;

                TField::write(value);
            }
        };

        template<class TFields, class TActions>
        // terminator for empty fields
        struct execute_fields_actions{
            template <class... T>
            static void execute(T... actions){                
            }
         };

        // for each fields execute all actions on this field
        template<class TField, class... TFields, class... TActions>
        struct execute_fields_actions<brigand::list<TField, TFields...>, brigand::list<TActions...>>
        {
            static void execute(TActions... actions){
                using ActionsList = brigand::list<TActions...>;

                // execute for field
                execute_field_actions<TField>::execute(actions...);

                // execute rest
                execute_fields_actions<brigand::list<TFields...>, ActionsList>::execute(actions...);
            }
        };
};
};

#endif