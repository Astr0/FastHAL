#ifndef FH_FIELDACTIONSEX_H_
#define FH_FIELDACTIONSEX_H_

#include "info.hpp"
#include "../std/std_types.hpp"
#include "../std/type_traits.hpp"
#include "../mp/const_list.hpp"
#include "actions_apply.hpp"

namespace fasthal{
    namespace details{
        template<class TField, class TAction, class TConcrete>
        struct field_action_base{
            using field_t = TField;
            using action_t = TAction;
        };

        template<class TField, class TAction, std::size_t... VValues>
        struct field_action_static: field_action_base<TField, TAction, field_action_static<TField, TAction, VValues...>>{
            constexpr void execute(field_data_type<TField>& current){ TAction::execute(current, VValues...); }
        };

        template<class TField, class TAction, typename TValue>
        struct field_action: field_action_base<TField, TAction, field_action<TField, TAction, TValue>>{
            using value_t = TValue;
            static constexpr std::size_t value_count = 1;

            template<std::size_t N>
            constexpr std::size_t getValue(){return N == 0 ? value : 0;}

            const value_t value;
            constexpr field_action(value_t __value): value(__value){}
            constexpr void execute(field_data_type<TField>& current){ TAction::execute(current, value); }
        };

        template<class TField, class TAction, typename TValue>
        struct field_action_2: field_action_base<TField, TAction, field_action_2<TField, TAction, TValue>>{
            using value_t = TValue;
            static constexpr std::size_t value_count = 2;

            template<std::size_t N>
            constexpr std::size_t getValue(){return N == 0 ? value : (N == 1 ? value2 : 0);}

            const value_t value, value2;
            constexpr field_action_2(value_t __value, value_t __value2): value(__value), value2(__value2){}
            constexpr void execute(field_data_type<TField>& current){ TAction::execute(current, value, value2); }
        };

        // convert dynamic field values to static
        template<class TFieldAction, std::size_t... VValues>
        struct to_static_field_action_impl{
            using field_t = typename TFieldAction::field_t;
            using action_t = typename TFieldAction::action_t;
            using args_t = integral_list<std::size_t, VValues...>;
           
            using target_args_t = at_c<split_at<args_t, size_t<TFieldAction::value_count>>, 0>;

            template<class... TArgs>
            struct make_helper{
                using type = field_action_static<field_t, action_t, TArgs::value...>;
            };

            using type = typename unpack<target_args_t, make_helper>::type;
        };

        template<class TFieldAction, std::size_t... VValues>
        using to_static_field_action = typename to_static_field_action_impl<TFieldAction, VValues...>::type;

        #define FH_TO_STATIC_FIELD_ACTION(a) (::fasthal::details::to_static_field_action<decltype(a), a.get<0>(), a.get<1>()>)
    }

    namespace mp{
        namespace details{
            // don't eat space with static actions
            template<class TField, class TAction, std::size_t... VValues>
            struct is_static_element_impl<fasthal::details::field_action_static<TField, TAction, VValues...>>: std::true_type { };
        }
    }    

    
    // template<class... TFieldAction>
    // struct combine_helper{
    //     static constexpr auto combine(TFieldAction... actions){
    //         return mp::make_const_list(actions...);
    //     }
    // };

    // template<class TFieldAction>
    // struct combine_helper<TFieldAction>{
    //     static constexpr auto combine(TFieldAction action){
    //         return action;
    //     }
    // };

    template<class... TFieldAction>
    constexpr auto inline combine_a(TFieldAction... actions){
        return mp::make_const_list(actions...);
        //return combine_helper<TFieldAction...>::combine(actions...);
    }

    template<class TFieldAction>
    constexpr auto inline combine_a(TFieldAction action){
        return action;
    }
    
    template<class TField, typename TDataType = field_data_type<TField>, enable_if_field_c<TField> dummy = nullptr>
    constexpr auto write_a(TField field, const TDataType value) 
    {
        return details::field_action<TField, details::write_field, TDataType>{value};
    }

    template<class TField, typename TMaskType = field_mask_type<TField>, enable_if_field_c<TField> dummy = nullptr>
    constexpr auto set_a(TField field, TMaskType mask) 
    {
        return details::field_action<TField, details::set_field, TMaskType>{mask};
    }
    
    template<class TField, typename TMaskType = field_mask_type<TField>, enable_if_field_c<TField> dummy = nullptr>
    constexpr auto clear_a(TField field, TMaskType mask) {
        return details::field_action<TField, details::clear_field, TMaskType>{mask};
    }

    template<class TField, typename TMaskType = field_mask_type<TField>, enable_if_field_c<TField> dummy = nullptr>
    constexpr auto clear_set_a(TField field, TMaskType clearMask, TMaskType setMask) {
        return details::field_action_2<TField, details::clear_set_field, TMaskType>{clearMask, setMask};
    }

    template<class TField, typename TMaskType = field_mask_type<TField>, enable_if_field_c<TField> dummy = nullptr>
    constexpr auto toggle_a(TField field, TMaskType mask) {
        return details::field_action<TField, details::toggle_field, TMaskType>{mask};
    }

    template<class TField, enable_if_field_c<TField> dummy = nullptr>
    constexpr auto read_a(TField field) {
        return details::field_action_static<TField, details::read_field>{};
    }

    template<class TField, class... TFields, enable_if_field_c<TField> dummy = nullptr>
    constexpr auto get_a(TField field, details::field_action_results_t<details::field_value<TFields>...> results)
    {
        constexpr auto index = brigand::index_of<brigand::list<TFields...>, TField>::value;
        return mp::get<index>(results).value;
    }

    template<class TField, enable_if_field_c<TField> dummy = nullptr>
    constexpr auto read_i(TField field) {
        return get_a(field, apply(read_a(field)));
    }

    // static actions
    template<std::size_t VValue, class TField, enable_if_field_c<TField> dummy = nullptr>
    constexpr enable_if_field_t<TField, details::field_action_static<TField, details::write_field, VValue>>
    write_a(TField field) 
    {
        return {};
    }

    template<std::size_t VValue, class TField, enable_if_field_c<TField> dummy = nullptr>
    constexpr enable_if_field_t<TField, details::field_action_static<TField, details::set_field, VValue>>
    set_a(TField field) 
    {
        return {};
    }

    template<std::size_t VValue, class TField, enable_if_field_c<TField> dummy = nullptr>
    constexpr enable_if_field_t<TField, details::field_action_static<TField, details::clear_field, VValue>>
    clear_a(TField field) 
    {
        return {};
    }

    template<std::size_t VValue, std::size_t VValue2, class TField, enable_if_field_c<TField> dummy = nullptr>
    constexpr enable_if_field_t<TField, details::field_action_static<TField, details::clear_set_field, VValue, VValue2>>
    clear_set_a(TField field) 
    {
        return {};
    }
    
    template<std::size_t VValue, class TField, enable_if_field_c<TField> dummy = nullptr>
    constexpr enable_if_field_t<TField, details::field_action_static<TField, details::toggle_field, VValue>>
    toggle_a(TField field) 
    {
        return {};
    }

    
}

#endif