// /*
//  * typeutils.h
//  *
//  * Created: 03.02.2013 22:44:29
//  *  Author: astr0
//  */ 

// #pragma once

// #ifndef TYPEUTILS_H_
// #define TYPEUTILS_H_

// #include "TypeManip.h"

// namespace fasthal
// {
// 	namespace common
// 	{
// 		template<class T1, class T2>
// 		constexpr bool isSameType()
// 		{
// 			return Loki::IsSameType<T1, T2>::value;
// 		}
		
// 		// Removes pointer from a type
// 		template<class T> struct remove_pointer                    {typedef T type;};
// 		template<class T> struct remove_pointer<T*>                {typedef T type;};
// 		template<class T> struct remove_pointer<T* const>          {typedef T type;};
// 		template<class T> struct remove_pointer<T* volatile>       {typedef T type;};
// 		template<class T> struct remove_pointer<T* const volatile> {typedef T type;};

// 		// Removes reference from a type
// 		template<class T> struct remove_reference                    {typedef T type;};
// 		template<class T> struct remove_reference<T&>                {typedef T type;};
			
			
// 		/// add_rvalue_reference
// 		template<class T>
// 		struct add_rvalue_reference
// 		{
// 			typedef T&& type;
// 		};
		
// 		template<class T>
// 		struct add_rvalue_reference<T&>
// 		{
// 			typedef T& type;
// 		};		

// 		template<class T>
// 		struct add_rvalue_reference<T&&>
// 		{
// 			typedef T type;
// 		};
			
// 		template <class T>
// 		typename add_rvalue_reference<T>::type declval(); // no definition required			
// 	}
// }


// #endif /* TYPEUTILS_H_ */