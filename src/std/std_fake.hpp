#ifndef STD_FAKE_H_
#define STD_FAKE_H_

#include "type_traits.hpp"

namespace std{
	
template<bool VTest, typename T>
using enable_if_t = typename enable_if<VTest, T>::type;

template<bool VTest>
using enable_if_c = enable_if_t<VTest, void*>;

template<typename T>
using base_type_t = 
    typename std::remove_cv<
        typename std::remove_reference<
            typename std::remove_pointer<T>::type
        >::type>
    ::type;

template<typename T1, typename T2>
static constexpr auto is_same_v = std::is_same<T1, T2>::value;

template<class T, size_t N>
constexpr size_t array_size(T (&)[N]) { return N; }

template<unsigned VNum>
struct empty_t{};

};

#endif