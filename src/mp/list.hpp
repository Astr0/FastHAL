#ifndef FH_MP_LIST_H_
#define FH_MP_LIST_H_

#include "type_traits.hpp"

namespace fasthal
{
namespace mp
{

template <typename... T>
struct list
{
};

template <unsigned N, typename T, typename ...R>
struct type_at {
    using type = typename type_at<N - 1, R...>::type;
};

template <typename T, typename ...R>
struct type_at<0, T, R...> {
    using type = T;
};

template <template<typename> class TFunc, typename ...R>
struct all: true_type { };

template <template<typename> class TFunc, typename T, typename ...R>
struct all<TFunc, T, R...> : bool_constant<TFunc<T>::value && all<TFunc, R...>::value>{    
};


};
};

#endif