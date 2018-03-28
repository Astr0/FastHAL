#ifndef FH_MP_LIST_H_
#define FH_MP_LIST_H_

#include "type_traits.hpp"

namespace fasthal
{
namespace mp
{

// varargs wrapper for convenience
template <typename... T>
struct list
{
};

// empty list
using empty_list = list<>;

// type at index, dunno why not for list =/
template <unsigned N, typename T, typename ...R>
struct type_at {
    using type = typename type_at<N - 1, R...>::type;
};

template <typename T, typename ...R>
struct type_at<0, T, R...> {
    using type = T;
};

template <unsigned N, typename ...R>
using type_at_t = typename type_at<N, R...>::type;

// check if all varargs match predicate, dunno why not for list =/
template <template<typename> class TPredicate, typename ...R>
struct all: true_type { };

template <template<typename> class TPredicate, typename T, typename ...R>
struct all<TPredicate, T, R...> : bool_constant<TPredicate<T>::value && all<TPredicate, R...>::value>{    
};

// Linq Select
template<typename TList, template<class> class TSelector>
struct select;

template<template<class...>class TList, typename ...T, template<class> class TSelector>
struct select<TList<T...>, TSelector>{
    using type = TList<typename TSelector<T>::type...>;
};

template<typename TList, template<class> class TSelector>
using select_t = typename select<TList, TSelector>::type;

// appends lists into one huge list
template <typename... Ts>
struct append
{
    using type = empty_list;
};
template <typename T>
struct append<T>
{
    using type = T;
};
template <template <typename...> class L1, template <typename...> class L2, typename... T1s,
            typename... T2s, typename... Ts>
struct append<L1<T1s...>, L2<T2s...>, Ts...> : append<L1<T1s..., T2s...>, Ts...>
{
};
template <template <typename...> class L, template <typename...> class L1,
            template <typename...> class L2, template <typename...> class L3,
            template <typename...> class L4, template <typename...> class L5,
            template <typename...> class L6, template <typename...> class L7,
            template <typename...> class L8, template <typename...> class L9,
            template <typename...> class L10, template <typename...> class L11,
            template <typename...> class L12, template <typename...> class L13,
            template <typename...> class L14, template <typename...> class L15,
            template <typename...> class L16, typename... Ts, typename... T1s, typename... T2s,
            typename... T3s, typename... T4s, typename... T5s, typename... T6s, typename... T7s,
            typename... T8s, typename... T9s, typename... T10s, typename... T11s,
            typename... T12s, typename... T13s, typename... T14s, typename... T15s,
            typename... T16s, typename... Us>
struct append<L<Ts...>, L1<T1s...>, L2<T2s...>, L3<T3s...>, L4<T4s...>, L5<T5s...>,
                    L6<T6s...>, L7<T7s...>, L8<T8s...>, L9<T9s...>, L10<T10s...>, L11<T11s...>,
                    L12<T12s...>, L13<T13s...>, L14<T14s...>, L15<T15s...>, L16<T16s...>, Us...>
    : append<L<Ts..., T1s..., T2s..., T3s..., T4s..., T5s..., T6s..., T7s..., T8s...,
                    T9s..., T10s..., T11s..., T12s..., T13s..., T14s..., T15s..., T16s...>,
                    Us...>
{
};
template <typename... Ts>
using append_t = typename append<Ts...>::type;

// Linq SelectMany multiple times so list<a,list<b,c>,d> becomes list<a,b,c,d>

// not_a_list
template <class T>
struct flatten
{
    using type = T;
};

// not_a_list - return list to append
template <template<class...> class TList, class T>
struct flatten_el
{
    using type = TList<T>;
};

// list, append it
template <template<class...> class TList, class... T>
struct flatten_el<TList, TList<T...>>
: append<typename flatten_el<TList, T>::type...>
{
};

// list template deducer
template <template<class...> class TList, class... T>
struct flatten<TList<T...>>
: flatten_el<TList, TList<T...>>
{
};

template <typename TList>
using flatten_t = typename flatten<TList>::type;

};
};

#endif