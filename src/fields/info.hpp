/*
 * portinfo.h
 *
 * Created: 06.02.2013 0:58:06
 *  Author: astr0
 */ 


#ifndef FH_FIELDS_INFO_H_
#define FH_FIELDS_INFO_H_

#include "../utils/mask.hpp"
#include "../sys/typeutils.h"
#include "../mp/type_traits.hpp"

namespace fasthal
{
	template<class TField>
	using field_data_type = decltype(fasthal::common::declval<TField>().read());

	template<class TField>
	using field_mask_types = bitmask_types<field_data_type<TField>>;

	template<class TField>
	using field_mask_type = typename field_mask_types<TField>::masktype_t;

	template <class TField>
	struct field_width: std::integral_constant<unsigned, sizeof(field_data_type<TField>) * 8>{};
}

#endif /* PORTINFO_H_ */