#ifndef STD_FAKE_H_
#define STD_FAKE_H_

#include "type_traits.hpp"

namespace std{
	
template<bool B, typename C>
using enable_if_t = typename enable_if<B, C>::type;

// template <class T>
// inline T&& forward(typename std::remove_reference<T>::type& t) noexcept
// {
//     return static_cast<T&&>(t);
// }

// template <class T>
// inline T&& forward(typename std::remove_reference<T>::type&& t) noexcept
// {
//     static_assert(!std::is_lvalue_reference<T>::value,
//                   "Can not forward an rvalue as an lvalue.");
//     return static_cast<T&&>(t);
// }

};

#endif