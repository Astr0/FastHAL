#ifndef FH_FIELDACTIONSEX_H_
#define FH_FIELDACTIONSEX_H_

#include "info.hpp"
#include "../std/std_types.hpp"
#include "../std/std_fake.hpp"
#include "../std/type_traits.hpp"
#include "../mp/const_list.hpp"
#include "actions_apply.hpp"

namespace fasthal{
    namespace details{
        template<class TField, class TAction, typename... TValue>
        struct field_action: mp::const_list<TValue...>
        {
            using field_t = TField;
            using action_t = TAction;

            constexpr field_action(){}
            //template<std::enable_if_c<(sizeof...(TValue) > 0)> dummy = nullptr>
            constexpr field_action(TValue... __value): mp::const_list<TValue...>(__value...){}

            template<typename... TSeq>
            struct execute_helper{
                static constexpr void execute(mp::const_list<TValue...>& action, field_data_type<TField>& current){
                    TAction::execute(current, mp::get<TSeq::value>(action)...);
                }
            };

            constexpr void execute(field_data_type<TField>& current)
            { 
                using seq = brigand::make_sequence<brigand::size_t<0>, sizeof...(TValue)>;
                using executor = brigand::unpack<seq, execute_helper>;
                executor::execute(*this, current);
            }
        };

        template<class TField, class TAction>
        struct field_action<TField, TAction>: mp::const_list<>{
            using field_t = TField;
            using action_t = TAction;

            constexpr field_action(){}

            constexpr void execute(field_data_type<TField>& current)
            { 
                TAction::execute(current);
            }
        };

        template<class TAction, class TField, typename... TValue>
        static constexpr auto make_action(TField field, TValue... values){
            return field_action<TField, TAction, TValue...>{values...};
        }

        template<class TField>
        struct needs_field_actions_impl: is_field_impl<TField> {};
    }

    namespace mp{
        namespace details{
            // don't eat space with static actions
            template<class TField, class TAction, typename... TValue>
            struct is_static_element_impl<fasthal::details::field_action<TField, TAction, TValue...>>
                : is_static_element_impl<fasthal::mp::const_list<TValue...>> { };
        }
    }    

    
    template<class... TFieldAction>
    constexpr auto inline combine(TFieldAction... actions){
        return mp::flatten_const_list(actions...);
        //return combine_helper<TFieldAction...>::combine(actions...);
    }

    template<class TFieldAction>
    constexpr auto inline combine(TFieldAction action){
        return action;
    }

    template<class TField>
    using enable_if_needs_field_actions = std::enable_if_c<details::needs_field_actions_impl<std::base_type_t<TField>>::value>;
    
    // ACTIONS
    template<class TField, typename TDataType = field_data_type<TField>, enable_if_needs_field_actions<TField> dummy = nullptr>
    constexpr auto write(TField field, const TDataType value) 
    {
        return details::make_action<details::write_field>(field, value);
    }

    template<class TField, typename TMaskType = field_mask_type<TField>, enable_if_needs_field_actions<TField> dummy = nullptr>
    constexpr auto set(TField field, TMaskType mask) 
    {
        return details::make_action<details::set_field>(field, mask);
    }
    
    template<class TField, typename TMaskType = field_mask_type<TField>, enable_if_needs_field_actions<TField> dummy = nullptr>
    constexpr auto clear(TField field, TMaskType mask) {
        return details::make_action<details::clear_field>(field, mask);
    }

    template<class TField, 
        typename TClearMaskType = field_mask_type<TField>, 
        typename TSetMaskType = field_mask_type<TField>, 
        enable_if_needs_field_actions<TField> dummy = nullptr>
    constexpr auto clear_set(TField field, TClearMaskType clearMask, TSetMaskType setMask) {
        return details::make_action<details::clear_set_field>(field, clearMask, setMask);
    }

    template<class TField, typename TMaskType = field_mask_type<TField>, enable_if_needs_field_actions<TField> dummy = nullptr>
    constexpr auto toggle(TField field, TMaskType mask) {
        return details::make_action<details::toggle_field>(field, mask);
    }

    template<class TField, enable_if_needs_field_actions<TField> dummy = nullptr>
    constexpr auto read(TField field) {
        return details::make_action<details::read_field>(field);
    }

    template<class TField, class... TFields, enable_if_needs_field_actions<TField> dummy = nullptr>
    constexpr auto get(TField field, details::field_action_results_t<TFields...> results)
    {
        using fields_t = brigand::list<TFields...>;
        using index_t = brigand::index_of<fields_t, TField>;
        return mp::get<index_t::value>(results).value;
    }

    // Immidiate actions
    template<class TField, typename TDataType = field_data_type<TField>, enable_if_field_c<TField> dummy = nullptr>
    constexpr void write_(TField field, const TDataType value) 
    {
        apply(write(field, value));
    }

    template<class TField, typename TMaskType = field_mask_type<TField>, enable_if_field_c<TField> dummy = nullptr>
    constexpr void set_(TField field, TMaskType mask) 
    {
        apply(set(field, mask));
    }
    
    template<class TField, typename TMaskType = field_mask_type<TField>, enable_if_field_c<TField> dummy = nullptr>
    constexpr void clear_(TField field, TMaskType mask) {
        apply(clear(field, mask));
    }

    template<class TField, 
        typename TClearMaskType = field_mask_type<TField>, 
        typename TSetMaskType = field_mask_type<TField>, 
        enable_if_field_c<TField> dummy = nullptr>
    constexpr void clear_set_(TField field, TClearMaskType clearMask, TSetMaskType setMask) {
        apply(clear_set(field, clearMask, setMask));
    }

    template<class TField, typename TMaskType = field_mask_type<TField>, enable_if_field_c<TField> dummy = nullptr>
    constexpr void toggle_(TField field, TMaskType mask) {
        apply(toggle(field, mask));
    }

    template<class TField, enable_if_field_c<TField> dummy = nullptr>
    constexpr auto read_(TField field) {
        return get(field, apply(read(field)));
    }
}

#endif