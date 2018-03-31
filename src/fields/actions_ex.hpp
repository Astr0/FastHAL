#ifndef FH_FIELDACTIONSEX_H_
#define FH_FIELDACTIONSEX_H_

#include "info.hpp"
#include "../std/std_types.hpp"
#include "../std/type_traits.hpp"
#include "../mp/const_list.hpp"

namespace fasthal{
    struct write_field{
        template<typename T, typename V>
        static constexpr T execute(T current, V value){
            return value;
        }
    };
    struct set_field{
        template<typename T, typename V>
        static constexpr T execute(T current, V value){
            return current | value;
        }
    };
    struct clear_field{
        template<typename T, typename V>
        static constexpr T execute(T current, V value){
            return current & ~value;
        }
    };
    struct toggle_field{
        template<typename T, typename V>
        static constexpr T execute(T current, V value){
            return current ^ value;
        }
    };
    struct read_field{
        template<typename T, typename V>
        static constexpr T execute(T current, V value){
            return current;
        }
    };

    template<class TField, class TAction, std::size_t VValue>
    struct field_action_static{
        using field_t = TField;
        using action_t = TAction;

        template<typename T>
        constexpr T execute(T current){
            return TAction::execute(current, VValue);
        }
    };

    template<class TField, class TAction, typename TValue>
    struct field_action{
        using field_t = TField;
        using action_t = TAction;

        constexpr field_action(TValue __value): value(__value){}

        const TValue value;

        template<typename T>
        constexpr T execute(T current){
            return TAction::execute(current, value);
        }
    };
    
    
    template<class TField, typename TDataType = field_data_type<TField>>
    constexpr auto write_a(TField field, const TDataType value) 
    {
        return field_action<TField, write_field, TDataType>{value};
    }

    template<std::size_t VValue, class TField>
    constexpr auto write_a(TField field) 
    {
        return field_action_static<TField, write_field, VValue>{};
    }

    // template<class TField, typename TMaskType = field_mask_type<TField>>
    // constexpr auto clearAndSet(TField field, TMaskType clearMask, TMaskType setMask) 
    // {
    //     TField::write((TField::read() & ~clearMask) | setMask);
    // }

    template<class TField, typename TMaskType = field_mask_type<TField>>
    constexpr auto set_a(TField field, TMaskType mask) 
    {
        return field_action<TField, set_field, TMaskType>{mask};
    }

    template<std::size_t VValue, class TField>
    constexpr auto set_a(TField field) 
    {
        return field_action_static<TField, set_field, VValue>{};
    }

    template<class TField, typename TMaskType = field_mask_type<TField>>
    constexpr auto clear_a(TField field, TMaskType mask) {
        return field_action<TField, clear_field, TMaskType>{mask};
    }
    template<std::size_t VValue, class TField>
    constexpr auto clear_a(TField field) 
    {
        return field_action_static<TField, clear_field, VValue>{};
    }


    template<class TField, typename TMaskType = field_mask_type<TField>>
    constexpr auto toggle_a(TField field, TMaskType mask) {
        return field_action<TField, toggle_field, TMaskType>{mask};
    }
    template<std::size_t VValue, class TField>
    constexpr auto toggle_a(TField field) 
    {
        return field_action_static<TField, toggle_field, VValue>{};
    }

    template<class TField>
    constexpr auto read_a(TField field) {
        return field_action<TField, read_field, void>{};
    }

    namespace mp{
        namespace details{
            // don't eat space with static actions
            template<class TField, class TAction, std::size_t VValue>
            struct is_static_element_impl<field_action_static<TField, TAction, VValue>>: std::true_type { };
        }
    }    
}

#endif