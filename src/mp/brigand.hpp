/*!
@file

@copyright Edouard Alligand and Joel Falcou 2015-2017
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/
#define BRIGAND_NO_BOOST_SUPPORT


#include "../std/utility.hpp"
#include "../std/type_traits.hpp"
#include "../std/initializer_list.hpp"

#ifndef BRIGAND_HPP_INCLUDED
#define BRIGAND_HPP_INCLUDED

#define BRIGAND_COMP_MSVC_2013
// #if defined(_MSC_VER) && !defined(__GNUC__) && !defined(__clang__)
// #define BRIGAND_COMP_MSVC
// #if _MSC_VER == 1900
// #define BRIGAND_COMP_MSVC_2015
// #elif _MSC_VER == 1800
// #define BRIGAND_COMP_MSVC_2013
// #endif
// #elif __INTEL_COMPILER
// #define BRIGAND_COMP_INTEL
// #elif __GNUC__
// #ifndef __clang__
// #define BRIGAND_COMP_GCC
// #else
// #define BRIGAND_COMP_CLANG
// #endif
// #endif
// #if defined(__CUDACC__)
// #define BRIGAND_COMP_CUDA
// #endif

namespace brigand
{
template <typename T, T Value>
struct integral_constant
{
    static const T value = Value;
    using value_type = T;
    using type = integral_constant<T, Value>;
    operator value_type() const { return (value); }
    value_type operator()() const { return (value); }
};
template <typename T, T Value>
const T integral_constant<T, Value>::value;
using true_type = ::brigand::integral_constant<bool, true>;
using false_type = ::brigand::integral_constant<bool, false>;
}
namespace brigand
{
template <class... T>
struct list
{
};
template <typename T, T... Values>
using integral_list = brigand::list<brigand::integral_constant<T, Values>...>;
using empty_sequence = brigand::list<>;
}
namespace brigand
{
namespace detail
{
    template <class, class>
    struct dup_append_list;
    template <template <class...> class List, class... Ts, class... Us>
    struct dup_append_list<List<Ts...>, List<Us...>>
    {
        using type = List<Ts..., Ts..., Us...>;
    };
    template <class T, template <class...> class List, unsigned int N>
    struct filled_list_impl
        : dup_append_list<typename filled_list_impl<T, List, N / 2>::type,
                          typename filled_list_impl<T, List, N - N / 2 * 2>::type>
    {
    };
    template <class T, template <class...> class List>
    struct filled_list_impl<T, List, 1>
    {
        using type = List<T>;
    };
    template <class T, template <class...> class List>
    struct filled_list_impl<T, List, 0>
    {
        using type = List<>;
    };
}
template <class T, unsigned int N, template <class...> class List = list>
using filled_list = typename detail::filled_list_impl<T, List, N>::type;
}
namespace brigand
{
  namespace lazy
  {
    template <class A, template<class...> class B> struct wrap;
    template<template<class...> class A, class... T, template<class...> class B>
    struct wrap<A<T...>, B>
    {
      using type = B<T...>;
    };
  }
  template<class A, template<class...> class B>
  using wrap = typename lazy::wrap<A, B>::type;
}
namespace brigand
{
namespace detail
{
    template <typename... Ts>
    struct append_impl
    {
        using type = brigand::empty_sequence;
    };
    template <typename T>
    struct append_impl<T>
    {
        using type = T;
    };
    template <template <typename...> class L1, template <typename...> class L2, typename... T1s,
              typename... T2s, typename... Ts>
    struct append_impl<L1<T1s...>, L2<T2s...>, Ts...> : append_impl<L1<T1s..., T2s...>, Ts...>
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
    struct append_impl<L<Ts...>, L1<T1s...>, L2<T2s...>, L3<T3s...>, L4<T4s...>, L5<T5s...>,
                       L6<T6s...>, L7<T7s...>, L8<T8s...>, L9<T9s...>, L10<T10s...>, L11<T11s...>,
                       L12<T12s...>, L13<T13s...>, L14<T14s...>, L15<T15s...>, L16<T16s...>, Us...>
        : append_impl<L<Ts..., T1s..., T2s..., T3s..., T4s..., T5s..., T6s..., T7s..., T8s...,
                        T9s..., T10s..., T11s..., T12s..., T13s..., T14s..., T15s..., T16s...>,
                      Us...>
    {
    };
}
namespace lazy
{
    template <typename... Ts>
    using append = detail::append_impl<Ts...>;
}
template <typename... Ts>
using append = typename detail::append_impl<Ts...>::type;
namespace lazy
{
    template <typename T>
    struct join;
    template <template <typename...> class L, typename... Ts>
    struct join<L<Ts...>> : ::brigand::detail::append_impl<L<>, Ts...>
    {
    };
}
template <typename T>
using join = wrap<T, append>;
}
namespace brigand
{
    template <typename First, typename Second>
    struct pair
    {
        using first_type = First;
        using second_type = Second;
    };
}
namespace brigand
{
    struct no_such_type_ {};
}
namespace brigand
{
  template<typename T> struct type_ { using type = T; };
  template<typename T> using type_from = typename T::type;
}

namespace brigand
{
namespace lazy
{
    template <typename M, typename K>
    struct lookup_at
    {
        using target_t = type_<K>;
        using type = decltype(M::at(target_t{}));
    };
    template <typename M, typename K>
    struct lookup : lookup_at<M, K>::type
    {
    };
}
template <typename M, typename K>
using lookup = typename lazy::lookup<M, K>::type;
namespace detail
{
    template <class... T>
    struct map_impl;
    template <>
    struct map_impl<>
    {
        template <typename U>
        static type_<no_such_type_> at(U);
        template <class K>
        static brigand::false_type has_key(type_<K>);
        template <class K>
        static map_impl erase(type_<K>);
        template <class P>
        static map_impl<P> insert(type_<P>);
    };
    template <class... Ts>
    struct map_impl
    {
    private:
        struct Pack : pair<typename Ts::first_type, Ts>...
        {
        };
        template <class K, class P>
        static type_<typename P::second_type> at_impl(pair<K, P> *);
    public:
        template <class K>
        static decltype(at_impl<K>(static_cast<Pack *>(nullptr))) at(type_<K>);
        template <class K>
        static type_<no_such_type_> at(K);
        template <class K, class = decltype(at_impl<K>(static_cast<Pack *>(nullptr)))>
        static brigand::true_type has_key(type_<K>);
        template <class K>
        static brigand::false_type has_key(K);
        template <class K, class X>
        using erase_t = typename std::conditional<std::is_same<K, typename X::first_type>::value,
                                                  list<>, list<X>>::type;
        template <class K, typename... Xs>
        struct erase_return_t
        {
            using type = append<map_impl<>, erase_t<K, Xs>...>;
        };
        template <class K>
        static typename erase_return_t<K, Ts...>::type erase(type_<K>);
        template <class P, class = decltype(static_cast<pair<typename P::first_type, P> *>(
                               static_cast<Pack *>(nullptr)))>
        static map_impl insert(type_<P>);
        template <class P>
        static map_impl<Ts..., typename P::type> insert(P);
    };
    template <class... Ts>
    struct make_map : type_<typename Ts::first_type>...
    {
        using type = map_impl<Ts...>;
    };
}
template <class... Ts>
using map = typename detail::make_map<Ts...>::type;
}
namespace brigand
{
namespace detail
{
    template <class T>
    struct element_at;
    template <class... Ts>
    struct element_at<list<Ts...>>
    {
        template <class T>
        type_<T> static at(Ts..., type_<T> *, ...);
    };
    template <unsigned int N, typename Seq>
    struct at_impl;
    template <unsigned int N, template <typename...> class L, class... Ts>
    struct at_impl<N, L<Ts...>> : decltype(element_at<brigand::filled_list<void const *, N>>::at(
                                      static_cast<type_<Ts> *>(nullptr)...))
    {
    };
}
template <class L, unsigned int Index>
using at_c = typename detail::at_impl<Index, L>::type;
namespace detail
{
    template <typename T>
    struct has_at_method
    {
        struct dummy
        {
        };
        template <typename C, typename P>
        static auto test(P * p) -> decltype(C::at(*p), brigand::true_type());
        template <typename, typename>
        static brigand::false_type test(...);
        static const bool value =
            std::is_same<brigand::true_type, decltype(test<T, dummy>(nullptr))>::value;
    };
    template <class L, typename Index, bool>
    struct at_dispatch
    {
        using type = at_c<L, Index::value>;
    };
    template <class L, typename Index>
    struct at_dispatch<L, Index, true>
    {
        using type = lookup<L, Index>;
    };
}
template <class Seq, typename K>
using at = typename detail::at_dispatch<Seq, K, detail::has_at_method<Seq>::value>::type;
}
namespace brigand
{
  template <template <typename...> class, typename...>
  struct bind
  {
  };
}
namespace brigand
{
  template<unsigned int Index> struct args
  {
  };
  struct _1 {};
  struct _2 {};
  using _3 = args<2>;
  using _4 = args<3>;
  using _5 = args<4>;
  using _6 = args<5>;
  using _7 = args<6>;
  using _8 = args<7>;
  using _9 = args<8>;
  using _state = _1;
  using _element = _2;
}
namespace brigand
{
template <typename T>
struct defer
{
};
template <typename T>
struct pin
{
};
template <typename T>
struct parent
{
};
namespace detail
{
    template <typename T, typename... Ts>
    struct packaged_lcall
    {
    };
    template <typename T, typename... Ls>
  struct apply {
    using type = T;
  };
  template <template<typename...> class F, typename...Ts, typename... Args>
  struct apply<bind<F,Ts...>, Args...>
  {
    using type = F<typename apply<Ts, Args...>::type...>;
  };
  template <template <typename...> class F, typename... Ts, typename L, typename... Ls>
  struct apply<F<Ts...>, L, Ls...> : F<typename apply<Ts, L, Ls...>::type...>
  {
  };
  template <typename T, typename... Args, typename...Ls>
  struct apply<pin<T>, list<Args...>, Ls...>
  {
    using type = T;
  };
  template <unsigned int N, typename L, typename...Ls>
  struct apply<args<N>, L, Ls...>
  {
    using type = at_c<L, N>;
  };
  template <typename T, typename...Ts, typename...Ls>
  struct apply<_1, list<T, Ts...>, Ls...>
  {
    using type = T;
  };
  template <typename T, typename U, typename...Ts, typename...Ls>
  struct apply<_2, list<T, U, Ts...>, Ls...>
  {
    using type = U;
  };
  template <typename T, typename L, typename...Ls>
  struct apply<parent<T>, L, Ls...> : apply<T,Ls...>
  {
  };
  template <typename Lambda, typename L, typename...Ls>
  struct apply<defer<Lambda>, L, Ls...>
  {
    using type = packaged_lcall<Lambda, L, Ls...>;
  };
  template <template <typename...> class Lambda, typename... Ts, typename... PLs, typename L, typename...Ls>
  struct apply<packaged_lcall<Lambda<Ts...>, PLs...>, L, Ls...> : Lambda<typename apply<Ts, L, Ls..., PLs...>::type...>
  {
  };
  template <template <typename...> class Lambda, typename... Ts, typename... PLs, typename L, typename...Ls>
  struct apply<packaged_lcall<bind<Lambda,Ts...>, PLs...>, L, Ls...>
  {
    using type = Lambda<typename apply<Ts, L, Ls..., PLs...>::type...>;
  };
  template<typename T, typename...Ts>
  using bound_apply = typename apply<T, brigand::list<Ts...>>::type;
}
template <typename Lambda, typename... Args>
using apply = typename detail::apply<Lambda, brigand::list<Args...>>::type;
}

namespace brigand
{
namespace detail
{
  template<class L>
  struct clear_impl;
  template<template<class...> class L, class... Ts>
  struct clear_impl<L<Ts...>>
  {
    using type = L<>;
  };
}
  template<class L>
  using clear = typename detail::clear_impl<L>::type;
}
namespace brigand
{
namespace detail
{
    template <bool b, typename O, typename L, unsigned int I>
    struct split_at_impl;
#if defined(BRIGAND_COMP_INTEL)
    template <template <typename...> class S, typename... Os>
    struct split_at_impl<false, S<Os...>, S<>, 0>
    {
        using type = S<S<Os...>, S<>>;
    };
#else
    template <template <typename...> class S, typename... Os, typename... Ts>
    struct split_at_impl<false, S<Os...>, S<Ts...>, 0>
    {
        using type = S<S<Os...>, S<Ts...>>;
    };
#endif
    template <template <typename...> class S, typename... Os, typename T, typename... Ts>
    struct split_at_impl<false, S<Os...>, S<T, Ts...>, 0>
    {
        using type = S<S<Os...>, S<T, Ts...>>;
    };
    template <template <typename...> class S, typename... Os, typename T, typename... Ts,
              unsigned int I>
    struct split_at_impl<false, S<Os...>, S<T, Ts...>, I>
        : split_at_impl<false, S<Os..., T>, S<Ts...>, (I - 1)>
    {
    };
    template <template <typename...> class S, typename... Os, typename T1, typename T2, typename T3,
              typename T4, typename T5, typename T6, typename T7, typename T8, typename T9,
              typename T10, typename T11, typename T12, typename T13, typename T14, typename T15,
              typename T16, typename... Ts, unsigned int I>
    struct split_at_impl<
        true, S<Os...>,
        S<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, Ts...>, I>
        : split_at_impl<((I - 16) > 16), S<Os..., T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
                                           T13, T14, T15, T16>,
                        S<Ts...>, (I - 16)>
    {
    };
    template <typename L, typename I>
    struct call_split_at_impl : split_at_impl<(I::value > 16), brigand::clear<L>, L, I::value>
    {
    };
}
namespace lazy
{
    template <typename L, typename I>
    using split_at = ::brigand::detail::call_split_at_impl<L, I>;
}
template <typename L, typename I>
using split_at = typename ::brigand::lazy::split_at<L, I>::type;
}
namespace brigand
{
namespace detail
{
    template <class L, class... T>
    struct push_front_impl;
    template <template <class...> class L, class... U, class... T>
    struct push_front_impl<L<U...>, T...>
    {
        using type = L<T..., U...>;
    };
}
namespace lazy
{
    template <class L, class... T>
    struct push_front : detail::push_front_impl<L, T...>
    {
    };
}
template <class L, class... T>
using push_front = typename detail::push_front_impl<L, T...>::type;
namespace detail
{
    template <class L>
    struct front_impl;
    template <template <class...> class L, class T, class... U>
    struct front_impl<L<T, U...>>
    {
        using type = T;
    };
}
template <class L>
using front = typename detail::front_impl<L>::type;
namespace detail
{
    template <class L, unsigned int N>
    struct pop_front_impl;
    template <template <class...> class L, class T, class... U>
    struct pop_front_impl<L<T, U...>, 1>
    {
        using type = L<U...>;
    };
    template <template <class...> class L, class>
    struct pop_front_element;
    template <template <class...> class L, class... Ts>
    struct pop_front_element<L, list<Ts...>>
    {
        template <class... Us>
        static L<Us...> impl(Ts..., type_<Us> *...);
    };
    template <template <class...> class L, class... Ts, unsigned int N>
    struct pop_front_impl<L<Ts...>, N>
    {
        using type = decltype(pop_front_element<L, filled_list<void const *, N>>::impl(
            static_cast<type_<Ts> *>(nullptr)...));
    };
}
namespace lazy
{
    template <class L, class N = brigand::integral_constant<unsigned int, 1>>
    struct pop_front : detail::pop_front_impl<L, N::value>
    {
    };
}
template <class L, class N = brigand::integral_constant<unsigned int, 1>>
using pop_front = typename detail::pop_front_impl<L, N::value>::type;
}
namespace brigand
{
  template<unsigned int Index> struct args;
  namespace detail
  {
    template<typename T, typename List>
    struct substitute_impl
    {
      using type = T;
    };
    template<template<class...> class T, typename... Ts, typename List>
    struct substitute_impl<T<Ts...>,List>
    {
      using type = T<typename substitute_impl<Ts,List>::type...>;
    };
    template<unsigned int Index, typename List>
    struct substitute_impl<args<Index>,List>
    {
      using type = brigand::at_c<List,Index>;
    };
  }
  template<typename T, typename List>
  using substitute = typename detail::substitute_impl<T,List>::type;
}

namespace brigand
{
template <std::int8_t V>
using int8_t = brigand::integral_constant<std::int8_t, V>;
template <std::uint8_t V>
using uint8_t = brigand::integral_constant<std::uint8_t, V>;
template <std::int16_t V>
using int16_t = brigand::integral_constant<std::int16_t, V>;
template <std::uint16_t V>
using uint16_t = brigand::integral_constant<std::uint16_t, V>;
template <std::int32_t V>
using int32_t = brigand::integral_constant<std::int32_t, V>;
template <std::uint32_t V>
using uint32_t = brigand::integral_constant<std::uint32_t, V>;
template <std::int64_t V>
using int64_t = brigand::integral_constant<std::int64_t, V>;
template <std::uint64_t V>
using uint64_t = brigand::integral_constant<std::uint64_t, V>;
template <std::size_t V>
using size_t = brigand::integral_constant<std::size_t, V>;
template <std::ptrdiff_t V>
using ptrdiff_t = brigand::integral_constant<std::ptrdiff_t, V>;
}
namespace brigand
{
namespace detail
{
#if defined(BRIGAND_COMP_GCC) || defined(BRIGAND_COMP_CLANG)
    constexpr unsigned int count_bools(bool const * const begin, bool const * const end,
                                      unsigned int n)
    {
        return begin == end ? n : detail::count_bools(begin + 1, end, n + *begin);
    }
#endif
    template <bool... Bs>
    struct template_count_bools
    {
        using type = ::brigand::size_t<0>;
    };
    template <bool B, bool... Bs>
    struct template_count_bools<B, Bs...>
    {
        using type = ::brigand::size_t<B + template_count_bools<Bs...>::type::value>;
    };
    template <bool B1, bool B2, bool B3, bool B4, bool B5, bool B6, bool B7, bool B8, bool B9,
              bool B10, bool B11, bool B12, bool B13, bool B14, bool B15, bool B16, bool... Bs>
    struct template_count_bools<B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, B11, B12, B13, B14, B15,
                                B16, Bs...>
    {
        using type =
            ::brigand::size_t<B1 + B2 + B3 + B4 + B5 + B6 + B7 + B8 + B9 + B10 + B11 + B12 + B13 +
                              B14 + B15 + B16 + template_count_bools<Bs...>::type::value>;
    };
}
namespace lazy
{
    template <typename List, typename Pred>
    struct count_if
    {
    };
    template <template <typename...> class S, typename Pred>
    struct count_if<S<>, Pred>
    {
        using type = ::brigand::size_t<0>;
    };
#if defined(BRIGAND_COMP_GCC) || defined(BRIGAND_COMP_CLANG)
    template <template <typename...> class S, template <typename...> class F>
    struct count_if<S<>, bind<F, _1>>
    {
        using type = ::brigand::size_t<0>;
    };
    template <template <typename...> class S, template <typename...> class F>
    struct count_if<S<>, F<_1>>
    {
        using type = ::brigand::size_t<0>;
    };
    template <template <typename...> class S, typename... Ts, typename Pred>
    struct count_if<S<Ts...>, Pred>
    {
        static constexpr bool s_v[] = {::brigand::apply<Pred, Ts>::type::value...};
        using type = brigand::size_t<::brigand::detail::count_bools(s_v, s_v + sizeof...(Ts), 0u)>;
    };
    template <template <typename...> class S, typename... Ts, template <typename...> class F>
    struct count_if<S<Ts...>, bind<F, _1>>
    {
        static constexpr bool s_v[] = {F<Ts>::value...};
        using type = brigand::size_t<::brigand::detail::count_bools(s_v, s_v + sizeof...(Ts), 0u)>;
    };
    template <template <typename...> class S, typename... Ts, template <typename...> class F>
    struct count_if<S<Ts...>, F<_1>>
    {
        static constexpr bool s_v[] = {F<Ts>::type::value...};
        using type = brigand::size_t<::brigand::detail::count_bools(s_v, s_v + sizeof...(Ts), 0u)>;
    };
#else
#if defined(BRIGAND_COMP_MSVC_2015)
    template <template <typename...> class S, typename... Ts, typename Pred>
    struct count_if<S<Ts...>, Pred>
        : ::brigand::detail::template_count_bools<::brigand::apply<Pred, Ts>::value...>
    {
    };
#else
    template <template <typename...> class S, typename... Ts, typename Pred>
    struct count_if<S<Ts...>, Pred>
    {
        template <typename T>
        using val_t = brigand::apply<Pred, T>;
        using type = typename ::brigand::detail::template_count_bools<val_t<Ts>::value...>::type;
    };
#endif
#endif
}
template <typename List, typename Pred>
using count_if = typename lazy::count_if<List, Pred>::type;
template <class... T>
using count = brigand::integral_constant<unsigned int, sizeof...(T)>;
}
namespace brigand
{
  template<class L> using size = wrap<L, count>;
}
namespace brigand
{
namespace detail
{
    template <class L, class... T>
    struct push_back_impl;
    template <template <class...> class L, class... U, class... T>
    struct push_back_impl<L<U...>, T...>
    {
        using type = L<U..., T...>;
    };
}
template <class L, class... T>
using push_back = typename detail::push_back_impl<L, T...>::type;
template <class L>
using back = at_c<L, size<L>::value - 1>;
template <class L, class N = brigand::integral_constant<unsigned int, 1>>
using pop_back =
    front<split_at<L, brigand::integral_constant<unsigned int, size<L>::value - N::value>>>;
}
namespace brigand
{
namespace detail
{
    template <class L1, class L2>
    struct rot90;
    template <class... L1, template <class...> class S1, class... T1, template <class...> class S2,
              class... T2, template <class...> class S3, class... T3, template <class...> class S4,
              class... T4, template <class...> class S5, class... T5, template <class...> class S6,
              class... T6, template <class...> class S7, class... T7, template <class...> class S8,
              class... T8, class... L2>
    struct rot90<list<L1...>, list<S1<T1...>, S2<T2...>, S3<T3...>, S4<T4...>, S5<T5...>, S6<T6...>,
                                   S7<T7...>, S8<T8...>, L2...>>
        : rot90<list<push_back<L1, T1, T2, T3, T4, T5, T6, T7, T8>...>, list<L2...>>
    {
    };
    template <class... L1, template <class...> class S, class... T, class... L2>
    struct rot90<list<L1...>, list<S<T...>, L2...>> : rot90<list<push_back<L1, T>...>, list<L2...>>
    {
    };
    template <class L1>
    struct rot90<L1, list<>>
    {
        using type = L1;
    };
    template <class Func, class Seq1, class Seq2, class Seqs>
    struct transform_impl;
    template <class F, class T1, class T2, class Seq>
    struct transform_apply;
    template <class F, class T1, class T2, class... Ts>
    struct transform_apply<F, T1, T2, list<Ts...>>
    {
        using type = brigand::apply<F, T1, T2, Ts...>;
    };
    template <class Func, template <class...> class Seq1, class... T1,
              template <class...> class Seq2, class... T2, class... Seqs>
    struct transform_impl<Func, Seq1<T1...>, Seq2<T2...>, list<Seqs...>>
    {
        using type = Seq1<typename transform_apply<Func, T1, T2, Seqs>::type...>;
    };
    template <unsigned int N, class Seq1, class Seq2, class... FuncOrSeqs>
    struct transform : transform_impl<back<list<FuncOrSeqs...>>, Seq1, Seq2,
                                      typename rot90<filled_list<list<>, size<Seq1>::value>,
                                                     pop_back<list<FuncOrSeqs...>>>::type>
    {
    };
    template <template <class...> class Seq, class... T, class Func>
    struct transform<0, Seq<T...>, Func>
    {
        using type = Seq<brigand::apply<Func, T>...>;
    };
    template <template <class...> class Seq, class... T, template <typename...> class Func>
    struct transform<0, Seq<T...>, bind<Func, _1>>
    {
        using type = Seq<Func<T>...>;
    };
    template <template <class...> class Seq, class... T, template <typename...> class Func>
    struct transform<0, Seq<T...>, Func<_1>>
    {
        using type = Seq<typename Func<T>::type...>;
    };
    template <template <class...> class Seq1, class... T1, template <class...> class Seq2,
              class... T2, class Func>
    struct transform<1, Seq1<T1...>, Seq2<T2...>, Func>
    {
        using type = Seq1<brigand::apply<Func, T1, T2>...>;
    };
}
namespace lazy
{
    template <typename Sequence1, typename OpSeq1, typename... OpSeq2>
    struct transform : detail::transform<sizeof...(OpSeq2), Sequence1, OpSeq1, OpSeq2...>
    {
    };
}
template <typename Sequence1, typename OpSeq1, typename... OpSeq2>
using transform = typename detail::transform<sizeof...(OpSeq2), Sequence1, OpSeq1, OpSeq2...>::type;
}
namespace brigand
{
template <typename T>
struct make_integral : brigand::integral_constant<typename T::value_type, T::value>
{
};
template <typename L>
using as_integral_list = transform<L, make_integral<brigand::_1>>;
}
namespace brigand
{
namespace detail
{
    template <typename L, template <class...> class Sequence>
    struct as_sequence_impl
    {
        using type = wrap<L, Sequence>;
    };
}
template <typename L, template <class...> class Sequence>
using as_sequence = typename detail::as_sequence_impl<L, Sequence>::type;
template <typename L>
using as_list = as_sequence<L, brigand::list>;
}

// namespace brigand
// {
//   template <typename... T>
//   struct pair_wrapper_
//   {
//     static_assert (sizeof...(T) == 2
//                   , "as_pair requires a type list of exactly two types"
//                   );
//     using type = no_such_type_;
//   };
//   template <typename T, typename U>
//   struct pair_wrapper_<T,U>
//   {
//       using type = std::pair<T,U>;
//   };
//   template <typename... T>
//   using pair_wrapper = typename pair_wrapper_<T...>::type;
//   template <typename L>
//   using as_pair = wrap<L, pair_wrapper>;
// }

// namespace brigand
// {
//   template <typename... T>
//   using tuple_wrapper = typename std::tuple<T...>;
//   template <typename L>
//   using as_tuple = wrap<L, tuple_wrapper>;
// }
// #if !defined(BRIGAND_NO_BOOST_SUPPORT)
// #include <boost/fusion/container/vector/vector_fwd.hpp>
// #include <boost/fusion/container/deque/deque_fwd.hpp>
// #include <boost/fusion/container/list/list_fwd.hpp>
// #include <boost/fusion/container/set/set_fwd.hpp>
// namespace brigand
// {
//   template <typename... T>
//   using fusion_vector_wrapper = boost::fusion::vector<T...>;
//   template <typename... T>
//   using fusion_list_wrapper = boost::fusion::list<T...>;
//   template <typename... T>
//   using fusion_deque_wrapper = boost::fusion::deque<T...>;
//   template <typename... T>
//   using fusion_set_wrapper = boost::fusion::set<T...>;
//   template <typename L> using as_fusion_vector = wrap<L, fusion_vector_wrapper>;
//   template <typename L> using as_fusion_deque  = wrap<L, fusion_deque_wrapper>;
//   template <typename L> using as_fusion_list   = wrap<L, fusion_list_wrapper>;
//   template <typename L> using as_fusion_set    = wrap<L, fusion_set_wrapper>;
// }
// #include <boost/variant.hpp>
// namespace brigand
// {
//   template <typename... T>
//   using variant_wrapper = typename boost::variant<T...>;
//   template <typename L>
//   using as_variant = wrap<L, variant_wrapper>;
// }
// #endif
namespace brigand
{
template <bool B>
using bool_ = brigand::integral_constant<bool, B>;
}
namespace brigand
{
namespace detail
{
    template <typename Args>
  struct non_null_impl : bool_<Args::value != 0>{};
    using non_null = non_null_impl<_1>;
}
}


namespace brigand
{
#if defined(BRIGAND_COMP_MSVC_2013) || defined(BRIGAND_COMP_CUDA) || defined(BRIGAND_COMP_INTEL)
namespace detail
{
    template <class P, class T>
    struct all_helper : ::brigand::apply<P, T>
    {
    };
    template <bool...>
    struct bools_
    {
    };
    template <typename Sequence, typename Predicate, typename... Ts>
    struct all_impl;
    template <template <class...> class Sequence, typename Predicate, typename... Ts>
    struct all_impl<Sequence<Ts...>, Predicate>
        : std::is_same<bools_<true, all_helper<Predicate, Ts>::value...>,
                       bools_<all_helper<Predicate, Ts>::value..., true>>
    {
    };
}
#else
namespace detail
{
    struct all_same
    {
        const bool value = false;
        constexpr all_same(...) {}
        template <typename T>
        constexpr all_same(std::initializer_list<T *>) : value{true}
        {
        }
    };
    template <typename Sequence, typename Predicate>
    struct all_impl : bool_<true>
    {
    };
    template <template <class...> class Sequence, typename Predicate, typename T, typename... Ts>
    struct all_impl<Sequence<T, Ts...>, Predicate>
    {
        static constexpr all_same tester{
            static_cast< ::brigand::apply<Predicate, T> *>(nullptr),
            static_cast< ::brigand::apply<Predicate, Ts> *>(nullptr)...};
        using type = bool_<(::brigand::apply<Predicate, T>::value != 0 && tester.value)>;
    };
    template <template <class...> class Sequence, template <typename...> class F, typename T,
              typename... Ts>
    struct all_impl<Sequence<T, Ts...>, bind<F, _1>>
    {
        static constexpr all_same tester{static_cast<F<T> *>(nullptr),
                                         static_cast<F<Ts> *>(nullptr)...};
        using type = bool_<(F<T>::value != 0 && tester.value)>;
    };
    template <template <class...> class Sequence, template <typename...> class F, typename T,
              typename... Ts>
    struct all_impl<Sequence<T, Ts...>, F<_1>>
    {
        static constexpr all_same tester{static_cast<typename F<T>::type *>(nullptr),
                                         static_cast<typename F<Ts>::type *>(nullptr)...};
        using type = bool_<(F<T>::type::value != 0 && tester.value)>;
    };
}
#endif
template <typename Sequence, typename Predicate = detail::non_null>
using all = typename detail::all_impl<Sequence, Predicate>::type;
}
namespace brigand
{
#if defined(BRIGAND_COMP_MSVC_2013) || defined(BRIGAND_COMP_CUDA) || defined(BRIGAND_COMP_INTEL)
namespace detail
{
    template <typename Sequence, typename Pred>
    struct none_impl
    {
        template <typename T>
        struct nope
        {
            using that = brigand::apply<Pred, T>;
            using type = bool_<!that::value>;
        };
        using type = all<Sequence, nope<_1>>;
    };
}
#else
namespace detail
{
    template <typename Sequence, typename Predicate>
    struct none_impl : bool_<true>
    {
    };
    template <template <class...> class Sequence, typename Predicate, typename T, typename... Ts>
    struct none_impl<Sequence<T, Ts...>, Predicate>
    {
        static constexpr all_same tester{
            static_cast< ::brigand::apply<Predicate, T> *>(nullptr),
            static_cast< ::brigand::apply<Predicate, Ts> *>(nullptr)...};
        using type = bool_<(::brigand::apply<Predicate, T>::value == 0 && tester.value)>;
    };
    template <template <class...> class Sequence, template <typename...> class F, typename T,
              typename... Ts>
    struct none_impl<Sequence<T, Ts...>, bind<F, _1>>
    {
        static constexpr all_same tester{static_cast<F<T> *>(nullptr),
                                         static_cast<F<Ts> *>(nullptr)...};
        using type = bool_<(F<T>::value == 0 && tester.value)>;
    };
    template <template <class...> class Sequence, template <typename...> class F, typename T,
              typename... Ts>
    struct none_impl<Sequence<T, Ts...>, F<_1>>
    {
        static constexpr all_same tester{static_cast<typename F<T>::type *>(nullptr),
                                         static_cast<typename F<Ts>::type *>(nullptr)...};
        using type = bool_<(F<T>::type::value == 0 && tester.value)>;
    };
}
#endif
template <typename Sequence, typename Predicate = detail::non_null>
using none = typename detail::none_impl<Sequence, Predicate>::type;
}
namespace brigand
{
  namespace detail
  {
    template< typename Sequence, typename Predicate >
    struct any_impl : bool_<!none<Sequence,Predicate>::value> {};
  }
  template<typename Sequence, typename Predicate = detail::non_null>
  using any = typename detail::any_impl<Sequence,Predicate>::type;
}
namespace brigand
{
namespace detail
{
    template <template <typename...> class S, template <typename...> class F, typename... Ts>
    struct finder
    {
        template <typename T>
        using P = F<Ts..., T>;
        template <bool InNext8, bool Match, typename... Ls>
        struct find
        {
            using type = S<>;
        };
        template <typename L>
        struct find<true, false, L>
        {
            using type = S<>;
        };
        template <typename L, typename... Ls>
        struct find<true, true, L, Ls...>
        {
            using type = S<L, Ls...>;
        };
        template <typename L1, typename L2, typename... Ls>
        struct find<true, false, L1, L2, Ls...> : find<true, F<Ts..., L2>::value, L2, Ls...>
        {
        };
        template <typename L0, typename L1, typename L2, typename L3, typename L4, typename L5,
                  typename L6, typename L7, typename L8,
                  typename... Ls>
        struct find<false, false, L0, L1, L2, L3, L4, L5, L6, L7, L8, Ls...>
            : find<true, F<Ts..., L8>::value, L8, Ls...>
        {
        };
        template <typename L1, typename L2, typename L3, typename L4, typename L5, typename L6,
                  typename L7, typename L8, typename L9, typename L10, typename L11, typename L12,
                  typename L13, typename L14, typename L15, typename L16,
                  typename... Ls>
        struct find<false, false, L1, L2, L3, L4, L5, L6, L7, L8, L9, L10, L11, L12, L13, L14, L15,
                    L16, Ls...>
            : find<(P<L9>::value || P<L10>::value || P<L11>::value || P<L12>::value ||
                    P<L13>::value || P<L14>::value || P<L15>::value || P<L16>::value),
                   P<L9>::value, L9, L10, L11, L12, L13, L14, L15, L16, Ls...>
        {
        };
    };
}
}
namespace brigand
{
namespace detail
{
    template <template<class...> class L, class...>
    struct reverse_elements;
    template <template <class...> class L>
    struct reverse_elements<L>
    {
        using type = L<>;
    };
    template <template <class...> class L, class T0, class... Ts>
    struct reverse_elements<L, T0, Ts...>
    : append_impl<typename reverse_elements<L, Ts...>::type, L<T0>>
    {
    };
    template <template <class...> class L, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class... Ts>
    struct reverse_elements<L, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, Ts...>
    : append_impl<typename reverse_elements<L, Ts...>::type, L<T15, T14, T13, T12, T11, T10, T9, T8, T7, T6, T5, T4, T3, T2, T1, T0>>
    {
    };
    template<class L>
    struct reverse_impl;
    template<template<class...> class L, class... U>
    struct reverse_impl<L<U...>>
    : reverse_elements<L, U...>
    {
    };
}
namespace lazy
{
    template <typename L>
    using reverse = typename detail::reverse_impl<L>;
}
    template <typename L>
    using reverse = typename detail::reverse_impl<L>::type;
}
namespace brigand
{
namespace lazy
{
    template <typename Sequence, typename Predicate = ::brigand::detail::non_null>
    struct find;
    template <template <typename...> class Sequence, typename... Ls, typename Pred>
    struct find<Sequence<Ls...>, Pred>
        : detail::finder<Sequence, detail::bound_apply, Pred>::template find<
              false, false, void, void, void, void, void, void, void, void, Ls...>
    {
    };
    template <template <typename...> class Sequence, typename... Ls, template <typename...> class F>
    struct find<Sequence<Ls...>, bind<F, _1>>
        : detail::finder<Sequence, F>::template find<false, false, void, void, void, void, void,
                                                     void, void, void, Ls...>
    {
    };
}
template <typename Sequence, typename Predicate = brigand::detail::non_null>
using find = typename lazy::find<Sequence, Predicate>::type;
namespace lazy
{
    template <typename Sequence, typename Predicate = detail::non_null>
    using reverse_find =
        ::brigand::lazy::reverse< ::brigand::find<brigand::reverse<Sequence>, Predicate>>;
}
template <typename Sequence, typename Predicate = detail::non_null>
using reverse_find = typename ::brigand::lazy::reverse_find<Sequence, Predicate>::type;
namespace detail
{
    template <typename Sequence, typename Predicate>
    using find_size = size<brigand::find<Sequence, Predicate>>;
    template <typename Sequence, typename Predicate>
    using empty_find = bool_<find_size<Sequence, Predicate>::value == 0>;
    template <typename Sequence, typename Predicate>
    using non_empty_find = bool_<find_size<Sequence, Predicate>::value != 0>;
}
template <typename Sequence, typename Predicate = detail::non_null>
using not_found = typename detail::empty_find<Sequence, Predicate>;
template <typename Sequence, typename Predicate = detail::non_null>
using found = typename detail::non_empty_find<Sequence, Predicate>;
}
namespace brigand
{
namespace detail
{
    template <class L>
    struct flatten_impl
    {
        using type = L;
    };
    template <template<class...> class L, class T>
    struct flatten_element_impl
    {
        using type = L<T>;
    };
    template <template<class...> class L, class... Ts>
    struct flatten_element_impl<L, L<Ts...>>
    : append_impl<typename flatten_element_impl<L, Ts>::type...>
    {
    };
    template <template<class...> class L, class... Ts>
    struct flatten_impl<L<Ts...>>
    : flatten_element_impl<L, L<Ts...>>
    {
    };
}
namespace lazy
{
    template <typename Sequence>
    using flatten = typename detail::flatten_impl<Sequence>;
}
template <typename Sequence>
using flatten = typename lazy::flatten<Sequence>::type;
}
namespace brigand { namespace detail
{
  template<class Functor, class State, class Sequence>
  struct fold_impl
  {
      using type = State;
  };
  template<
      class Functor, class State, template <class...> class Sequence,
      class T0>
  struct fold_impl<Functor, State, Sequence<T0>>
  {
      using type = brigand::apply<Functor, State, T0>;
  };
  template<
      class Functor, class State, template <class...> class Sequence,
      class T0, class T1>
  struct fold_impl<Functor, State, Sequence<T0, T1>>
  {
      using type = brigand::apply<Functor,
          brigand::apply<Functor,State, T0>, T1
      >;
  };
  template<
      class Functor, class State, template <class...> class Sequence,
      class T0, class T1, class T2>
  struct fold_impl<Functor, State, Sequence<T0, T1, T2>>
  {
      using type = brigand::apply<Functor,
          brigand::apply<Functor,
              brigand::apply<Functor, State, T0>, T1
          >, T2
      >;
  };
  template<
      class Functor, class State, template <class...> class Sequence,
      class T0, class T1, class T2, class T3>
  struct fold_impl<Functor, State, Sequence<T0, T1, T2, T3>>
  {
      using type = brigand::apply<Functor,
          brigand::apply<Functor, 
              brigand::apply<Functor,
                  brigand::apply<Functor, State, T0>, T1
              >, T2
          >, T3
      >;
  };
  template<
      class Functor, class State, template <class...> class Sequence,
      class T0, class T1, class T2, class T3, class T4>
  struct fold_impl<Functor, State, Sequence<T0, T1, T2, T3, T4>>
  {
      using type = brigand::apply<Functor,
          brigand::apply<Functor,
              brigand::apply<Functor,
                  brigand::apply<Functor,
                      brigand::apply<Functor, State, T0>, T1
                  >, T2
              >, T3
          >, T4
      >;
  };
  template<
      class Functor, class State, template <class...> class Sequence,
      class T0, class T1, class T2, class T3, class T4, class T5>
  struct fold_impl<Functor, State, Sequence<T0, T1, T2, T3, T4, T5>>
  {
      using type = brigand::apply<Functor,
          brigand::apply<Functor,
              brigand::apply<Functor,
                  brigand::apply<Functor,
                      brigand::apply<Functor,
                          brigand::apply<Functor, State, T0>, T1
                      >, T2
                  >, T3
              >, T4
          >, T5
      >;
  };
  template<
      class Functor, class State, template <class...> class Sequence,
      class T0, class T1, class T2, class T3, class T4, class T5, class T6>
  struct fold_impl<Functor, State, Sequence<T0, T1, T2, T3, T4, T5, T6>>
  {
      using type = brigand::apply<Functor,
          brigand::apply<Functor,
              brigand::apply<Functor,
                  brigand::apply<Functor,
                      brigand::apply<Functor,
                          brigand::apply<Functor,
                              brigand::apply<Functor, State, T0>, T1
                          >, T2
                      >, T3
                  >, T4
              >, T5
          >, T6
      >;
  };
  template<
      class Functor, class State, template <class...> class Sequence,
      class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
  struct fold_impl<Functor, State, Sequence<T0, T1, T2, T3, T4, T5, T6, T7>>
  {
      using type = brigand::apply<Functor,
          brigand::apply<Functor,
              brigand::apply<Functor,
                  brigand::apply<Functor,
                      brigand::apply<Functor,
                          brigand::apply<Functor,
                              brigand::apply<Functor,
                                  brigand::apply<Functor, State, T0>, T1
                              >, T2
                          >, T3
                      >, T4
                  >, T5
              >, T6
          >, T7
      >;
  };
  template<
      class Functor, class State, template <class...> class Sequence,
      class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class... T>
  struct fold_impl<Functor, State, Sequence<T0, T1, T2, T3, T4, T5, T6, T7, T...>>
  : fold_impl<
      Functor,
      brigand::apply<Functor,
          brigand::apply<Functor,
              brigand::apply<Functor,
                  brigand::apply<Functor,
                      brigand::apply<Functor,
                          brigand::apply<Functor,
                              brigand::apply<Functor,
                                  brigand::apply<Functor,
                                      State, T0
                                  >, T1
                              >, T2
                          >, T3
                      >, T4
                  >, T5
              >, T6
          >, T7
      >,
      Sequence<T...>
  >
  {};
  template<typename Functor, typename State, typename Sequence>
  struct reverse_fold_impl
  {
    using type = State;
  };
  template <typename Functor, typename State, template <typename...> class L, typename T, typename... Ts>
  struct reverse_fold_impl<Functor, State, L<T, Ts...>>
  {
      using type =
          brigand::apply<Functor, typename reverse_fold_impl<Functor, State, L<Ts...>>::type, T>;
  };
  template<
    typename Functor, typename State, template <typename...> class L,
    typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename... Ts>
    struct reverse_fold_impl<Functor, State, L<T0, T1, T2, T3, T4, T5, T6, T7, Ts...>>{
    using type = brigand::apply<Functor,
      brigand::apply<Functor,
      brigand::apply<Functor,
      brigand::apply<Functor,
      brigand::apply<Functor,
      brigand::apply<Functor,
      brigand::apply<Functor,
      brigand::apply<Functor,
      typename reverse_fold_impl<Functor, State, L<Ts...>>::type, T7
      >, T6
      >, T5
      >, T4
      >, T3
      >, T2
      >, T1
      >, T0
      >;
  };
} }
namespace brigand
{
namespace lazy
{
    template <class Sequence, class State, class Functor>
    using fold = typename detail::fold_impl<Functor, State, Sequence>;
    template <class Sequence, class State, class Functor>
    using reverse_fold = typename detail::reverse_fold_impl<Functor, State, Sequence>;
}
template <class Sequence, class State, class Functor>
using fold = typename ::brigand::lazy::fold<Sequence, State, Functor>::type;
template <class Sequence, class State, class Functor>
using reverse_fold = typename ::brigand::lazy::reverse_fold<Sequence, State, Functor>::type;
}


// namespace brigand
// {
//   template<class F, class...Ts> F for_each_args(F f, Ts&&...a)
//   {
//     return (void)std::initializer_list<int>{((void)std::ref(f)(static_cast<Ts&&>(a)),0)...}, f;
//     //return (void)std::initializer_list<int>{((void)f(static_cast<Ts&&>(a)),0)...}, f;
//   }
// }
namespace brigand
{
  namespace detail
  {
    template<template<class...> class List, typename... Elements, typename Functor>
    Functor for_each_impl( List<Elements...>&&, Functor f )
    {
      return for_each_args( f, type_<Elements>()... );
    }
  }
  template<typename List, typename Functor> Functor for_each( Functor f )
  {
    return detail::for_each_impl( List{}, f );
  }
}
namespace brigand
{
namespace detail
{
    template <bool Found, class Sequence, typename Predicate, typename NotFoundType>
    struct index_if_impl
    {
        using type = ::brigand::size_t<size<Sequence>::value -
                                       size< ::brigand::find<Sequence, Predicate>>::value>;
    };
    template <class Sequence, typename Predicate, typename NotFoundType>
    struct index_if_impl<false, Sequence, Predicate, NotFoundType>
    {
        using type = NotFoundType;
    };
}
template <class Sequence, class Predicate, class NotFoundType = no_such_type_>
using index_if = typename detail::index_if_impl< ::brigand::found<Sequence, Predicate>::value,
                                                Sequence, Predicate, NotFoundType>::type;
template <class Sequence, typename T>
using index_of = index_if<Sequence, std::is_same<typename brigand::pin<T>, ::brigand::_1>>;
}
namespace brigand
{
namespace detail
{
    template <class T, class, class, T>
    struct range_cat;
#ifdef BRIGAND_COMP_MSVC
    template <class T, T Start, T Int>
    struct int_plus
    {
        using type = brigand::integral_constant<T, Start + Int>;
    };
#endif
    template <class T, class... Ts, T... Ints, T Start>
    struct range_cat<T, list<Ts...>, list<brigand::integral_constant<T, Ints>...>, Start>
    {
#ifdef BRIGAND_COMP_MSVC
        using type = list<Ts..., typename int_plus<T, Start, Ints>::type...>;
#else
        using type = list<Ts..., brigand::integral_constant<T, Start + Ints>...>;
#endif
    };
    template <class T, T Start, unsigned int N>
    struct range_impl : range_cat<T, typename range_impl<T, Start, N / 2>::type,
                                  typename range_impl<T, Start, N - N / 2>::type, N / 2>
    {
    };
    template <class T, T Start>
    struct range_impl<T, Start, 1>
    {
        using type = list<brigand::integral_constant<T, Start>>;
    };
    template <class T, T Start>
    struct range_impl<T, Start, 0>
    {
        using type = list<>;
    };
    template <class T, class, class, T>
    struct reverse_range_cat;
#ifdef BRIGAND_COMP_MSVC
    template <class T, T Start, T Int>
    struct int_minus
    {
        using type = brigand::integral_constant<T, Int - Start>;
    };
#endif
    template <class T, class... Ts, T... Ints, T Start>
    struct reverse_range_cat<T, list<Ts...>, list<brigand::integral_constant<T, Ints>...>, Start>
    {
#ifdef BRIGAND_COMP_MSVC
        using type = list<Ts..., typename int_minus<T, Start, Ints>::type...>;
#else
        using type = list<Ts..., brigand::integral_constant<T, Ints - Start>...>;
#endif
    };
    template <class T, T Start, unsigned int N>
    struct reverse_range_impl
        : reverse_range_cat<T, typename reverse_range_impl<T, Start, N / 2>::type,
                            typename reverse_range_impl<T, Start, N - N / 2>::type, N / 2>
    {
    };
    template <class T, T Start>
    struct reverse_range_impl<T, Start, 1>
    {
        using type = list<brigand::integral_constant<T, Start>>;
    };
    template <class T, T Start>
    struct reverse_range_impl<T, Start, 0>
    {
        using type = list<>;
    };
    template <class T, T Start, T Stop>
    struct reverse_range_safe
    {
        static_assert(
            Start >= Stop,
            "Invalid parameters. reverse_range<> syntax is reverse_range<type, from, down_to>");
        using type = typename reverse_range_impl<T, Start, Start - Stop>::type;
    };
}
template <class T, T Start, T Stop>
using range = typename detail::range_impl<T, Start, Stop - Start>::type;
template <class T, T Start, T Stop>
using reverse_range = typename detail::reverse_range_safe<T, Start, Stop>::type;
}
namespace brigand
{
#ifdef BRIGAND_COMP_MSVC
namespace detail
{
    template <class, class T>
    struct unique_x_t
    {
        operator T();
    };
    template <class Ints, class... Ts>
    struct is_set_impl;
    template <>
    struct is_set_impl<list<>>
    {
        using type = brigand::true_type;
    };
    inline brigand::true_type true_fn(...);
    template <class... Ints, class... Ts>
    struct is_set_impl<list<Ints...>, Ts...>
    {
        struct Pack : unique_x_t<Ints, Ts>...
        {
        };
        template <class... Us>
        static auto is_set(Us...) -> decltype(true_fn(static_cast<Us>(Pack())...));
        static brigand::false_type is_set(...);
        using type = decltype(is_set(Ts()...));
    };
    template <class>
    struct qrref
    {
    };
    template <class>
    struct qref
    {
    };
    template <class>
    struct qnoref
    {
    };
    template <class T>
    struct msvc_quali_ref
    {
        using type = qnoref<T>;
    };
    template <class T>
    struct msvc_quali_ref<T &>
    {
        using type = qref<T>;
    };
    template <class T>
    struct msvc_quali_ref<T &&>
    {
        using type = qrref<T>;
    };
}
template <class... Ts>
using is_set = typename detail::is_set_impl<range<unsigned int, 0, sizeof...(Ts)>,
                                            detail::msvc_quali_ref<Ts>...>::type;
#else
namespace detail
{
    template <class, class T>
    struct unique_x_t : type_<T>
    {
    };
    template <class Ints, class... Ts>
    struct is_set_cont;
    template <class... Ints, class... Ts>
    struct is_set_cont<list<Ints...>, Ts...> : unique_x_t<Ints, Ts>...
    {
    };
}
template <class... Ts>
using is_set = bool_<sizeof(detail::is_set_cont<range<unsigned int, 0, sizeof...(Ts)>, Ts...>) == 1>;
#endif
}

namespace brigand
{
#if defined(BRIGAND_COMP_GCC) || defined(BRIGAND_COMP_CLANG)
namespace lazy
{
    template <typename L, typename Pred>
    struct remove_if;
    template <template <class...> class L, typename... Ts, typename Pred>
    struct remove_if<L<Ts...>, Pred>
        : ::brigand::detail::append_impl<
              L<>, typename std::conditional< ::brigand::apply<Pred, Ts>::value, list<>,
                                             list<Ts>>::type...>
    {
    };
    template <template <class...> class L, typename... Ts, template <typename...> class F>
    struct remove_if<L<Ts...>, bind<F, _1>>
        : ::brigand::detail::append_impl<
              L<>, typename std::conditional<F<Ts>::value, list<>, list<Ts>>::type...>
    {
    };
    template <template <class...> class L, typename... Ts, template <typename...> class F>
    struct remove_if<L<Ts...>, F<_1>>
        : ::brigand::detail::append_impl<
              L<>, typename std::conditional<F<Ts>::type::value, list<>, list<Ts>>::type...>
    {
    };
}
namespace lazy
{
    template <typename L, typename T>
    struct remove;
    template <template <class...> class L, typename... Ts, typename T>
    struct remove<L<Ts...>, T>
        : ::brigand::detail::append_impl<
              L<>, typename std::conditional<std::is_same<Ts, T>::value, list<>, list<Ts>>::type...>
    {
    };
}
namespace lazy
{
    template <typename L, typename Pred>
    struct filter;
    template <template <class...> class L, typename... Ts, typename Pred>
    struct filter<L<Ts...>, Pred>
        : ::brigand::detail::append_impl<
              L<>, typename std::conditional< ::brigand::apply<Pred, Ts>::value, list<Ts>,
                                             list<>>::type...>
    {
    };
    template <template <class...> class L, typename... Ts, template <typename...> class F>
    struct filter<L<Ts...>, bind<F, _1>>
        : ::brigand::detail::append_impl<
              L<>, typename std::conditional<F<Ts>::value, list<Ts>, list<>>::type...>
    {
    };
    template <template <class...> class L, typename... Ts, template <typename...> class F>
    struct filter<L<Ts...>, F<_1>>
        : ::brigand::detail::append_impl<
              L<>, typename std::conditional<F<Ts>::type::value, list<Ts>, list<>>::type...>
    {
    };
}
#else
namespace detail
{
    template <class Pred, class T>
    struct empty_helper : ::brigand::apply<Pred, T>
    {
    };
    template <typename Pred, typename T, bool B>
    struct empty_if_true : std::conditional<empty_helper<Pred, T>::value == B, list<>, list<T>>
    {
    };
    template <template <typename...> class F, typename T, bool B>
    struct empty_if_true<bind<F, _1>, T, B> : std::conditional<F<T>::value == B, list<>, list<T>>
    {
    };
    template <template <typename...> class F, typename T, bool B>
    struct empty_if_true<F<_1>, T, B> : std::conditional<F<T>::type::value == B, list<>, list<T>>
    {
    };
}
namespace lazy
{
    template <typename L, typename Pred>
    struct remove_if;
    template <template <class...> class L, typename... Ts, typename Pred>
    struct remove_if<L<Ts...>, Pred>
        : ::brigand::detail::append_impl<
              L<>, typename ::brigand::detail::empty_if_true<Pred, Ts, true>::type...>
    {
    };
}
namespace lazy
{
    template <typename L, typename T>
    struct remove;
    template <template <class...> class L, typename... Ts, typename T>
    struct remove<L<Ts...>, T>
        : ::brigand::detail::append_impl<
              L<>, typename std::conditional<std::is_same<Ts, T>::value, list<>, list<Ts>>::type...>
    {
    };
}
namespace lazy
{
    template <typename L, typename Pred>
    struct filter;
    template <template <class...> class L, typename... Ts, typename Pred>
    struct filter<L<Ts...>, Pred>
        : ::brigand::detail::append_impl<
              L<>, typename ::brigand::detail::empty_if_true<Pred, Ts, false>::type...>
    {
    };
}
#endif
template <typename L, typename Pred>
using remove_if = typename lazy::remove_if<L, Pred>::type;
template <typename L, typename T>
using remove = typename lazy::remove<L, T>::type;
template <typename L, typename Pred>
using filter = typename lazy::filter<L, Pred>::type;
}
namespace brigand
{
template <class Seq, class Pred>
using partition = pair<filter<Seq, Pred>, remove_if<Seq, Pred>>;
}

namespace brigand
{
namespace detail
{
    template <typename Pred, typename T>
    struct rep_helper
    {
        using type = brigand::apply<Pred, T>;
    };
    template <typename T, typename Pred, typename NewType>
    struct replacer : std::conditional<rep_helper<Pred, T>::type::value, NewType, T>
    {
    };
    template <typename T, template <typename...> class F, typename NewType>
    struct replacer<T, bind<F, _1>, NewType> : std::conditional<F<T>::value, NewType, T>
    {
    };
    template <typename T, template <typename...> class F, typename NewType>
    struct replacer<T, F<_1>, NewType> : std::conditional<F<T>::type::value, NewType, T>
    {
    };
}
namespace lazy
{
    template <typename Sequence, typename Predicate, typename NewType>
    struct replace_if;
    template <template <typename...> class S, typename... Ts, typename Predicate, typename NewType>
    struct replace_if<S<Ts...>, Predicate, NewType>
    {
        using type = S<typename detail::replacer<Ts, Predicate, NewType>::type...>;
    };
    template <typename Sequence, typename OldType, typename NewType>
    using replace = replace_if<Sequence, std::is_same<_1, pin<OldType>>, NewType>;
}
template <typename Sequence, typename Predicate, typename NewType>
using replace_if = typename ::brigand::lazy::replace_if<Sequence, Predicate, NewType>::type;
template <typename Sequence, typename OldType, typename NewType>
using replace = typename ::brigand::lazy::replace<Sequence, OldType, NewType>::type;
}

namespace brigand
{
  template<typename C, typename T, typename F>
  inline typename std::enable_if<C::value,T&&>::type select(T&& t, F&&)
  {
    return std::forward<T>(t);
  }
  template<typename C, typename T, typename F>
  inline typename std::enable_if<!C::value,F&&>::type select(T&&, F&& f)
  {
    return std::forward<F>(f);
  }
}
namespace brigand
{
namespace detail
{
    template<typename TOut, typename TCurrent, typename TDelim, typename... Ts>
    struct split_impl;
    template<template<typename...> class L, typename... Os, typename... Cs, typename TDelim, typename T, typename... Ts>
    struct split_impl<L<Os...>, L<Cs...>, TDelim, T, Ts...> :
        split_impl<L<Os...>, L<Cs..., T>, TDelim, Ts...> {};
    template<template<typename...> class L, typename... Os, typename... Cs, typename TDelim, typename T>
    struct split_impl<L<Os...>, L<Cs...>, TDelim, T> {
        using type = L<Os..., L<Cs..., T>>;
    };
    template<template<typename...> class L, typename... Os, typename... Cs, typename TDelim, typename... Ts>
    struct split_impl<L<Os...>, L<Cs...>, TDelim, TDelim, Ts...> :
        split_impl<L<Os..., L<Cs...>>, L<>, TDelim, Ts...> {};
    template<template<typename...> class L, typename... Os, typename... Cs, typename TDelim>
    struct split_impl<L<Os...>, L<Cs...>, TDelim, TDelim> {
        using type = L<Os..., L<Cs...>>;
    };
    template<template<typename...> class L, typename... Os, typename TDelim, typename... Ts>
    struct split_impl<L<Os...>, L<>, TDelim, TDelim, Ts...> :
        split_impl<L<Os...>, L<>, TDelim, Ts...> {};
    template<template<typename...> class L, typename... Os, typename TDelim>
    struct split_impl<L<Os...>, L<>, TDelim, TDelim> {
        using type = L<Os...>;
    };
    template<template<typename...> class L, typename... Os, typename TDelim>
    struct split_impl<L<Os...>, L<>, TDelim> {
        using type = L<Os...>;
    };
    template<typename TList, typename TDelim>
    struct split_helper;
    template<template<typename...> class L, typename T, typename... Ts, typename TDelim>
    struct split_helper<L<T,Ts...>, TDelim> : split_impl<L<>, L<>, TDelim, T, Ts...>{};
#if defined(BRIGAND_COMP_INTEL)
    template<template<typename...> class L, typename TDelim>
    struct split_helper<L<>, TDelim> {
        using type = L<>;
    };
#else
    template<template<typename...> class L, typename... T, typename TDelim>
    struct split_helper<L<T...>, TDelim> {
        using type = L<>;
    };
#endif
}
namespace lazy
{
    template<typename TList, typename TDelim>
    using split = detail::split_helper<TList, TDelim>;
}
template<typename TList, typename TDelim>
using split = typename lazy::split<TList, TDelim>::type;
}
namespace brigand
{
  template <typename A, typename B>
  struct less : bool_ < (A::value < B::value) > {};
}

namespace brigand
{
namespace detail
{
    template <class Comp, class T1, class U>
    struct merge_helper : ::brigand::apply<Comp, T1, U>
    {
    };
    template <class L, class Seq1, class Seq2, class Comp>
    struct merge_impl;
    template <bool, class L, class Seq1, class Seq2, class Comp>
    struct merge_insert;
    template <class... R, class T0, class T1, class... Ts, class U, class... Us, class Comp>
    struct merge_insert<true, list<R...>, list<T0, T1, Ts...>, list<U, Us...>, Comp>
        : merge_insert<merge_helper<Comp, T1, U>::value, list<R..., T0>, list<T1, Ts...>,
                       list<U, Us...>, Comp>
    {
    };
    template <class... R, class T, class U, class... Us, class Comp>
    struct merge_insert<true, list<R...>, list<T>, list<U, Us...>, Comp>
    {
        using list = ::brigand::list<R..., T>;
        using left = ::brigand::list<>;
        using right = ::brigand::list<U, Us...>;
    };
    template <class... R, class T, class... Ts, class U0, class U1, class... Us, class Comp>
    struct merge_insert<false, list<R...>, list<T, Ts...>, list<U0, U1, Us...>, Comp>
        : merge_insert<merge_helper<Comp, T, U1>::value, list<R..., U0>, list<T, Ts...>,
                       list<U1, Us...>, Comp>
    {
    };
    template <class... R, class T, class... Ts, class U, class Comp>
    struct merge_insert<false, list<R...>, list<T, Ts...>, list<U>, Comp>
    {
        using list = ::brigand::list<R..., U>;
        using left = ::brigand::list<T, Ts...>;
        using right = ::brigand::list<>;
    };
    template <class... R, class T0, class T1, class T2, class T3, class T4, class T5, class T6,
              class T7, class T8, class T9, class... Ts, class U0, class U1, class U2, class U3,
              class U4, class U5, class U6, class U7, class U8, class U9, class... Us, class Comp>
    struct merge_impl<list<R...>, list<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, Ts...>,
                      list<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, Us...>, Comp>
    {
        using sub = merge_insert<merge_helper<Comp, T0, U0>::value, list<>,
                                 list<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>,
                                 list<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9>, Comp>;
        using type = typename merge_impl<append<list<R...>, typename sub::list>,
                                         append<typename sub::left, list<Ts...>>,
                                         append<typename sub::right, list<Us...>>, Comp>::type;
    };
    template <class... R, class T, class... Ts, class U, class... Us, class Comp>
    struct merge_impl<list<R...>, list<T, Ts...>, list<U, Us...>, Comp>
        : std::conditional<merge_helper<Comp, T, U>::value,
                           merge_impl<list<R..., T>, list<Ts...>, list<U, Us...>, Comp>,
                           merge_impl<list<R..., U>, list<T, Ts...>, list<Us...>, Comp>>::type
    {
    };
    template <class... R, class... Ts, class Comp>
    struct merge_impl<list<R...>, list<Ts...>, list<>, Comp>
    {
        using type = list<R..., Ts...>;
    };
    template <class... R, class... Us, class Comp>
    struct merge_impl<list<R...>, list<>, list<Us...>, Comp>
    {
        using type = list<R..., Us...>;
    };
    template <class... R, class Comp>
    struct merge_impl<list<R...>, list<>, list<>, Comp>
    {
        using type = list<R...>;
    };
}
template <class Seq1, class Seq2, class Comp = less<_1, _2>>
using merge =
    append<clear<Seq1>,
           typename detail::merge_impl<list<>, wrap<Seq1, list>, wrap<Seq2, list>, Comp>::type>;
}
namespace brigand
{
namespace detail
{
    template <class Ls, class Seq, typename Comp>
    struct sort_impl;
    template <class L, class Comp>
    struct mini_sort;
    template <class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7,
              class T8, class... Ts, class Comp>
    struct mini_sort<list<T0, T1, T2, T3, T4, T5, T6, T7, T8, Ts...>, Comp>
        : merge_impl<list<>, typename mini_sort<list<T0, T1, T2, T3, T4, T5, T6, T7>, Comp>::type,
                     typename mini_sort<list<T8, Ts...>, Comp>::type, Comp>
    {
    };
    template <class T0, class T1, class T2, class T3, class T4, class... Ts, class Comp>
    struct mini_sort<list<T0, T1, T2, T3, T4, Ts...>, Comp>
        : merge_impl<list<>, typename mini_sort<list<T0, T1, T2, T3>, Comp>::type,
                     typename mini_sort<list<T4, Ts...>, Comp>::type, Comp>
    {
    };
    template <class T0, class T1, class T2, class T3, class Comp>
    struct mini_sort<list<T0, T1, T2, T3>, Comp>
        : merge_impl<list<>, typename mini_sort<list<T0, T1>, Comp>::type,
                     typename mini_sort<list<T2, T3>, Comp>::type, Comp>
    {
    };
    template <class T0, class T1, class T2, class Comp>
    struct mini_sort<list<T0, T1, T2>, Comp>
        : merge_impl<list<>, typename mini_sort<list<T0, T1>, Comp>::type, list<T2>, Comp>
    {
    };
    template <class T0, class T1, class Comp>
    struct mini_sort<list<T0, T1>, Comp>
    {
        using base = ::brigand::apply<Comp, T0, T1>;
        using type = typename std::conditional<base::value, list<T0, T1>, list<T1, T0>>::type;
    };
    template <class T0, class Comp>
    struct mini_sort<list<T0>, Comp>
    {
        using type = list<T0>;
    };
    template <class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7,
              class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15,
              class T16, class T17, class... Ts, typename Comp>
    struct sort_impl<list<>, list<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
                                  T15, T16, T17, Ts...>,
                     Comp>
        : sort_impl<list<typename mini_sort<list<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
                                                 T12, T13, T14, T15, T16, T17>,
                                            Comp>::type>,
                    list<Ts...>, Comp>
    {
    };
    template <class L0, class T0, class T1, class T2, class T3, class T4, class T5, class T6,
              class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14,
              class T15, class T16, class T17, class... Ts, typename Comp>
    struct sort_impl<list<L0>, list<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
                                    T15, T16, T17, Ts...>,
                     Comp>
        : sort_impl<list<L0, typename mini_sort<list<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10,
                                                     T11, T12, T13, T14, T15, T16, T17>,
                                                Comp>::type>,
                    list<Ts...>, Comp>
    {
    };
    template <class L0, class L1, class T0, class T1, class T2, class T3, class T4, class T5,
              class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13,
              class T14, class T15, class T16, class T17, class... Ts, typename Comp>
    struct sort_impl<list<L0, L1>, list<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
                                        T14, T15, T16, T17, Ts...>,
                     Comp>
        : sort_impl<list<L0, L1, typename mini_sort<list<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9,
                                                         T10, T11, T12, T13, T14, T15, T16, T17>,
                                                    Comp>::type>,
                    list<Ts...>, Comp>
    {
    };
    template <class L0, class L1, class L2, class T0, class T1, class T2, class T3, class T4,
              class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12,
              class T13, class T14, class T15, class T16, class T17, class... Ts, typename Comp>
    struct sort_impl<list<L0, L1, L2>, list<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
                                            T13, T14, T15, T16, T17, Ts...>,
                     Comp>
        : sort_impl<list<merge<L0, L1, Comp>,
                         merge<typename mini_sort<list<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10,
                                                       T11, T12, T13, T14, T15, T16, T17>,
                                                  Comp>::type,
                               L2, Comp>>,
                    list<Ts...>, Comp>
    {
    };
    template <class T, class... Ts, typename Comp>
    struct sort_impl<list<>, list<T, Ts...>, Comp>
    {
        using type = typename mini_sort<list<T, Ts...>, Comp>::type;
    };
    template <class L, class T, class... Ts, typename Comp>
    struct sort_impl<list<L>, list<T, Ts...>, Comp>
    {
        using type = merge<typename mini_sort<list<T, Ts...>, Comp>::type, L, Comp>;
    };
    template <class L0, class L1, class T, class... Ts, typename Comp>
    struct sort_impl<list<L0, L1>, list<T, Ts...>, Comp>
    {
        using type =
            merge<L0, merge<typename mini_sort<list<T, Ts...>, Comp>::type, L1, Comp>, Comp>;
    };
    template <class L0, class L1, class L2, class T, class... Ts, typename Comp>
    struct sort_impl<list<L0, L1, L2>, list<T, Ts...>, Comp>
    {
        using type = merge<merge<L0, L1, Comp>,
                           merge<typename mini_sort<list<T, Ts...>, Comp>::type, L2, Comp>, Comp>;
    };
    template <class L, typename Comp>
    struct sort_impl<list<L>, list<>, Comp>
    {
        using type = L;
    };
    template <class L0, class L1, typename Comp>
    struct sort_impl<list<L0, L1>, list<>, Comp>
    {
        using type = merge<L0, L1, Comp>;
    };
    template <class L0, class L1, class L2, typename Comp>
    struct sort_impl<list<L0, L1, L2>, list<>, Comp>
    {
        using type = merge<merge<L0, L1, Comp>, L2, Comp>;
    };
    template <typename Comp>
    struct sort_impl<list<>, list<>, Comp>
    {
        using type = list<>;
    };
    template <
        class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8,
        class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16,
        class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24,
        class T25, class T26, class T27, class T28, class T29, class T30, class T31, class T32,
        class T33, class T34, class T35, class T36, class T37, class T38, class T39, class T40,
        class T41, class T42, class T43, class T44, class T45, class T46, class T47, class T48,
        class T49, class T50, class T51, class T52, class T53, class T54, class T55, class T56,
        class T57, class T58, class T59, class T60, class T61, class T62, class T63, class T64,
        class T65, class T66, class T67, class T68, class T69, class T70, class T71, class T72,
        class T73, class T74, class T75, class T76, class T77, class T78, class T79, class T80,
        class T81, class T82, class T83, class T84, class T85, class T86, class T87, class T88,
        class T89, class T90, class T91, class T92, class T93, class T94, class T95, class T96,
        class T97, class T98, class T99, class T100, class T101, class T102, class T103, class T104,
        class T105, class T106, class T107, class T108, class T109, class T110, class T111,
        class T112, class T113, class T114, class T115, class T116, class T117, class T118,
        class T119, class T120, class T121, class T122, class T123, class T124, class T125,
        class T126, class T127, class T128, class T129, class T130, class T131, class T132,
        class T133, class T134, class T135, class T136, class T137, class T138, class T139,
        class T140, class T141, class T142, class T143, class T144, class T145, class T146,
        class T147, class T148, class T149, class T150, class T151, class T152, class T153,
        class T154, class T155, class T156, class T157, class T158, class T159, class T160,
        class T161, class T162, class T163, class T164, class T165, class T166, class T167,
        class T168, class T169, class T170, class T171, class T172, class T173, class T174,
        class T175, class T176, class T177, class T178, class T179, class T180, class T181,
        class T182, class T183, class T184, class T185, class T186, class T187, class T188,
        class T189, class T190, class T191, class T192, class T193, class T194, class T195,
        class T196, class T197, class T198, class T199, class T200, class T201, class T202,
        class T203, class T204, class T205, class T206, class T207, class T208, class T209,
        class T210, class T211, class T212, class T213, class T214, class T215, class T216,
        class T217, class T218, class T219, class T220, class T221, class T222, class T223,
        class T224, class T225, class T226, class T227, class T228, class T229, class T230,
        class T231, class T232, class T233, class T234, class T235, class T236, class T237,
        class T238, class T239, class T240, class T241, class T242, class T243, class T244,
        class T245, class T246, class T247, class T248, class T249, class T250, class T251,
        class T252, class T253, class T254, class T255, typename... Ts, typename Comp>
    struct sort_impl<
        list<>,
        list<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18,
             T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35,
             T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52,
             T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69,
             T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,
             T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102,
             T103, T104, T105, T106, T107, T108, T109, T110, T111, T112, T113, T114, T115, T116,
             T117, T118, T119, T120, T121, T122, T123, T124, T125, T126, T127, T128, T129, T130,
             T131, T132, T133, T134, T135, T136, T137, T138, T139, T140, T141, T142, T143, T144,
             T145, T146, T147, T148, T149, T150, T151, T152, T153, T154, T155, T156, T157, T158,
             T159, T160, T161, T162, T163, T164, T165, T166, T167, T168, T169, T170, T171, T172,
             T173, T174, T175, T176, T177, T178, T179, T180, T181, T182, T183, T184, T185, T186,
             T187, T188, T189, T190, T191, T192, T193, T194, T195, T196, T197, T198, T199, T200,
             T201, T202, T203, T204, T205, T206, T207, T208, T209, T210, T211, T212, T213, T214,
             T215, T216, T217, T218, T219, T220, T221, T222, T223, T224, T225, T226, T227, T228,
             T229, T230, T231, T232, T233, T234, T235, T236, T237, T238, T239, T240, T241, T242,
             T243, T244, T245, T246, T247, T248, T249, T250, T251, T252, T253, T254, T255, Ts...>,
        Comp>
    {
        using type = merge<
            typename sort_impl<
                list<>,
                list<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17,
                     T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33,
                     T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49,
                     T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65,
                     T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81,
                     T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97,
                     T98, T99, T100, T101, T102, T103, T104, T105, T106, T107, T108, T109, T110,
                     T111, T112, T113, T114, T115, T116, T117, T118, T119, T120, T121, T122, T123,
                     T124, T125, T126, T127, T128, T129, T130, T131, T132, T133, T134, T135, T136,
                     T137, T138, T139, T140, T141, T142, T143, T144, T145, T146, T147, T148, T149,
                     T150, T151, T152, T153, T154, T155, T156, T157, T158, T159, T160, T161, T162,
                     T163, T164, T165, T166, T167, T168, T169, T170, T171, T172, T173, T174, T175,
                     T176, T177, T178, T179, T180, T181, T182, T183, T184, T185, T186, T187, T188,
                     T189, T190, T191, T192, T193, T194, T195, T196, T197, T198, T199, T200, T201,
                     T202, T203, T204, T205, T206, T207, T208, T209, T210, T211, T212, T213, T214,
                     T215, T216, T217, T218, T219, T220, T221, T222, T223, T224, T225, T226, T227,
                     T228, T229, T230, T231, T232, T233, T234, T235, T236, T237, T238, T239, T240,
                     T241, T242, T243, T244, T245, T246, T247, T248, T249, T250, T251, T252, T253,
                     T254>,
                Comp>::type,
            typename sort_impl<list<>, list<T255, Ts...>, Comp>::type, Comp>;
    };
}
template <class Seq, class Comp = less<_1, _2>>
using sort = append<clear<Seq>, typename detail::sort_impl<list<>, wrap<Seq, list>, Comp>::type>;
}
namespace brigand
{
template <typename A>
struct complement
    : brigand::integral_constant<typename A::value_type, typename A::value_type(~A::value)>
{
};
}
namespace brigand
{
  template <typename A, typename B>
  struct divides : brigand::integral_constant < typename A::value_type, A::value / B::value > {};
}
namespace brigand
{
  template<class T>
  struct identity
  {
    using type = T;
  };
}
namespace brigand
{
  template <typename A, typename B>
  struct max : brigand::integral_constant < typename A::value_type
                                      , (A::value < B::value) ? B::value : A::value
                                      >
  {};
}
namespace brigand
{
template <typename A, typename B>
struct min : brigand::integral_constant<typename A::value_type,
                                        (A::value < B::value) ? A::value : B::value>
{
};
}
namespace brigand
{
template <typename A, typename B>
struct minus : brigand::integral_constant<typename A::value_type, A::value - B::value>
{
};
}
namespace brigand
{
template <typename A, typename B>
struct modulo : brigand::integral_constant<typename A::value_type, A::value % B::value>
{
};
}
namespace brigand
{
template <typename A>
struct negate : brigand::integral_constant<typename A::value_type, -A::value>
{
};
}
namespace brigand
{
template <typename A>
struct next : brigand::integral_constant<typename A::value_type, A::value + 1>
{
};
}
namespace brigand
{
template <typename A, typename B>
struct plus : brigand::integral_constant<typename A::value_type, A::value + B::value>
{
};
}
namespace brigand
{
template <typename A>
struct prev : brigand::integral_constant<typename A::value_type, A::value - 1>
{
};
}
namespace brigand
{
template <typename A, typename B>
struct times : brigand::integral_constant<typename A::value_type, A::value * B::value>
{
};
}
namespace brigand
{
template <typename A, typename B>
struct bitand_ : brigand::integral_constant<typename A::value_type, A::value & B::value>
{
};
}
namespace brigand
{
template <typename A, typename B>
struct bitor_ : brigand::integral_constant<typename A::value_type, A::value | B::value>
{
};
}
namespace brigand
{
template <typename A, typename B>
struct bitxor_ : brigand::integral_constant<typename A::value_type, A::value ^ B::value>
{
};
}
namespace brigand
{
  template <typename A, typename B>
  struct shift_left : brigand::integral_constant<typename A::value_type, (A::value << B::value)> {};
}
namespace brigand
{
  template <typename A, typename B>
  struct shift_right : brigand::integral_constant<typename A::value_type, (A::value >> B::value)> {};
}
namespace brigand
{
  template <typename A, typename B>
  struct equal_to : bool_ < (A::value == B::value) > {};
}
namespace brigand
{
  template <typename A, typename B>
  struct greater : bool_<(A::value > B::value) > {};
}
namespace brigand
{
  template <typename A, typename B>
  struct greater_equal : bool_ < (A::value >= B::value) > {};
}
namespace brigand
{
  template <typename A, typename B>
  struct less_equal : bool_ < (A::value <= B::value) > {};
}
namespace brigand
{
  template <typename A, typename B>
  struct not_equal_to : bool_ < (A::value != B::value) > {};
}

namespace brigand
{
  template <typename Condition, typename A, typename B>
  struct eval_if
  {
    using type = typename std::conditional<Condition::value, A, B>::type::type;
  };
  template <bool Condition, typename A, typename B>
  struct eval_if_c
  {
    using type = typename std::conditional<Condition, A, B>::type::type;
  };
}

namespace brigand
{
  template <typename Condition, typename A, typename B>
  struct if_ : std::conditional<Condition::value, A, B> {};
  template <bool Condition, typename A, typename B>
  struct if_c : std::conditional<Condition, A, B> {};
}
namespace brigand
{
  template <typename A, typename B>
  struct and_ : brigand::integral_constant <typename A::value_type, A::value && B::value > {};
}
namespace brigand
{
  template <typename T>
  struct not_ : brigand::integral_constant<typename T::value_type, !T::value> {};
}
namespace brigand
{
  template <typename A, typename B>
  struct or_ : brigand::integral_constant < typename A::value_type, A::value || B::value > {};
}
namespace brigand
{
  template <typename A, typename B>
  struct xor_ : brigand::integral_constant<typename A::value_type, A::value != B::value> {};
}
namespace brigand
{
  template<class T>
  struct always
  {
     using type = T; 
  };
}
namespace brigand
{
namespace detail
{
    template<template<class> class F, unsigned N, class T>
    struct repeat_impl
    : repeat_impl<F, N-7, F<F<F<F<F<F<F<T>>>>>>>>
    {};
    template<template<class> class F, class T>
    struct repeat_impl<F, 7, T>
    {
        using type = F<F<F<F<F<F<F<T>>>>>>>;
    };
    template<template<class> class F, class T>
    struct repeat_impl<F, 6, T>
    {
        using type = F<F<F<F<F<F<T>>>>>>;
    };
    template<template<class> class F, class T>
    struct repeat_impl<F, 5, T>
    {
        using type = F<F<F<F<F<T>>>>>;
    };
    template<template<class> class F, class T>
    struct repeat_impl<F, 4, T>
    {
        using type = F<F<F<F<T>>>>;
    };
    template<template<class> class F, class T>
    struct repeat_impl<F, 3, T>
    {
        using type = F<F<F<T>>>;
    };
    template<template<class> class F, class T>
    struct repeat_impl<F, 2, T>
    {
        using type = F<F<T>>;
    };
    template<template<class> class F, class T>
    struct repeat_impl<F, 1, T>
    {
        using type = F<T>;
    };
    template<template<class> class F, class T>
    struct repeat_impl<F, 0, T>
    {
        using type = T;
    };
}
namespace lazy
{
    template<template<class> class F, class N, class T>
    using repeat = typename detail::repeat_impl<F, N::value, T>;
}
    template<template<class> class F, class N, class T>
    using repeat = typename ::brigand::lazy::repeat<F, N, T>::type;
}
namespace brigand
{
template <typename T>
struct sizeof_ : brigand::integral_constant<unsigned int, sizeof(T)>
{
};
}
namespace brigand
{
namespace detail
{
    template<class C, class K>
    struct has_key_impl
    {
        using type = decltype(C::has_key(type_<K>{}));
    };
}
    template<class L, class K>
    using has_key = typename detail::has_key_impl<L, K>::type;
}
namespace brigand
{
namespace detail
{
    template<class Start, unsigned N, class Next, class... E>
    struct mksq8
    : mksq8<brigand::apply<Next, Start>, N-1, Next, E..., Start>
    {};
    template<class Start, class Next, class... E>
    struct mksq8<Start, 0, Next, E...>
    {
        using type = list<E...>;
    };
    template<class Start, class Next, class... E>
    struct mksq8<Start, 1, Next, E...>
    {
        using type = list<E..., Start>;
    };
    template<class Start, class Next>
    struct mksq8<Start, 8, Next>
    {
        using t1 = brigand::apply<Next, Start>;
        using t2 = brigand::apply<Next, t1>;
        using t3 = brigand::apply<Next, t2>;
        using t4 = brigand::apply<Next, t3>;
        using t5 = brigand::apply<Next, t4>;
        using t6 = brigand::apply<Next, t5>;
        using t7 = brigand::apply<Next, t6>;
        using type = list<Start, t1, t2, t3, t4, t5, t6, t7>;
    };
    template<template<class...> class List, class Start, unsigned N, class Next, bool, class... L>
    struct make_sequence_impl
    : make_sequence_impl<
        List,
        brigand::apply<Next, typename mksq8<Start, 8, Next>::t7>,
        N-8,
        Next,
        (N-8<=8),
        L...,
        typename mksq8<Start, 8, Next>::type
    >
    {};
    template<template<class...> class List, class Start, unsigned N, class Next, class... L>
    struct make_sequence_impl<List, Start, N, Next, true, L...>
    {
        using type = append<List<>, L..., typename mksq8<Start, N, Next>::type>;
    };
}
    template<class Start, unsigned N, class Next = next<_1>, template<class...> class List = list>
    using make_sequence = typename detail::make_sequence_impl<List, Start, N, Next, (N<=8)>::type;
}
namespace brigand
{
template <class L, unsigned int Index>
using erase_c =
    append<front<split_at<L, brigand::integral_constant<unsigned int, Index>>>,
           pop_front<back<split_at<L, brigand::integral_constant<unsigned int, Index>>>>>;
namespace detail
{
    template <typename T>
    struct has_erase_method
    {
        struct dummy
        {
        };
        template <typename C, typename P>
        static auto test(P * p) -> decltype(C::erase(type_<P>{}), brigand::true_type());
        template <typename, typename>
        static brigand::false_type test(...);
        static const bool value =
            std::is_same<brigand::true_type, decltype(test<T, dummy>(nullptr))>::value;
    };
    template <class L, class I, bool>
    struct erase_dispatch
    {
        using type = erase_c<L, I::value>;
    };
    template <class C, class K>
    struct erase_dispatch<C, K, true>
    {
        using type = decltype(C::erase(type_<K>{}));
    };
}
template <class L, class K>
using erase = typename detail::erase_dispatch<L, K, detail::has_erase_method<L>::value>::type;
}
namespace brigand
{
namespace detail
{
    template <class C, class T>
    struct insert_impl
    {
        using type = decltype(C::insert(type_<T>{}));
    };
}
    template<class L, class T>
    using insert = typename detail::insert_impl<L, T>::type;
}
namespace brigand
{
namespace detail
{
    template <class L, class K>
    struct contains_impl
    {
        using type = decltype(L::contains(type_<K>{}));
    };
}
    template <class L, class K>
    using contains = typename detail::contains_impl<L, K>::type;
}
namespace brigand
{
namespace detail
{
    template<class... Ts>
    struct make_set;
    template<class U, class K>
    struct set_erase_pred_impl
    {
        using type = list<U>;
    };
    template<class K>
    struct set_erase_pred_impl<K,K>
    {
        using type = list<>;
    };
    template <class... T>
    struct set_impl
    {
        template <typename K, typename = decltype(static_cast<type_<K>*>(static_cast<make_set<T...>*>(nullptr)))>
        static brigand::true_type contains(type_<K>);
        template <typename K>
        static brigand::false_type contains(K);
        template <typename K, typename = decltype(static_cast<type_<K>*>(static_cast<make_set<T...>*>(nullptr)))>
        static brigand::true_type has_key(type_<K>);
        template <typename K>
        static brigand::false_type has_key(K);
        template <class K>
        static append<set_impl<>, typename set_erase_pred_impl<T, K>::type...> erase(type_<K>);
        template<class K, class = decltype(static_cast<type_<K>*>(static_cast<make_set<T...>*>(nullptr)))>
        static set_impl insert(type_<K>);
        template<class K>
        static set_impl<T..., typename K::type> insert(K);
    };
    template<class... Ts>
    struct make_set : type_<Ts>...
    {
      using type = set_impl<Ts...>;
    };
}
    template <typename... T>
    using set_wrapper = typename detail::make_set<T...>::type;
    template <typename L>
    using as_set = wrap<L, set_wrapper>;
    template<class... Ts>
    using set = typename detail::make_set<Ts...>::type;
}
namespace brigand
{
namespace detail
{
    template <typename Pair>
  struct get_second {
    using type = typename Pair::second_type;
  };
}
template <typename Map, template <class...> class Sequence = brigand::list>
using values_as_sequence = transform<as_sequence<Map, Sequence>, detail::get_second<_1>>;
}
namespace brigand
{
namespace detail
{
    template <typename Pair>
  struct get_first {
    using type = typename Pair::first_type;
  };
}
template <typename Map, template <class...> class Sequence = brigand::set>
using keys_as_sequence = transform<as_sequence<Map, Sequence>, detail::get_first<_1>>;
}
namespace brigand
{
  struct empty_base {};
}
namespace brigand
{
  template<typename T, typename R = void > struct has_type
  {
    using type = R;
  };
}
namespace brigand
{
  template<typename... Ts> struct inherit;
  template<typename T> struct inherit<T>
  {
    struct type : public T {};
  };
  template<> struct inherit<>
  {
    using type = empty_base;
  };
  template<> struct inherit<empty_base>
  {
    using type = empty_base;
  };
  template<typename T1, typename T2> struct inherit<T1,T2>
  {
    struct type : public T1, T2 {};
  };
  template<typename T1> struct inherit<T1,empty_base>
  {
    using type = T1;
  };
  template<typename T2> struct inherit<empty_base,T2>
  {
    using type = T2;
  };
  template<> struct inherit<empty_base,empty_base>
  {
    using type = empty_base;
  };
  template<typename T1, typename T2, typename T3, typename... Ts>
  struct  inherit<T1, T2, T3, Ts...>
        : inherit<T1, typename inherit<T2,typename inherit<T3, Ts...>::type>::type>
  {};
}
namespace brigand
{
  namespace lazy
  {
    template< typename Types
      , typename Node
      , typename Root = brigand::empty_base
    >
      struct inherit_linearly;
  template< typename Types
      , template<typename...> class Node, typename...Ts
      , typename Root
    >
  struct inherit_linearly<Types,Node<Ts...>,Root>
    {
      using type = brigand::fold<Types,Root,bind<Node,Ts...>>;
    };
  }
  template< typename Types
            , typename Node
            , typename Root = brigand::empty_base
            >
  using inherit_linearly = typename lazy::inherit_linearly<Types,Node,Root>::type;
}


// namespace brigand
// {
//   template<typename RealType, typename Type, Type Value>
//   struct real_ : brigand::integral_constant<Type,Value>
//   {
//     using value_type  = RealType;
//     using parent      = brigand::integral_constant<Type,Value>;
//     inline operator value_type() const
//     {
//       value_type that;
//       std::memcpy(&that, &parent::value, sizeof(value_type));
//       return that;
//     }
//   };
//   template<std::uint32_t Value>
//   struct single_ : real_<float, std::uint32_t, Value> {};
//   template<std::uint64_t Value>
//   struct double_ : real_<double, std::uint64_t,Value> {};
// }
#endif