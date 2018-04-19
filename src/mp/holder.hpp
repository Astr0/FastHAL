#ifndef FH_MP_HOLDER_H_
#define FH_MP_HOLDER_H_

namespace fasthal::mp{
    namespace details{
        template<class T>
        struct is_static_impl: brigand::false_type{ };

        template<typename T, T V>
        struct is_static_impl<std::integral_constant<T, V>>: std::true_type{};
    }

    template<class T>
    using is_static = details::is_static_impl<std::base_type_t<T>>;

    template<class T, std::size_t Index = 0, bool is_static = is_static<T>::value>
    struct holder{
        T _value;
        
        constexpr holder(){}
        constexpr holder(T v): _value(v) { }

        constexpr T get() const{return _value;}

        void set(T v) { _value = v;}
    };

    template<class T, std::size_t Index>
    struct holder<T, Index, true>{
        constexpr holder() { }

        constexpr holder(T v) { }

        constexpr T get() const{return T{};}
        void set(T v){}
    };
    
}

#endif