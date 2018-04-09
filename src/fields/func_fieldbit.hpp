#ifndef FH_FUNCS_FIELDBIT_H_
#define FH_FUNCS_FIELDBIT_H_

#include "fieldbit.hpp"

namespace fasthal{
    namespace details{
        template<class TFunc>
		struct func_fieldbit_impl {};

		template<class TFunc>
		using func_fieldbit = func_fieldbit_impl<std::base_type_t<TFunc>>;

		template<class TFieldBit>
		struct func_fieldbit_enable {
			using enable_t = TFieldBit;
		};

		template<class TFieldBit>
		struct func_fieldbit_ready {
			using ready_t = TFieldBit;
		};

		template<class TFieldBit>
		struct func_fieldbit_reset {
			using reset_t = TFieldBit;
		};

		template<class TFieldBit>
		struct func_fieldbit_ready_reset:
			func_fieldbit_ready<TFieldBit>,
			func_fieldbit_reset<TFieldBit>{};
    }

    // FUNCS
	// enable
	template<typename TFunc, typename TValue, typename TFieldBit = typename details::func_fieldbit<TFunc>::enable_t>
	constexpr auto inline enable(TFunc func, TValue v) { return set(TFieldBit{}, v); }
	template<typename TFunc, typename TValue, typename TFieldBit = typename details::func_fieldbit<TFunc>::enable_t>
	inline void enable_(TFunc func, TValue v) { set_(TFieldBit{}, v); }
	
	template<typename TFunc, typename TFieldBit = typename details::func_fieldbit<TFunc>::enable_t>
	constexpr auto inline enable(TFunc func) { return set(TFieldBit{}); }
	template<typename TFunc, typename TFieldBit = typename details::func_fieldbit<TFunc>::enable_t>
	inline void enable_(TFunc func){ set_(TFieldBit{}); }
	
	template<typename TFunc, typename TFieldBit = typename details::func_fieldbit<TFunc>::enable_t>
	constexpr auto inline disable(TFunc func){ return clear(TFieldBit{}); }
	template<typename TFunc, typename TFieldBit = typename details::func_fieldbit<TFunc>::enable_t>
	inline void disable_(TFunc func){clear_(TFieldBit{});}
	
	template<typename TFunc, typename TFieldBit = typename details::func_fieldbit<TFunc>::enable_t>
	constexpr auto inline enabled_(TFunc func){ return read_(TFieldBit{}); }	
	
	// ready
	template<typename TFunc, typename TFieldBit = typename details::func_fieldbit<TFunc>::ready_t>
	constexpr auto inline ready_(TFunc func){ return read_(TFieldBit{}); }	

	// reset
	template<typename TFunc, typename TFieldBit = typename details::func_fieldbit<TFunc>::reset_t>
	constexpr auto inline reset(TFunc func){ return set(TFieldBit{}); }	

	template<typename TFunc, typename TFieldBit = typename details::func_fieldbit<TFunc>::reset_t>
	inline void reset_(TFunc func){ set_(TFieldBit{}); }	
}

#endif