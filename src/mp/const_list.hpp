#ifndef FH_CONSTLIST_H_
#define FH_CONSTLIST_H_

#include "brigand.hpp"
#include "holder.hpp"
#include "../std/type_traits.hpp"
#include "../std/std_fake.hpp"

namespace fasthal{
    namespace mp{
        
        template <class... TElements>
        struct const_list{};

        template <class T, class... TRest>
        struct const_list<T, TRest...>: 
            holder<T, (sizeof...(TRest))>, 
            const_list<TRest...>{

            constexpr const_list(){}

            constexpr const_list(const T __el, const TRest... __rest): holder<T, (sizeof...(TRest))>{__el}, const_list<TRest...>{__rest...}
            {                
            }
        };

        namespace details{
            // const list statis if all elements are static
            template<typename... T>
            struct is_static_impl<const_list<T...>>: brigand::all<const_list<T...>, brigand::bind<is_static, brigand::_1>> { };
        }

        template<class... TElements>
        constexpr auto make_const_list(TElements... elements){
            return const_list<TElements...>{elements...};
        }

        template<std::size_t N, class... TElements>
        constexpr auto get(const_list<TElements...> list){      
            using list_t = const_list<TElements...>;
            using el_t = brigand::at_c<list_t, N>;                    
            using holder_t = const holder<el_t, (brigand::size<list_t>::value - 1 - N)>;      
            // cause we can't change return type
            return list.holder_t::get();
        }

        namespace details{
            // empty T == full args
            template<typename... T>
            struct flatten_helper{
                template<typename... TArgs>
                static constexpr auto flatten(TArgs... args){
                    return make_const_list(args...);
                }
            }; 

            // not const list
            template<typename T, typename... R>
            struct flatten_helper<T, R...>{
                template<typename... TArgs>
                static constexpr auto flatten(T value, R... rest, TArgs... args){
                    // forward value
                    return flatten_helper<R...>::flatten(rest..., args..., value);
                }
            }; 

            // unwrap const list
            template<typename... TList, typename... R>
            struct flatten_helper<const_list<TList...>, R...>{
                using list_t = const_list<TList...>;
                
                template<typename... TIndex>
                struct list_exec{
                    template<typename... TArgs>
                    static constexpr auto flatten(list_t list, R... rest, TArgs... args){
                        // forward list elements, we can even fully unfold here if pass get<> to flatten
                        return flatten_helper<TList..., R...>::flatten(
                                mp::get<TIndex::value>(list)...,
                                rest..., 
                                args...                                
                        );
                    }
                };

                template<typename... TArgs>
                static constexpr auto flatten(list_t value, R... rest, TArgs... args){
                    using indices_t = brigand::make_sequence<
                        brigand::size_t<0>, 
                        brigand::size<list_t>::value>;
                    return brigand::unpack<indices_t, list_exec>::flatten(value, rest..., args...);
                }
            }; 
        }

        template<class... TElements>
        constexpr auto flatten_const_list(TElements... elements){
            return details::flatten_helper<TElements...>::flatten(elements...);
        }        
    }
}
#endif