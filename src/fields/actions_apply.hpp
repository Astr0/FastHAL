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

        // empty
        template<typename TValue, template<class> class TFilter, class... TActions>
        struct actions_executor
        {
            static constexpr TValue execute(TValue value, TActions... actions){
                return value;
            }
        };

        // normal
        template<typename TValue, template<class> class TFilter, class TAction, class... TRest>
        struct actions_executor<TValue, TFilter, TAction, TRest...>
        {
            static constexpr TValue execute(TValue value, TAction action, TRest... rest){
                using next_t = actions_executor<TValue, TFilter, TRest...>;
                return next_t::execute(TFilter<TAction>::value ? action.execute(value) : value, rest...);
            }
        };

        // tuple...
        template<typename TValue, template<class> class TFilter, class... TTuple, class... TRest>
        struct actions_executor<TValue, TFilter, mp::const_list<TTuple...>, TRest...>
        {
            template<std::size_t I = 0, std::size_t N = sizeof...(TTuple)>
            struct unwrapper{
                static constexpr TValue execute(TValue value, mp::const_list<TTuple...> tuple){
                    return unwrapper<I+1>::execute(
                            actions_executor<TValue, TFilter, at_c<mp::const_list<TTuple...>, I>>::execute(value, mp::get<I>(tuple)),
                            tuple);
                    // collect args
                    //return unwrapper<I+1>::execute(value, tuple, args..., mp::get<I>(tuple));
                }
            };

            template<std::size_t N>
            struct unwrapper<N, N>
            {
                static constexpr TValue execute(TValue value, mp::const_list<TTuple...> tuple){
                    // all args are here 
                    return value;
                    //return actions_executor<TValue, TFilter, TArgs...>::execute(value, args...);
                }
            };

            static constexpr TValue execute(TValue value, mp::const_list<TTuple...> tuple, TRest... rest){
                // ignore tuple for now (check if compiles)
                using next_t = actions_executor<TValue, TFilter, TRest...>;
                return next_t::execute(unwrapper<>::execute(value, tuple), rest...);
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

            // catch all
            template<class... TFields>
            struct fields_iterator{
                static inline void execute(TActions... actions){}
            };

            // iterate
            template<class TField, class... TRestFields>
            struct fields_iterator<TField, TRestFields...>{
                static inline void execute(TActions... actions){
                    field_apply<TField>::execute(actions...);

                    fields_iterator<TRestFields...>::execute(actions...);
                }
            };

            static inline void apply(TActions... actions){
                using all_actions = flatten<mp::const_list<TActions...>>;

                // group by field
                using fields_t = no_duplicates<
                    transform<all_actions, bind<get_action_field, _1>>>;

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