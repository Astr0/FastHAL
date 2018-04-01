/*
 * portinfo.h
 *
 * Created: 06.02.2013 0:58:06
 *  Author: astr0
 */ 


#ifndef FH_FIELDS_INFO_H_
#define FH_FIELDS_INFO_H_

#include "../utils/mask.hpp"
#include "../std/type_traits.hpp"
#include "../std/std_fake.hpp"

namespace fasthal
{
	namespace details{
		template<class TField>
		struct is_field_impl: std::false_type{};
	}

	template<class TField>
	using field_data_type = decltype(std::declval<TField>().read());

	template<class TField>
	using field_mask_types = bitmask_types<field_data_type<TField>>;

	template<class TField>
	using field_mask_type = typename field_mask_types<TField>::masktype_t;

	template <class TField>
	struct field_width: std::integral_constant<unsigned, sizeof(field_data_type<TField>) * 8>{};

	template <class TField>
	using is_field = details::is_field_impl<std::base_type_t<TField>>;

	template <class TField, class T>
	using enable_if_field_t = std::enable_if_t<is_field<TField>::value, T>;

	template <class TField>
	using enable_if_field_c = std::enable_if_c<is_field<TField>::value>;
}

#endif /* PORTINFO_H_ */