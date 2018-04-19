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
        static TRes invoke_functor(TArgs... args){
            static_assert(sizeof(T) == 1, "Not 'static' functor");
            return (*reinterpret_cast<T*>(0))(args...);
        }
    public:
        constexpr static_func(){}
        constexpr static_func(ptr_t func): _func(func){}
        constexpr static_func(const static_func<TRes(TArgs...)>& f):_func(f._func){}

        template<class TFunc>
        constexpr static_func(TFunc functor): _func(invoke_functor<TFunc>){}

        TRes operator()(TArgs... args){
            // if constexpr (std::is_same_v<TRes, void>)
            //     _func(args...);
            // else
            return _func(args...);
        }
    };
}

#endif