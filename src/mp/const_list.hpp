#ifndef FH_CONSTLIST_H_
#define FH_CONSTLIST_H_

#include "brigand.hpp"

namespace fasthal{
    namespace mp{
        // cause tuple is not constexpr is C++14....
        template <class... TElements>
        struct const_list;

        template<>
        struct const_list<>{};

        template <class T, class...TRest>
        struct const_list<T, TRest...>{
            using element_t = T;
            using rest_t = const_list<TRest...>;

            const element_t element;
            const rest_t rest;

            constexpr const_list(const element_t __el, const TRest... __rest): element{__el}, rest{__rest...}
            {                
            }
        };

        namespace details{
            template<class TList, std::size_t N, bool found = N == 0>
            struct const_list_get_helper;
            
            template<class TList, std::size_t N>
            struct const_list_get_helper<TList, N, true>
            {
                static constexpr auto get(TList list){
                    return list.element;
                }
            };

            template<class TList, std::size_t N>
            struct const_list_get_helper<TList, N, false>
            {
                static constexpr auto get(TList list){
                    using next_t = const_list_get_helper<typename TList::rest_t, N-1>;
                    return next_t::get(list.rest);
                }
            };
        }

        template<class... TElements>
        static constexpr auto make_const_list(const TElements... elements){
            return const_list<TElements...>{elements...};
        }

        template<std::size_t N, class... TElements>
        constexpr auto get(const_list<TElements...> list){
            return details::const_list_get_helper<const_list<TElements...>, N>::get(list);                
        }
    }
}
#endif