#ifndef FH_ACTIONSAPPLY_H_
#define FH_ACTIONSAPPLY_H_

#include "actions_ex.hpp"
#include "info.hpp"
#include "../std/type_traits.hpp"
#include "../mp/brigand_ex.hpp"
#include "../mp/const_list.hpp"

namespace fasthal{
    namespace details{
        using namespace brigand;

        template<class TAction>
        using get_action_field = typename TAction::field_t;

        template<class TAction, class TActionType>
        using is_action_of_type = std::is_same<typename TAction::action_t, TActionType>;

        template<typename TValue, template<class> class TFilter, class... TActions>
        struct actions_executor
        {
            static constexpr TValue execute(TValue value, TActions... actions){
                return value;
            }
        };

        template<typename TValue, template<class> class TFilter, class TAction, class... TRest>
        struct actions_executor<TValue, TFilter, TAction, TRest...>
        {
            static constexpr TValue execute(TValue value, TAction action, TRest... rest){
                using next_t = actions_executor<TValue, TFilter, TRest...>;
                return next_t::execute(TFilter<TAction>::value ? action.execute(value) : value, rest...);
            }
        };

        template<class... TActions>
        struct actions_apply{            
            template <class TField>
            struct field_apply{
                using field_datatype_t = field_data_type<TField>;
                using field_masktype_t = field_mask_type<TField>;

                template<class TAction>
                using is_my_action = std::is_same<typename TAction::field_t, TField>;

                static constexpr inline void execute(TActions... actions){
                    auto value = TField::read();
                    
                    value = actions_executor<field_datatype_t, is_my_action, TActions...>::execute(value, actions...);

                    TField::write(value);
                }
            };

            template<class TFields>
            struct fields_iterator;

            // catch all
            template<class... TFields>
            struct fields_iterator<list<TFields...>>{
                static inline void execute(TActions... actions){}
            };

            // iterate
            template<class TField, class... TRestFields>
            struct fields_iterator<list<TField, TRestFields...>>{
                static inline void execute(TActions... actions){
                    field_apply<TField>::execute(actions...);

                    fields_iterator<list<TRestFields...>>::execute(actions...);
                }
            };

            static inline void apply(TActions... actions){
                // group by field
                using fields_t = no_duplicates<
                    transform<list<TActions...>, bind<get_action_field, _1>>>;

                // execute actions for field                
                fields_iterator<fields_t>::execute(actions...);
            }
        };        
    };


    template<class... T>
    inline void apply(const T... actions){
        // flatten actions(TODO)
        //auto flattenActions = mp::make_const_list(actions...);
        
        // execute
        details::actions_apply<T...>::apply(actions...);
    }
}

#endif