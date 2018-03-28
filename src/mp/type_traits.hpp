#ifndef FH_TYPE_TRAITS_H_
#define FH_TYPE_TRAITS_H_
namespace fasthal{
namespace mp{
    template<typename T, T v>
    struct integral_constant {
        static constexpr T value = v;
        typedef T value_type;
        typedef integral_constant type;
        constexpr operator value_type() const noexcept { return value; }
        constexpr value_type operator()() const noexcept { return value; } //Начиная с c++14
    };
    
    template<bool B>
    using bool_constant = integral_constant<bool, B>;

    using true_type = bool_constant<true>;
    using false_type = bool_constant<false>;

    template<bool B, class T = void>
    struct enable_if {};
    
    template<class T>
    struct enable_if<true, T> { typedef T type; };

    template <bool B, class T>
    using enable_if_t = typename enable_if<B, T>::type;
};
};
#endif
