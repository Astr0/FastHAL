#ifndef FH_FIELDACTIONSEX_H_
#define FH_FIELDACTIONSEX_H_

#include "info.hpp"
#include "../std/std_types.hpp"
#include "../std/type_traits.hpp"
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
        
        template<class... TActions>
        using field_actions_list_t = mp::const_list<TActions...>;      

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
        struct clear_and_set_field{
            template<typename T, typename V>
            static constexpr void execute(T& current, V value1, V value2) { 
                clear_field::execute(current, value1);
                set_field::execute(current, value2);
            }
        };

        template<class TField, class TAction>
        struct field_action_base{
            using field_t = TField;
            using action_t = TAction;
        };

        template<class TField, class TAction, std::size_t VValue>
        struct field_action_static: field_action_base<TField, TAction>{
            constexpr void execute(field_data_type<TField>& current){ TAction::execute(current, VValue); }
        };

        template<class TField, class TAction, std::size_t VValue, std::size_t VValue2>
        struct field_action_static_2: field_action_base<TField, TAction>{
            constexpr void execute(field_data_type<TField>& current){ TAction::execute(current, VValue, VValue2); }
        };

        template<class TField, class TAction, typename TValue>
        struct field_action: field_action_base<TField, TAction>{
            const TValue value;
            constexpr field_action(TValue __value): value(__value){}
            constexpr void execute(field_data_type<TField>& current){ TAction::execute(current, value); }
        };

        template<class TField, class TAction, typename TValue>
        struct field_action_2: field_action_base<TField, TAction>{
            const TValue value, value2;
            constexpr field_action_2(TValue __value, TValue __value2): value(__value), value2(__value2){}
            constexpr void execute(field_data_type<TField>& current){ TAction::execute(current, value, value2); }
        };

    }

    
    template<class... TFieldAction>
    constexpr auto inline combine_a(TFieldAction... actions){
        return mp::make_const_list(actions...);
    }
    
    template<class TField, typename TDataType = field_data_type<TField>>
    constexpr enable_if_field_t<TField, details::field_action<TField, details::write_field, TDataType>>
    write_a(TField field, const TDataType value) 
    {
        return {value};
    }
    template<std::size_t VValue, class TField>
    constexpr enable_if_field_t<TField, details::field_action_static<TField, details::write_field, VValue>>
    write_a(TField field) 
    {
        return {};
    }

    // template<class TField, typename TMaskType = field_mask_type<TField>>
    // constexpr auto clearAndSet(TField field, TMaskType clearMask, TMaskType setMask) 
    // {
    //     TField::write((TField::read() & ~clearMask) | setMask);
    // }

    template<class TField, typename TMaskType = field_mask_type<TField>>
    constexpr enable_if_field_t<TField, details::field_action<TField, details::set_field, TMaskType>>
    set_a(TField field, TMaskType mask) 
    {
        return {mask};
    }
    template<std::size_t VValue, class TField>
    constexpr enable_if_field_t<TField, details::field_action_static<TField, details::set_field, VValue>>
    set_a(TField field) 
    {
        return {};
    }

    template<class TField, typename TMaskType = field_mask_type<TField>>
    constexpr enable_if_field_t<TField, details::field_action<TField, details::clear_field, TMaskType>>
    clear_a(TField field, TMaskType mask) {
        return {mask};
    }
    template<std::size_t VValue, class TField>
    constexpr enable_if_field_t<TField, details::field_action_static<TField, details::clear_field, VValue>>
    clear_a(TField field) 
    {
        return {};
    }

    template<class TField, typename TMaskType = field_mask_type<TField>>
    constexpr enable_if_field_t<TField, details::field_action_2<TField, details::clear_and_set_field, TMaskType>>
    clear_and_set_a(TField field, TMaskType clearMask, TMaskType setMask) {
        return {clearMask, setMask};
    }
    template<std::size_t VValue, std::size_t VValue2, class TField>
    constexpr enable_if_field_t<TField, details::field_action_static_2<TField, details::clear_and_set_field, VValue, VValue2>>
    clear_and_set_a(TField field) 
    {
        return {};
    }

    template<class TField, typename TMaskType = field_mask_type<TField>>
    constexpr enable_if_field_t<TField, details::field_action<TField, details::toggle_field, TMaskType>>
    toggle_a(TField field, TMaskType mask) {
        return {mask};
    }
    template<std::size_t VValue, class TField>
    constexpr enable_if_field_t<TField, details::field_action_static<TField, details::toggle_field, VValue>>
    toggle_a(TField field) 
    {
        return {};
    }

    template<class TField>
    constexpr enable_if_field_t<TField, details::field_action_static<TField, details::read_field, 0>>
    read_a(TField field) {
        return {};
    }

    template<class TField, class... TFields>
    constexpr enable_if_field_t<TField, field_data_type<TField>>
    get_a(TField field, details::field_action_results_t<details::field_value<TFields>...> results)
    {
        constexpr auto index = brigand::index_of<brigand::list<TFields...>, TField>::value;
        return mp::get<index>(results).value;
    }

    namespace mp{
        namespace details{
            // don't eat space with static actions
            template<class TField, class TAction, std::size_t VValue>
            struct is_static_element_impl<fasthal::details::field_action_static<TField, TAction, VValue>>: std::true_type { };

            template<class TField, class TAction, std::size_t VValue, std::size_t VValue2>
            struct is_static_element_impl<fasthal::details::field_action_static_2<TField, TAction, VValue, VValue2>>: std::true_type { };
        }
    }    
}

#endif