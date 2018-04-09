#ifndef FH_GPIO_H_
#define FH_GPIO_H_

namespace fasthal{
    namespace details{
        template<typename TFieldBit>
        struct gpio_input_bit{
            using gpin_t = TFieldBit;
            static constexpr auto gpin = gpin_t{};            
        };

        template<typename TFieldBit>
        struct gpio_output_bit{
            using gpout_t = TFieldBit;
            static constexpr auto gpout = gpout_t{};
        };
    }

    // actions
	template<typename TFunc, typename TFieldBit = typename details::func_fieldbit<TFunc>::gpout_t>
	constexpr auto inline set(TFunc func){
		return set(TFieldBit{});
	}

	template<typename TFunc, typename TFieldBit = typename details::func_fieldbit<TFunc>::gpout_t>
	constexpr auto inline clear(TFunc func){
		return clear(TFieldBit{});
	}

	template<typename TFunc, typename TV, typename TFieldBit = typename details::func_fieldbit<TFunc>::gpout_t>
	constexpr auto inline set(TFunc func, TV v){
		return set(TFieldBit{}, v);
	}

	template<typename TFunc, typename TFieldBit = typename details::func_fieldbit<TFunc>::gpout_t>
	constexpr auto inline toggle(TFunc func){
		return toggle(TFieldBit{});
	}

	template<typename TFunc, typename TFieldBit = typename details::func_fieldbit<TFunc>::gpin_t>
	constexpr auto inline read(TFunc func){
		return read(TFieldBit{});
	}

	template<typename TFunc, typename... TFields, typename TFieldBit = typename details::func_fieldbit<TFunc>::gpin_t>
	constexpr bool get(TFunc func, details::field_action_results_t<TFields...> results){
		return get(TFieldBit{});
	}

	// immediate actions
	template<typename TFunc, typename TFieldBit = typename details::func_fieldbit<TFunc>::gpout_t>
	inline void set_(TFunc func){
		 set_(TFieldBit{});
	}

	template<typename TFunc, typename TFieldBit = typename details::func_fieldbit<TFunc>::gpout_t>
	inline void clear_(TFunc func){
		clear_(TFieldBit{});
	}

	template<typename TFunc, typename TV, typename TFieldBit = typename details::func_fieldbit<TFunc>::gpout_t>
	inline void set_(TFunc func, TV v){
		set_(TFieldBit{}, v);
	}

	template<typename TFunc, typename TFieldBit = typename details::func_fieldbit<TFunc>::gpout_t>
	inline void toggle_(TFunc func){
		toggle_(TFieldBit{});
	}

	template<typename TFunc, typename TFieldBit = typename details::func_fieldbit<TFunc>::gpin_t>
	constexpr auto inline read_(TFunc func){
		return read_(TFieldBit{});
	}

	template<typename TFunc, typename TFieldBit = typename details::func_fieldbit<TFunc>::gpin_t>
	constexpr auto inline wait_lo(TFunc func){
		wait_lo(TFieldBit{});
	}

	template<typename TFunc, typename TFieldBit = typename details::func_fieldbit<TFunc>::gpin_t>
	constexpr auto inline wait_hi(TFunc func){
		wait_hi(TFieldBit{});
	}
}

#endif