#ifndef FH_FIELDS_EXECUTE_H_
#define FH_FIELDS_EXECUTE_H_

#include "../mp/brigand.hpp"

namespace fasthal{
namespace details{
template <class TField, class TActions>
        struct get_action_masks{
            
        };

        template <class TField, class TActions>
        // execute actions on specified field
        struct execute_field_actions{
            static void execute(){
                auto value = TField::read();
                
                auto toggleMask = 1;
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

        template<class TField, class... TFields, class... TActions>
        struct execute_fields_actions<brigand::list<TField, TFields...>, brigand::list<TActions...>>
        {
            static void execute(TActions... actions){
                using ActionsList = brigand::list<TActions...>;

                // execute for field
                execute_field_actions<TField, ActionsList>::execute();

                // execute rest
                execute_fields_actions<brigand::list<TFields...>, ActionsList>::execute(actions...);
            }
        };
};
};

#endif