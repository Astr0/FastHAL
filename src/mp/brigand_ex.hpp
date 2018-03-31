#ifndef FH_BRIGANDEX_H_
#define FH_BRIGANDEX_H_

#include "brigand.hpp"

namespace brigand{

    namespace details{
        template <class TList>
        struct no_duplicates_impl{
            using type = TList;
        };

        template <template<class...> class TList, class T, class... R>
        // makes list distinct by removing all occurences of T in R[est] of the list
        struct no_duplicates_impl<TList<T, R...>>{
            using type = brigand::push_front<
                typename no_duplicates_impl<
                    brigand::remove<TList<R...>, T>>::type, 
            T>;
        };
    };

    template <class TList>
    // remove duplicate types from list
    using no_duplicates = typename details::no_duplicates_impl<TList>::type;


    namespace details{
        template<unsigned int N>
        struct number_type_impl: 
            std::conditional<(N <= 256), 
                std::uint8_t,
                typename std::conditional<(N <= 65536), std::uint16_t, std::uint32_t>::type> { };
    }

	// smallest type that can hold N values (N-1 max value)
	template<unsigned int N>
	using number_type = typename details::number_type_impl<N>::type;

    namespace details{
        template<template<class...> class TResult>
        struct unpack_impl
        {
            template <typename TR, typename T> struct impl;

            template <typename T, typename... R>
            struct impl<TResult<R...>, T>{
                using type = TResult<R..., T>;
            };
        };
    }
    template<class TList, template<class...> class TResult>
    using unpack = fold<TList, TResult<>, typename details::unpack_impl<TResult>::template impl<_state, _element>>;
}

#endif