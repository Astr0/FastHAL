#ifndef FH_CONSTLIST_H_
#define FH_CONSTLIST_H_

#include "brigand.hpp"
#include "../std/std_fake.hpp"

namespace fasthal{
    namespace mp{
        namespace details{
            template<class TElement>
            struct is_static_element_impl: brigand::false_type{ };
               
            template<class TElement, std::size_t Index = 0, bool is_static = is_static_element_impl<std::base_type_t<TElement>>::value>
            struct element_holder{
                const TElement element;
                
                constexpr element_holder(TElement __element): element(__element) { }

                constexpr TElement getElement(){return element;}
            };

            template<class TElement, std::size_t Index>
            struct element_holder<TElement, Index, true>{
                constexpr element_holder(TElement __element) { }

                constexpr TElement getElement(){return TElement{};}
            };

        }

        // cause tuple is not constexpr is C++14....
        template <class... TElements>
        struct const_list;

        template<>
        struct const_list<>{};

        template <class T, class...TRest>
        struct const_list<T, TRest...>: details::element_holder<T>{
            using rest_t = const_list<TRest...>;

            const rest_t rest;

            constexpr const_list(const T __el, const TRest... __rest): details::element_holder<T>{__el}, rest{__rest...}
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
                    return list.getElement();
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
        constexpr auto make_const_list(const TElements... elements){
            return const_list<TElements...>{elements...};
        }

        template<std::size_t N, class... TElements>
        constexpr auto get(const_list<TElements...> list){
            return details::const_list_get_helper<const_list<TElements...>, N>::get(list);                
        }
    }
}
#endif