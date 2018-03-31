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

        

        template<class TActions>
        struct actions_executor{
            
            template <class TField>
            struct field_executor{
                using field_datatype_t = field_data_type<TField>;
                using field_masktype_t = field_mask_type<TField>;

                template<class TAction>
                using is_my_action = std::is_same<typename TAction::field_t, TField>;

                using my_actions = filter<TActions, bind<is_my_action, _1>>;

                template<class TActionType>
                using has_my_actions = any<my_actions, bind<is_action_of_type, _1, TActionType>>;

                static constexpr auto has_writes = has_my_actions<write_field>::value;
                static constexpr auto has_clears = has_my_actions<clear_field>::value;
                static constexpr auto has_sets = has_my_actions<set_field>::value;
                static constexpr auto has_toggles = has_my_actions<toggle_field>::value;

                template<std::size_t N>
                using action_n1_t = at_c<TActions, N - 1>;

                // return last write action value
                template<std::size_t N = size<TActions>::value,
                    bool found = is_my_action<action_n1_t<N>>::value && is_action_of_type<action_n1_t<N>, write_field>::value,
                    bool dummy = true>
                struct get_my_write{
                    static constexpr field_datatype_t get(TActions actions){
                        return mp::get<N - 1>(actions).value;
                    }
                };
                
                template<std::size_t N, bool dummy>
                struct get_my_write<N, false, dummy>
                {
                    static constexpr field_datatype_t get(TActions actions){
                        return get_my_write<N - 1>::get(actions);
                    }
                };

                template<bool dummy>
                struct get_my_write<1, false, dummy>
                {
                    static constexpr field_datatype_t get(TActions actions){
                        return field_datatype_t{};
                    }
                };

                template<typename TActionType, 
                    std::size_t N = size<TActions>::value,
                    bool found = is_my_action<action_n1_t<N>>::value && is_action_of_type<action_n1_t<N>, TActionType>::value>
                struct get_my_mask
                {
                    static constexpr field_masktype_t get(TActions actions){
                        return mp::get<N - 1>(actions).value | get_my_mask<TActionType, N - 1>::get(actions);
                    }
                };

                template<typename TActionType, std::size_t N> 
                struct get_my_mask<TActionType, N, false>
                {
                    static constexpr field_masktype_t get(TActions actions){
                        return get_my_mask<TActionType, N - 1>::get(actions);
                    }
                };

                template<typename TActionType>
                struct get_my_mask<TActionType, 1, false>
                {
                    static constexpr auto get(TActions actions){ return field_masktype_t{}; }
                };    

                template<typename TActionType>
                struct get_my_mask<TActionType, 1, true>
                {
                    static constexpr auto get(TActions actions){ return mp::get<0>(actions).value; }
                };    

                static void execute(TActions actions){                    
                    //check if we have write. If yes - than simply write value
                    if (has_writes){
                        // just write last write_field value
                        auto value = get_my_write<>::get(actions);
                        TField::write(value);
                        return;
                    }
                    // we would definitely need value now
                    auto value = TField::read();
                    
                    if (has_clears){
                        // clears here!
                        auto mask = get_my_mask<clear_field>::get(actions);
                        value &= ~mask;
                    } 
                    if (has_sets) {
                        // sets here!
                        auto mask = get_my_mask<set_field>::get(actions);
                        value |= mask;
                    }                     
                    if (has_toggles){
                        // toggles here!
                        auto mask = get_my_mask<toggle_field>::get(actions);
                        value ^= mask;
                    } 
                    if (has_clears || has_sets || has_toggles){
                        // we had some modifications
                        TField::write(value);
                    }
                }
            };

            template<class TFields>
            struct fields_iterator;

            // catch all
            template<class... TFields>
            struct fields_iterator<mp::const_list<TFields...>>{
                static void execute(TActions actions){}
            };

            // iterate
            template<class TField, class... TRestFields>
            struct fields_iterator<mp::const_list<TField, TRestFields...>>{
                static void execute(TActions actions){
                    field_executor<TField>::execute(actions);

                    fields_iterator<mp::const_list<TRestFields...>>::execute(actions);
                }
            };

            static void execute(TActions actions){
                // group by field
                using fields = no_duplicates<
                    transform<TActions, bind<get_action_field, _1>>>;

                // execute actions for field
                fields_iterator<fields>::execute(actions);
            }
        };        
    };


    template<class... T>
    void apply(T... actions){
        // flatten actions(TODO)
        auto flattenActions = mp::make_const_list(actions...);
        
        // execute
        details::actions_executor<decltype(flattenActions)>::execute(flattenActions);
    }
}

#endif