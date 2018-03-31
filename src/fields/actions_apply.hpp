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

        // execute
        template<typename TValue, template<class> class TFilter>
        struct actions_executor
        {
            template<typename TTuple, std::size_t I = 0, std::size_t N = size<TTuple>::value>
            struct unwrapper{
                static constexpr void execute(TValue& value, TTuple tuple){
                    actions_executor<TValue, TFilter>::execute(value, mp::get<I>(tuple));
                    return unwrapper<TTuple, I+1>::execute(value, tuple);
                }
            };

            template<typename TTuple, std::size_t N>
            struct unwrapper<TTuple, N, N>
            {
                static constexpr void execute(TValue& value, TTuple tuple){                    
                }
            };

            template<class TAction>
            static constexpr void execute(TValue& value, TAction action){
                if (TFilter<TAction>::value)
                    value = action.execute(value);
            }

            template<class... TTuple>
            static constexpr void execute(TValue& value, mp::const_list<TTuple...> tuple){
                unwrapper<mp::const_list<TTuple...>>::execute(value, tuple);
            }
        };

        template<class... TActions>
        struct actions_apply{            
            // all actions flattened
            using all_actions_t = flatten<mp::const_list<TActions...>>;

            template <class TField>
            struct field_apply{
                using field_datatype_t = field_data_type<TField>;
                using field_masktype_t = field_mask_type<TField>;

                template<class TAction>
                using is_my_action = std::is_same<typename TAction::field_t, TField>;

                using my_actions_t = filter<all_actions_t, bind<is_my_action, _1>>;
                static constexpr bool has_writes = any<my_actions_t, bind<is_action_of_type, _1, write_field>>::value;

                static constexpr inline void execute(TActions... actions){
                    auto value = has_writes ? field_datatype_t{} : TField::read();
                    
                    using executor_t = actions_executor<field_datatype_t, is_my_action>;
                    (executor_t::execute(value, actions), ...);

                    TField::write(value);
                }
            };

            // iterate through fields
            template<class... TField>
            struct fields_iterator{
                static inline void execute(TActions... actions){
                    (field_apply<TField>::execute(actions...), ...);
                }
            };

            static inline void apply(TActions... actions){
                // group by field
                using fields_t = no_duplicates<
                    transform<all_actions_t, bind<get_action_field, _1>>>;

                // execute actions for field            
                using fields_iterator_t = unpack<fields_t, fields_iterator>;    
                fields_iterator_t::execute(actions...);
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