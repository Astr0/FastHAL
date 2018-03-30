// /*
//  * typelistutils.h
//  *
//  * Created: 04.02.2013 3:59:27
//  *  Author: astr0
//  */ 


// #ifndef TYPELISTUTILS_H_
// #define TYPELISTUTILS_H_

// #include "Typelist.h"
// #include "typeutils.h"

// namespace fasthal
// {
// 	namespace common
// 	{
// 		namespace TL
// 		{
// 			// Variadic to type list
// 			template<int VPosition, template<int, class> class THead, class ...T> 
// 			struct MakeList;
			
// 			template<int VPosition, template<int, class> class THead>
// 			struct MakeList<VPosition, THead>
// 			{
// 				typedef Loki::NullType Result;
// 			};
			
// 			template<int VPosition, template<int, class> class THead, class TElement, class ...T>
// 			struct MakeList<VPosition, THead, TElement, T...>
// 			{
// 				typedef Loki::Typelist<THead<VPosition, TElement>, typename MakeList<VPosition + 1, THead, T...>::Result> Result;
// 			};	
			
// 			// Skip first VNum elements from TypeList
// 			template <class TList, int VNum> 
// 			struct SkipFirst;

// 			template <>
// 			struct SkipFirst<Loki::NullType, 0>
// 			{
// 				typedef Loki::NullType Result;
// 			};

// 			template <class THead, class TTail>
// 			struct SkipFirst<Loki::Typelist<THead, TTail>, 0>
// 			{
// 				typedef Loki::Typelist<THead, TTail> Result;
// 			};

// 			template <class THead, class TTail, int VNum>
// 			struct SkipFirst<Loki::Typelist<THead, TTail>, VNum>
// 			{
// 				typedef typename SkipFirst<TTail, VNum-1>::Result Result;
// 			};	
			
// 			// Take first VNum elements from TypeList
// 			template <class TList, int VNum> 
// 			struct TakeFirst;

// 			template <>
// 			struct TakeFirst<Loki::NullType, 0>
// 			{
// 				typedef Loki::NullType Result;
// 			};

// 			template <class THead, class TTail>
// 			struct TakeFirst<Loki::Typelist<THead, TTail>, 0>
// 			{
// 				typedef Loki::NullType Result;
// 			};

// 			template <class THead, class TTail, int VNum>
// 			struct TakeFirst<Loki::Typelist<THead, TTail>, VNum>
// 			{
// 				typedef Loki::Typelist<THead, typename TakeFirst<TTail, VNum-1>::Result>Result;
// 			};
			
// 			// Convert one type list to another TSelector<THead>::Result
// 			template <class TList, template<class> class TSelector>
// 			struct Select;
			
// 			template<template<class> class TSelector>
// 			struct Select<Loki::NullType, TSelector>
// 			{
// 				typedef Loki::NullType Result;				
// 			};
			
// 			template<class THead, class TTail, template<class> class TSelector>
// 			struct Select<Loki::Typelist<THead, TTail>, TSelector>
// 			{
// 				typedef Loki::Typelist<typename TSelector<THead>::Result, typename Select<TTail, TSelector>::Result> Result;
// 			};

// 			// filters type list by TPredicate<THead>::value			
// 			template <class TList, template <class> class TPredicate>
// 			struct Where;

// 			template <template <class> class TPredicate>
// 			struct Where<Loki::NullType, TPredicate>
// 			{
// 				typedef Loki::NullType Result;
// 			};

// 			template <class THead, class TTail, template <class> class TPredicate>
// 			class Where<Loki::Typelist<THead, TTail>, TPredicate>
// 			{
// 				typedef typename Where<TTail, TPredicate>::Result Fail;
// 				typedef Loki::Typelist<THead, typename Where<TTail, TPredicate>::Result> Pass;
// 				public:
// 				typedef typename Loki::Select<TPredicate<THead>::value, Pass, Fail>::Result Result;
// 			};
			
// 			// Not predicate
// 			template<template<class> class TPredicate>
// 			struct Not
// 			{
// 				template<class TInput>
// 				struct Result
// 				{
// 					static constexpr bool value = !TPredicate<TInput>::value;
// 				};
// 			};
			
// 			// Aggregate TPredicate<THead, VTail>::value, 
// 			template <class TList, class TValue, TValue VNullType, template<class, TValue> class TPredicate>
// 			struct Aggregate;

// 			template <class TValue, TValue VNullType, template<class, TValue> class TPredicate>
// 			struct Aggregate<Loki::NullType, TValue, VNullType, TPredicate>
// 			{
// 				static constexpr TValue value = VNullType;
// 			};
			
// 			template <class THead, class TTail, class TValue, TValue VNullType, template<class, TValue> class TPredicate>
// 			struct Aggregate<Loki::Typelist<THead, TTail>, TValue, VNullType, TPredicate>
// 			{
// 				static constexpr TValue value = TPredicate<THead, Aggregate<TTail, TValue, VNullType, TPredicate>::value>::value;
// 			};
			
// 			// true if TList is the end of list
// 			template <class TList>
// 			constexpr bool isEnd()
// 			{
// 				return isSameType<TList, Loki::NullType>();
// 			}
			
// 			// returns list length
// 			template <class TList>
// 			constexpr int length()
// 			{
// 				return Loki::TL::Length<TList>::value;
// 			}
			
// 			// true if all elements match predicate
// 			template<class TList, template<class> class TPredicate>
// 			struct All;
			
// 			template <template<class> class TPredicate>
// 			struct All<Loki::NullType, TPredicate>
// 			{
// 				// empty matches, right?
// 				static constexpr bool value = true;
// 			};
			
// 			template <class THead, class TTail, template<class> class TPredicate>
// 			struct All<Loki::Typelist<THead, TTail>, TPredicate>
// 			{
// 				static constexpr bool value = TPredicate<THead>::value ? All<TTail, TPredicate>::value : false;
// 			};
// 		}
// 	}
// }


// #endif /* TYPELISTUTILS_H_ */