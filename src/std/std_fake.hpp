#ifndef STD_FAKE_H_
#define STD_FAKE_H_

#include "type_traits.hpp"

namespace std{
	
template<bool VTest, typename T>
using enable_if_t = typename enable_if<VTest, T>::type;

template<typename T>
using base_type_t = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

};

#endif