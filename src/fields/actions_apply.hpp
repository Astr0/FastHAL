#ifndef FH_ACTIONSAPPLY_H_
#define FH_ACTIONSAPPLY_H_

#include "actions.hpp"
#include "info.hpp"
#include "../std/type_traits.hpp"
#include "../mp/brigand_ex.hpp"
#include "../mp/const_list.hpp"

namespace fasthal{
    namespace details{
        template<class TField>
        struct field_value{
            using field_t = TField;
            using datatype_t = field_data_type<TField>;

            constexpr field_value(datatype_t __value): value(__value){}

            const datatype_t value;
        };

        template<class TFieldValue>
        using get_fieldvalue_field = typename TFieldValue::field_t;

        template<class... TActions>
        using field_actions_list_t = mp::const_list<TActions...>;      

        template<class...TField>
        using field_action_results_t = mp::const_list<field_value<TField>...>;

        
        template<class... TField>
        constexpr auto inline combine_action_results(field_value<TField>... fieldValues){
            return mp::make_const_list(fieldValues...);
        }

        struct write_field{
            template<typename T, typename V>
            static constexpr inline void execute(T& current, V value){ current = T{ value }; }
        };
        struct set_field{
            template<typename T, typename V>
            static constexpr inline void execute(T& current, V value){ current |= value; }
        };
        struct clear_field{
            template<typename T, typename V>
            static constexpr inline void execute(T& current, V value){ current &= ~value; }
        };
        struct toggle_field{
            template<typename T, typename V>
            static constexpr inline void execute(T& current, V value){ current ^= value; }
        };
        struct read_field{
            template<typename T>
            static constexpr inline void execute(T& current) { }
        };
        struct clear_set_field{
            template<typename T, typename TClear, typename TSet>
            static constexpr inline void execute(T& current, TClear clear, TSet set) { 
                clear_field::execute(current, clear);
                set_field::execute(current, set);
            }
        };        

        template<class TAction>
        using get_action_field = typename TAction::field_t;

        template<class TAction, class TActionType>
        using is_action_of_type = std::is_same<typename TAction::action_t, TActionType>;

        // execute action
        template<typename TValue, template<class> class TFilter, typename TAction>
        struct actions_executor
        {
            static constexpr inline void execute(TValue& value, TAction action){
                // dont even try to build with wrong action types
                if constexpr(TFilter<TAction>::value)
                    action.execute(value);
            }
        };

        template<typename TValue, template<class> class TFilter, class... TActions>
        struct actions_executor<TValue, TFilter, field_actions_list_t<TActions...>>
        {
            using list_t = field_actions_list_t<TActions...>;
            template<typename... TIndex>
            struct list_exec{
                static constexpr inline void execute(TValue& value, list_t tuple){
                    (
                        (actions_executor<TValue, TFilter, brigand::at_c<list_t, TIndex::value>>
                         ::execute(value, mp::get<TIndex::value>(tuple)))
                    , ...);
                }
            };

            static constexpr inline void execute(TValue& value, list_t tuple){
                using indices_t = brigand::make_sequence<
                    brigand::size_t<0>, 
                    brigand::size<list_t>::value>;
                brigand::unpack<indices_t, list_exec>::execute(value, tuple);
            }
        };

        template<class... TActions>
        struct actions_apply{            
            // all actions flattened
            using all_actions_t = brigand::flatten<field_actions_list_t<TActions...>>;

            template <class TField>
            struct field_apply{
                using field_datatype_t = field_data_type<TField>;
                using field_masktype_t = field_mask_type<TField>;

                template<class TAction>
                using is_my_action = std::is_same<typename TAction::field_t, TField>;

                using my_actions_t = brigand::filter<all_actions_t, brigand::bind<is_my_action, brigand::_1>>;
                static constexpr bool has_writes = brigand::any<my_actions_t, brigand::bind<is_action_of_type, brigand::_1, write_field>>::value;
                static constexpr bool has_modifies = !brigand::all<my_actions_t, brigand::bind<is_action_of_type, brigand::_1, read_field>>::value;

                template<class TAction>
                static constexpr inline void execute(field_datatype_t& value, TAction action){
                    using executor_t = actions_executor<field_datatype_t, is_my_action, TAction>;
                    executor_t::execute(value, action);
                }

                static constexpr inline field_value<TField> execute(TActions... actions){
                    auto value = has_writes ? field_datatype_t{} : TField::read();
                    
                    // TCNT1 = (std::uint16_t)(TField::address());

                    (execute(value, actions), ...);

                    if (has_modifies)
                        TField::write(value);

                    return field_value<TField> { value };
                }
            };

            // iterate through fields
            template<class... TField>
            struct fields_iterator{
                template<typename... TFieldV>
                static constexpr inline auto execute(TActions... actions, TFieldV... fieldV){
                    return combine_action_results(fieldV...);
                }
            };
            template<class TField, class... TFields>
            struct fields_iterator<TField, TFields...>{
                template<typename... TFieldV>
                static constexpr inline auto execute(TActions... actions, TFieldV... fieldV){
                    auto field_v = field_apply<TField>::execute(actions...);
                    return fields_iterator<TFields...>::execute(actions..., fieldV..., field_v);
                }
            };

            static constexpr inline auto apply(TActions... actions){
                // group by field
                using fields_t = brigand::no_duplicates<
                    brigand::transform<all_actions_t, brigand::bind<get_action_field, brigand::_1>>>;

                // execute actions for field            
                using fields_iterator_t = brigand::unpack<fields_t, fields_iterator>;    
                return fields_iterator_t::execute(actions...);
            }
        };        
    };


    template<class... T>
    constexpr inline auto apply(const T... actions){
        // execute
        return details::actions_apply<T...>::apply(actions...);
    }
}

#endif