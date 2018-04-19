#ifndef FH_FUNCTION_H_
#define FH_FUNCTION_H_

#include "../std/std_fake.hpp"

namespace fasthal::mp{
    template<typename T>
    class static_func;

    template<typename TRes, typename... TArgs>
    class static_func<TRes(TArgs...)>{
        using ptr_t = TRes(*)(TArgs...);
        ptr_t _func;

        template<class T>
        static inline TRes invoke_functor(TArgs... args){
            static_assert(sizeof(T) == 1, "Not 'static' functor");
            // if constexpr (std::is_same_v<TRes, void>)
            //     T{}(args...);
            // else
            return (*reinterpret_cast<T*>(0))(args...);
        }
    public:
        constexpr static_func(){}
        constexpr static_func(const ptr_t func): _func(func){}
        constexpr static_func(const static_func<TRes(TArgs...)>& f):_func(f._func){}
        template<class TFunc>
        constexpr static_func(TFunc functor): _func(invoke_functor<TFunc>){}

        // static_func& operator=(const static_func& x){ _func = x._func; return *this; }
        // static_func& operator=(const ptr_t x){ _func = x; return *this; }
        // template<class T>
        // static_func& operator=(const static_func& x){ _func = x._func; }

        inline TRes operator()(TArgs... args) const{
            // if constexpr (std::is_same_v<TRes, void>)
            //      _func(args...);
            // else
            return _func(args...);
        }

        ~static_func(){}
    };
}

#endif