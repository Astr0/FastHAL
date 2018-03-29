/*
 * portinfo.h
 *
 * Created: 06.02.2013 0:58:06
 *  Author: astr0
 */ 


#ifndef FH_FIELDS_INFO_H_
#define FH_FIELDS_INFO_H_

#include "../sys/maskutils.h"
#include "../sys/typeutils.h"

namespace fasthal
{
	template<class TField>
	using field_data_type = decltype(fasthal::common::declval<TField>().read());

	template<class TField>
	using field_mask_types = fasthal::common::BitMaskTypes<field_data_type<TField>>;
}



#endif /* PORTINFO_H_ */