// TR1 utility -*- C++ -*-

// Copyright (C) 2007, 2008, 2009 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file tr1_impl/utility
 *  This is an internal header file, included by other library headers.
 *  You should not attempt to use it directly.
 */

#define _GLIBCXX_INCLUDE_AS_CXX0X

namespace std
{
  template<class _Tp>
    class tuple_size;

#ifdef _GLIBCXX_INCLUDE_AS_CXX0X
  template<std::size_t _Int, class _Tp>
#else
  template<int _Int, class _Tp>
#endif
    class tuple_element;

   // Various functions which give std::pair a tuple-like interface.
  template<class _Tp1, class _Tp2>
    struct tuple_size<std::pair<_Tp1, _Tp2> >
#ifdef _GLIBCXX_INCLUDE_AS_CXX0X
    { static const std::size_t value = 2; };
#else
    { static const int value = 2; };
#endif

  template<class _Tp1, class _Tp2>
#ifdef _GLIBCXX_INCLUDE_AS_CXX0X
    const std::size_t
#else
    const int
#endif
    tuple_size<std::pair<_Tp1, _Tp2> >::value;

  template<class _Tp1, class _Tp2>
    struct tuple_element<0, std::pair<_Tp1, _Tp2> >
    { typedef _Tp1 type; };
 
  template<class _Tp1, class _Tp2>
    struct tuple_element<1, std::pair<_Tp1, _Tp2> >
    { typedef _Tp2 type; };

#ifdef _GLIBCXX_INCLUDE_AS_CXX0X
  template<std::size_t _Int>
#else
  template<int _Int>
#endif
    struct __pair_get;

  template<>
    struct __pair_get<0>
    {
      template<typename _Tp1, typename _Tp2>
      static _Tp1& __get(std::pair<_Tp1, _Tp2>& __pair)
      { return __pair.first; }

      template<typename _Tp1, typename _Tp2>
      static const _Tp1& __const_get(const std::pair<_Tp1, _Tp2>& __pair)
      { return __pair.first; }
    };

  template<>
    struct __pair_get<1>
    {
      template<typename _Tp1, typename _Tp2>
      static _Tp2& __get(std::pair<_Tp1, _Tp2>& __pair)
      { return __pair.second; }

      template<typename _Tp1, typename _Tp2>
      static const _Tp2& __const_get(const std::pair<_Tp1, _Tp2>& __pair)
      { return __pair.second; }
    };

#ifdef _GLIBCXX_INCLUDE_AS_CXX0X
  template<std::size_t _Int, class _Tp1, class _Tp2>
#else
  template<int _Int, class _Tp1, class _Tp2>
#endif
    inline typename tuple_element<_Int, std::pair<_Tp1, _Tp2> >::type&
    get(std::pair<_Tp1, _Tp2>& __in)
    { return __pair_get<_Int>::__get(__in); }

#ifdef _GLIBCXX_INCLUDE_AS_CXX0X
  template<std::size_t _Int, class _Tp1, class _Tp2>
#else
  template<int _Int, class _Tp1, class _Tp2>
#endif
    inline const typename tuple_element<_Int, std::pair<_Tp1, _Tp2> >::type&
    get(const std::pair<_Tp1, _Tp2>& __in)
    { return __pair_get<_Int>::__const_get(__in); }
}
