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

        template<class TField>
        struct field_value{
            using field_t = TField;
            using datatype_t = field_data_type<TField>;

            constexpr field_value(datatype_t __value): value(__value){}

            const datatype_t value;
        };

        template <class... TActions>
        struct field_actions_list;
        
        template<class... TActions>
        using field_actions_list_t = field_actions_list<TActions...>;      

        template<class...TFieldValues>
        using field_action_results_t = mp::const_list<TFieldValues...>;

        
        template<class... TField>
        constexpr auto inline combine_action_results(field_value<TField>... fieldValues){
            return mp::make_const_list(fieldValues...);
        }

        struct write_field{
            template<typename T, typename V>
            static constexpr void execute(T& current, V value){ current = value; }
        };
        struct set_field{
            template<typename T, typename V>
            static constexpr void execute(T& current, V value){ current |= value; }
        };
        struct clear_field{
            template<typename T, typename V>
            static constexpr void execute(T& current, V value){ current &= ~value; }
        };
        struct toggle_field{
            template<typename T, typename V>
            static constexpr void execute(T& current, V value){ current ^= value; }
        };
        struct read_field{
            template<typename T, typename V>
            static constexpr void execute(T& current, V value) { }
        };
        struct clear_set_field{
            template<typename T, typename V>
            static constexpr void execute(T& current, V value1, V value2) { 
                clear_field::execute(current, value1);
                set_field::execute(current, value2);
            }
        };        

        template<class TAction>
        using get_action_field = typename TAction::field_t;

        template<class TAction, class TActionType>
        using is_action_of_type = std::is_same<typename TAction::action_t, TActionType>;

        // execute
        template<typename TValue, template<class> class TFilter>
        struct actions_executor
        {
            using this_t = actions_executor<TValue, TFilter>;

            template<typename... TIndex>
            struct tuple_exec{
                template<class TTuple>
                static constexpr void execute(TValue& value, TTuple tuple){
                    (this_t::execute(value, mp::get<TIndex::value>(tuple)), ...);
                }
            };

            template<class TAction>
            static constexpr void execute(TValue& value, TAction action){
                if (TFilter<TAction>::value)
                    action.execute(value);
            }

            template<class... TTuple>
            static constexpr void execute(TValue& value, field_actions_list_t<TTuple...> tuple){
                using tuple_indices_t = make_sequence<
                    brigand::size_t<0>, 
                    size<field_actions_list_t<TTuple...>>::value>;
                unpack<tuple_indices_t, tuple_exec>::execute(value, tuple);
            }
        };

        template<class... TActions>
        struct actions_apply{            
            // all actions flattened
            using all_actions_t = flatten<field_actions_list_t<TActions...>>;

            template <class TField>
            struct field_apply{
                using field_datatype_t = field_data_type<TField>;
                using field_masktype_t = field_mask_type<TField>;

                template<class TAction>
                using is_my_action = std::is_same<typename TAction::field_t, TField>;

                using my_actions_t = filter<all_actions_t, bind<is_my_action, _1>>;
                static constexpr bool has_writes = any<my_actions_t, bind<is_action_of_type, _1, write_field>>::value;
                static constexpr bool has_modifies = !all<my_actions_t, bind<is_action_of_type, _1, read_field>>::value;

                static constexpr inline field_value<TField> execute(TActions... actions){
                    auto value = has_writes ? field_datatype_t{} : TField::read();
                    
                    using executor_t = actions_executor<field_datatype_t, is_my_action>;
                    (executor_t::execute(value, actions), ...);

                    if (has_modifies)
                        TField::write(value);

                    return field_value<TField> { value };
                }
            };

            // iterate through fields
            template<class... TField>
            struct fields_iterator{
                static constexpr inline auto execute(TActions... actions){
                    return combine_action_results(field_apply<TField>::execute(actions...)...);
                }
            };

            // // one field iterator - just return field's value
            // template<class TField>
            // struct fields_iterator<TField>{
            //     static constexpr inline auto execute(TActions... actions){
            //         return field_apply<TField>::execute(actions...).value;
            //     }
            // };

            static constexpr inline auto apply(TActions... actions){
                // group by field
                using fields_t = no_duplicates<
                    transform<all_actions_t, bind<get_action_field, _1>>>;

                // execute actions for field            
                using fields_iterator_t = unpack<fields_t, fields_iterator>;    
                return fields_iterator_t::execute(actions...);
            }
        };        
    };


    template<class... T>
    constexpr inline auto apply(const T... actions){
        // execute
        return details::actions_apply<T...>::apply(actions...);
    }

    namespace details{
        template<class... TActions>
        struct field_actions_list: mp::const_list<TActions...>
        {
            constexpr field_actions_list(TActions... args): mp::const_list<TActions...>{args...} {}

            constexpr auto operator()(){
                return apply(*this);
            }
        };
    }
}

#endif