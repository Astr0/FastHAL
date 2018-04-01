#ifndef FH_VFIELD_H_
#define FH_VFIELD_H_

#include "info.hpp"
#include "../std/type_traits.hpp"
#include "fieldbit.hpp"
#include "vfield_impl.hpp"
#include "actions_ex.hpp"


namespace fasthal{
    // vfield - static field-like wrapper for FieldBits
	template<class... TFieldBits>
	struct vfield
	{
		static_assert(brigand::all<brigand::list<TFieldBits...>, brigand::bind<details::is_field_bit, brigand::_1>>::value, "Only field bits can be passed to make vfield");
	
		using impl_container_t = details::vfield_impl<TFieldBits...>;
		using datatype_t = typename impl_container_t::datatype_t;
		using impl_t = typename impl_container_t::impl_t;

		static void write(datatype_t value) 
		{
			impl_t::write(value);
			//apply(impl_t::write(value));
		}

		static auto read() 
		{
			return impl_t::read();
		}
	};
	namespace details{
		template<class... TFieldBits>
		struct is_field_impl<vfield<TFieldBits...>>: std::true_type{};
	}


	// create vfield
	template<class... TBits>
	constexpr decltype(auto) vField(TBits... bits){
		return vfield<TBits...>{};
	}

	// type info
	template <class... TFieldBits>
	struct field_width<vfield<TFieldBits...>>: brigand::count<TFieldBits...>{};

	// create vfield fieldBit - return passed field bit
	template<unsigned VNumber, class... TFieldBits>
	constexpr decltype(auto) fieldBit(vfield<TFieldBits...> field){
		return brigand::at_c<brigand::list<TFieldBits...>, VNumber> {};
	}

	// optimized actions_ex
	// template<class... TFieldBits, 
	// 	typename TField = vfield<TFieldBits...>,
	// 	typename TDataType = field_data_type<TField>>
	// static constexpr auto write_a(vfield<TFieldBits...> field, TDataType value) 
    // {
    //     return TField::impl_t::write_a(value);
    // }
    // template<std::size_t VValue, class... TFieldBits>
    // static constexpr auto write_a(vfield<TFieldBits...> field) 
    // {
    //     constexpr auto write = write_a(field, VValue);
	// 	//return ::fasthal::details::to_static_field_action<decltype(write), write.get<0>(), write.get<1>()>{};
	// 	//return FH_TO_STATIC_FIELD_ACTION(write){};
    // }

    // template<class TField, typename TMaskType = field_mask_type<TField>>
    // constexpr enable_if_field_t<TField, details::field_action<TField, details::set_field, TMaskType>>
    // set_a(TField field, TMaskType mask) 
    // {
    //     return {mask};
    // }
    // template<std::size_t VValue, class TField>
    // constexpr enable_if_field_t<TField, details::field_action_static<TField, details::set_field, VValue>>
    // set_a(TField field) 
    // {
    //     return {};
    // }

    // template<class TField, typename TMaskType = field_mask_type<TField>>
    // constexpr enable_if_field_t<TField, details::field_action<TField, details::clear_field, TMaskType>>
    // clear_a(TField field, TMaskType mask) {
    //     return {mask};
    // }
    // template<std::size_t VValue, class TField>
    // constexpr enable_if_field_t<TField, details::field_action_static<TField, details::clear_field, VValue>>
    // clear_a(TField field) 
    // {
    //     return {};
    // }

    // template<class TField, typename TMaskType = field_mask_type<TField>>
    // constexpr enable_if_field_t<TField, details::field_action_2<TField, details::clear_set_field, TMaskType>>
    // clear_set_a(TField field, TMaskType clearMask, TMaskType setMask) {
    //     return {clearMask, setMask};
    // }
    // template<std::size_t VValue, std::size_t VValue2, class TField>
    // constexpr enable_if_field_t<TField, details::field_action_static<TField, details::clear_set_field, VValue, VValue2>>
    // clear_set_a(TField field) 
    // {
    //     return {};
    // }

    // template<class TField, typename TMaskType = field_mask_type<TField>>
    // constexpr enable_if_field_t<TField, details::field_action<TField, details::toggle_field, TMaskType>>
    // toggle_a(TField field, TMaskType mask) {
    //     return {mask};
    // }
    // template<std::size_t VValue, class TField>
    // constexpr enable_if_field_t<TField, details::field_action_static<TField, details::toggle_field, VValue>>
    // toggle_a(TField field) 
    // {
    //     return {};
    // }

    // template<class TField>
    // constexpr enable_if_field_t<TField, details::field_action_static<TField, details::read_field, 0>>
    // read_a(TField field) {
    //     return {};
    // }

    // template<class TField, class... TFields>
    // constexpr enable_if_field_t<TField, field_data_type<TField>>
    // get_a(TField field, details::field_action_results_t<details::field_value<TFields>...> results)
    // {
    //     constexpr auto index = brigand::index_of<brigand::list<TFields...>, TField>::value;
    //     return mp::get<index>(results).value;
    // }	


	// optimized actions
	template<class... TFieldBits, 
		typename TField = vfield<TFieldBits...>,
		typename TMaskType = field_mask_type<TField>>
	static void set(vfield<TFieldBits...> field, TMaskType mask) 
	{
		TField::impl_t::set(mask);
	}

	template<class... TFieldBits, 
		typename TField = vfield<TFieldBits...>,
		typename TMaskType = field_mask_type<TField>>
	static void clear(vfield<TFieldBits...> field, TMaskType mask) 
	{
		TField::impl_t::clear(mask);					
	}

    template<class... TFieldBits, 
		typename TField = vfield<TFieldBits...>,
		typename TMaskType = field_mask_type<TField>>
	static void clear_set(vfield<TFieldBits...> field, TMaskType clearMask, TMaskType setMask) 
	{
		TField::impl_t::clear_set(clearMask, setMask);
	}				

	template<class... TFieldBits, 
		typename TField = vfield<TFieldBits...>,
		typename TMaskType = field_mask_type<TField>>
	static void toggle(vfield<TFieldBits...> field, TMaskType mask) 
	{
		TField::impl_t::toggle(mask);
	}
}

#endif