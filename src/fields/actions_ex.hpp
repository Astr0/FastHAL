#ifndef FH_FIELDACTIONSEX_H_
#define FH_FIELDACTIONSEX_H_

#include "info.hpp"

namespace fasthal{
    struct write_field{};
    struct set_field{};
    struct clear_field{};
    struct toggle_field{};
    struct read_field{};

    template<class TField, class TAction, typename TValue>
    struct field_action: field_action<TField, TAction, void>{
        using value_t = TValue;

        constexpr field_action(value_t __value): value(__value){}

        const TValue value;
    };
    template<class TField, class TAction>
    struct field_action<TField, TAction, void>{
        using field_t = TField;
        using action_t = TAction;
    };
    
    template<class TField, typename TDataType = field_data_type<TField>>
    constexpr auto write_ex(TField field, TDataType value) 
    {
        return field_action<TField, write_field, TDataType>{value};
    }

    // template<class TField, typename TMaskType = field_mask_type<TField>>
    // constexpr auto clearAndSet(TField field, TMaskType clearMask, TMaskType setMask) 
    // {
    //     TField::write((TField::read() & ~clearMask) | setMask);
    // }

    template<class TField, typename TMaskType = field_mask_type<TField>>
    constexpr auto set_ex(TField field, TMaskType mask) 
    {
        return field_action<TField, set_field, TMaskType>{mask};
    }

    template<class TField, typename TMaskType = field_mask_type<TField>>
    constexpr auto clear_ex(TField field, TMaskType mask) {
        return field_action<TField, clear_field, TMaskType>{mask};
    }
    template<class TField, typename TMaskType = field_mask_type<TField>>
    constexpr auto toggle_ex(TField field, TMaskType mask) {
        return field_action<TField, toggle_field, TMaskType>{mask};
    }

    template<class TField>
    constexpr auto read_ex(TField field) {
        return field_action<TField, read_field, void>{};
    }

}

#endif