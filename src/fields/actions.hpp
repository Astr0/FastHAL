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
                static constexpr inline void execute(mp::const_list<TValue...>& action, field_data_type<TField>& current){
                    TAction::execute(current, mp::get<TSeq::value>(action)...);
                }
            };

            constexpr void inline execute(field_data_type<TField>& current)
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

            constexpr inline void execute(field_data_type<TField>& current)
            { 
                TAction::execute(current);
            }
        };

        template<class TAction, class TField, typename... TValue>
        static constexpr inline auto make_action(TField field, TValue... values){
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
    constexpr auto inline write(TField field, TDataType value) 
    {
        return details::make_action<details::write_field>(field, value);
    }

    template<class TField, enable_if_field_c<TField> dummy = nullptr>
    constexpr auto inline clear(TField field) {
        using datatype_t = field_data_type<TField>;
        return write(field, integral_constant<datatype_t, datatype_t{}>{});
    }

    template<class TField, typename TMaskType = field_mask_type<TField>, enable_if_needs_field_actions<TField> dummy = nullptr>
    constexpr auto inline set(TField field, TMaskType mask) 
    {
        return details::make_action<details::set_field>(field, mask);
    }

    template<class TField, typename TMaskType = field_mask_type<TField>, enable_if_needs_field_actions<TField> dummy = nullptr>
    constexpr auto inline clear(TField field, TMaskType mask) {
        return details::make_action<details::clear_field>(field, mask);
    }

    template<class TField, 
        typename TClearMaskType = field_mask_type<TField>, 
        typename TSetMaskType = field_mask_type<TField>, 
        enable_if_needs_field_actions<TField> dummy = nullptr>
    constexpr auto inline clear_set(TField field, TClearMaskType clearMask, TSetMaskType setMask) {
        return details::make_action<details::clear_set_field>(field, clearMask, setMask);
    }

    template<class TField, typename TMaskType = field_mask_type<TField>, enable_if_needs_field_actions<TField> dummy = nullptr>
    constexpr auto inline toggle(TField field, TMaskType mask) {
        return details::make_action<details::toggle_field>(field, mask);
    }

    template<class TField, enable_if_needs_field_actions<TField> dummy = nullptr>
    constexpr auto inline read(TField field) {
        return details::make_action<details::read_field>(field);
    }

    template<class TField, class... TFields, enable_if_needs_field_actions<TField> dummy = nullptr>
    constexpr auto inline get(TField field, details::field_action_results_t<TFields...> results)
    {
        using fields_t = brigand::list<TFields...>;
        using index_t = brigand::index_of<fields_t, TField>;
        return mp::get<index_t::value>(results).value;
    }

    // Immidiate actions
    template<class TField, typename TDataType = field_data_type<TField>, enable_if_field_c<TField> dummy = nullptr>
    void inline write_(TField field, TDataType value) 
    {
        TField::write(value);
        //apply(write(field, value));
    }

    template<class TField, typename TMaskType = field_mask_type<TField>, enable_if_field_c<TField> dummy = nullptr>
    void inline set_(TField field, TMaskType mask) 
    {
        TField::write(TField::read() | mask);
        //apply(set(field, mask));
    }
    
    template<class TField, typename TMaskType = field_mask_type<TField>, enable_if_field_c<TField> dummy = nullptr>
    void inline clear_(TField field, TMaskType mask) {
        TField::write(TField::read() & ~mask);
        //apply(clear(field, mask));
    }

    template<class TField, 
        typename TClearMaskType = field_mask_type<TField>, 
        typename TSetMaskType = field_mask_type<TField>, 
        enable_if_field_c<TField> dummy = nullptr>
    void inline clear_set_(TField field, TClearMaskType clearMask, TSetMaskType setMask) {
        TField::write((TField::read() & ~clearMask) | setMask);
        //apply(clear_set(field, clearMask, setMask));
    }

    template<class TField, typename TMaskType = field_mask_type<TField>, enable_if_field_c<TField> dummy = nullptr>
    void inline toggle_(TField field, TMaskType mask) {
        TField::write(TField::read() ^ mask);
        //apply(toggle(field, mask));
    }

    template<class TField, enable_if_field_c<TField> dummy = nullptr>
    constexpr auto inline read_(TField field) {
        return TField::read();
        //return get(field, apply(read(field)));
    }
}

#endif