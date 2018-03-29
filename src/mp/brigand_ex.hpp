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
                    brigand::remove_if<
                        TList<R...>, 
                        std::is_same<T, brigand::_1>>>::type, 
            T>;
        };
    };

    template <class TList>
    using no_duplicates = typename details::no_duplicates_impl<TList>::type;

};

#endif