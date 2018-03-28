#ifndef FH_ACTIONS_H_
#define FH_ACTIONS_H_

#include "../mp/type_traits.hpp"

namespace fasthal{
    namespace actions{
        struct Toggle{};


        template<typename TBitField, typename TAction>
        struct Action: TAction{
            template<typename ...Ts>
            constexpr Action(Ts... args): TAction{args...}{}

            using FieldBits = typename TBitField::FieldBits;
        };
    }

    template <typename TBitField>
    constexpr mp::enable_if_t<details::is_bit_field<TBitField>::value, unsigned> write(TBitField field, unsigned v){
        return {};
    }

    template <typename TBitField>
    constexpr mp::enable_if_t<details::is_bit_field<TBitField>::value, unsigned> set(TBitField field){
        return {};
    }

    template <typename TBitField>
    constexpr mp::enable_if_t<details::is_bit_field<TBitField>::value, unsigned> clear(TBitField field){
        return {};
    }

    template <typename TBitField>
    constexpr mp::enable_if_t<details::is_bit_field<TBitField>::value, actions::Action<TBitField, actions::Toggle>> toggle(TBitField field){
        return {};
    }

    template <typename TBitField>
    constexpr mp::enable_if_t<details::is_bit_field<TBitField>::value, unsigned> read(TBitField field){
        return {};
    }
};

#endif