#ifndef FH_VFIELD_IMPL_H_
#define FH_VFIELD_IMPL_H_

#include "../mp/brigand_ex.hpp"
#include "../std/std_types.hpp"
#include "../utils/mask.hpp"
#include "actions_ex.hpp"
#include "nullfield.hpp"

namespace fasthal{
    namespace details{
        // hold fieldbit and position
        template<class TFieldBit, std::uint32_t VPos>
        struct fieldbit_pos{
            using field_bit_t = TFieldBit;
            static constexpr auto position = VPos;
        };

        template<class TFieldBit, class TPos>
        using make_fieldbit_pos = fieldbit_pos<TFieldBit, TPos::value>;
        
        template<class TFieldBitPos>
        using is_inverted_field_bit_pos = brigand::bool_<TFieldBitPos::field_bit_t::inverted>;

        template <class TFieldBitPos>
        using fieldbit_pos_masktype = decltype(TFieldBitPos::field_bit_t::mask);

        template <class TFieldBitPos>
        using fieldbit_pos_mask = integral_constant<fieldbit_pos_masktype<TFieldBitPos>, TFieldBitPos::field_bit_t::mask>;

        // make field mask from list
        template<typename TMask, template<class> class TGetMask, class TFieldBitsPos>
        using make_fieldbits_list_mask = brigand::fold<
            brigand::transform<TFieldBitsPos, brigand::bind<TGetMask, brigand::_1>>,
            integral_constant<TMask,TMask{}>, brigand::bitor_<brigand::_state, brigand::_element>>;

        // field mask for fieldbits_pos list
        template<typename TMask, class TFieldBitsPos>
        using make_fieldbits_pos_mask = make_fieldbits_list_mask<TMask, fieldbit_pos_mask, TFieldBitsPos>;

        template <class TFieldBitPos>
        using fieldbit_pos_inversion_mask = integral_constant<fieldbit_pos_masktype<TFieldBitPos>,
            TFieldBitPos::field_bit_t::inverted ? TFieldBitPos::field_bit_t::mask : fieldbit_pos_masktype<TFieldBitPos> {}>;

        // inverted bits mask
        template<typename TMask, class TFieldBitsPos>
        using make_fieldbits_pos_inversion_mask = make_fieldbits_list_mask<TMask, fieldbit_pos_inversion_mask, TFieldBitsPos>;

        // dummy inversion mask (always default)
        template<typename TMask, class TFieldBitsPos>
        using make_fieldbits_pos_false_inversion_mask = integral_constant<TMask, TMask{}>;  

        // Number of sequental bits
        template <class... TFieldBitsPos>
        struct get_fieldbit_pos_serial_count
		{
			static constexpr int pinNumber = -1;
			static constexpr int bitPosition = -1;
			static constexpr int value = 0;
		};

		template <class TFieldBitPos, class... R>
		struct get_fieldbit_pos_serial_count<TFieldBitPos, R...>
		{
            using next_t = get_fieldbit_pos_serial_count<R...>;
			static constexpr int pinNumber = TFieldBitPos::field_bit_t::number;
			static constexpr int bitPosition = TFieldBitPos::position;
			static constexpr int value =
				(pinNumber == next_t::pinNumber - 1 && bitPosition == next_t::bitPosition - 1) 
                    ? next_t::value + 1 
                    : 1;
		};              

        template<class TFieldBitsPos>
        using fieldbit_pos_match = brigand::bool_<TFieldBitsPos::position == TFieldBitsPos::field_bit_t::number>;

        template<class... TFieldBits>
        struct vfield_impl
        {
            // calc data type and mask type
            static constexpr auto bits_count = brigand::count<TFieldBits...>::value;
            using datatype_t = bytes_bitmask_type<maskSizeInBytes(bits_count)>;
            using masktype_t = bitmask_type<datatype_t>;

            // add position to each fieldbit
            using all_fieldbits_pos_t = brigand::transform<
                brigand::list<TFieldBits...>, 
                brigand::make_sequence<brigand::uint32_t<0>, bits_count>, 
                brigand::bind<make_fieldbit_pos, brigand::_1, brigand::_2>>;

            // iterate through bits on one field and do something great
            template<class... TFieldBitsPos>
            struct fieldbits_iterator{
                template<typename TValueType, typename TFieldType, template<typename, class...> class TInversionMask>
                static inline constexpr void appendValue(TValueType value, TFieldType& result)
                {
                }

                static inline constexpr void appendReadValue(datatype_t fieldValue, datatype_t& result)
                {                    
                }
            };

            template<class TFieldBitPos, class... R>
            struct fieldbits_iterator<TFieldBitPos, R...>{
                using field_bit_t = typename TFieldBitPos::field_bit_t;
                using field_t = typename field_bit_t::field_t;
                using field_datatype_t = field_data_type<field_t>;
                using field_masktype_t = field_mask_type<field_t>;

                using next_t = fieldbits_iterator<R...>;
                static constexpr auto datatype_mask = bitmask_types<datatype_t>::bitToMask(TFieldBitPos::position);
                using my_fieldbits_pos_t = brigand::list<TFieldBitPos, R...>;
                using my_transparent_fieldbits_pos_t = brigand::filter<my_fieldbits_pos_t, brigand::bind<fieldbit_pos_match, brigand::_1>>;
                using my_nottransparent_fieldbits_pos_t = brigand::remove_if<my_fieldbits_pos_t, brigand::bind<fieldbit_pos_match, brigand::_1>>;
                static constexpr auto my_transparent_count = brigand::size<my_transparent_fieldbits_pos_t>::value;
                
                static constexpr auto my_serial_count = get_fieldbit_pos_serial_count<TFieldBitPos, R...>::value;
                using my_serial_pins_split_t = brigand::split_at<my_fieldbits_pos_t, integral_constant<int, my_serial_count>>;
                using my_serial_pins_t = brigand::at_c<my_serial_pins_split_t, 0>;
                using my_nonserial_pins_t = brigand::at_c<my_serial_pins_split_t, 1>;

                template<typename TValueType, typename TFieldType, template<typename, class...> class TInversionMask>
                static inline constexpr void appendValue(TValueType value, TFieldType& result)
                {
                    if (my_transparent_count > 1)
                    {
                        // 1 to 1 bits
                        result |= (value & make_fieldbits_pos_mask<TFieldType, my_transparent_fieldbits_pos_t>::value) ^
                                TInversionMask<TFieldType, my_transparent_fieldbits_pos_t>::value;

                        return brigand::unpack<my_nottransparent_fieldbits_pos_t, fieldbits_iterator>::template appendValue<TValueType, TFieldType, TInversionMask>(value, result);
                    }

                    if (my_serial_count > 1)
                    { 
                        // shited bits
                        result |= (fasthal::shift<field_bit_t::number - TFieldBitPos::position>(value) & make_fieldbits_pos_mask<TFieldType, my_serial_pins_t>::value) 
                            ^ TInversionMask<TFieldType, my_serial_pins_t>::value;

                        return brigand::unpack<my_nonserial_pins_t, fieldbits_iterator>::template appendValue<TValueType, TFieldType, TInversionMask>(value, result);
                    }

                    // general case - check individual bit
                    if(TInversionMask<TFieldType, brigand::list<TFieldBitPos>>::value)
                    {
                        if(!(value & datatype_mask))
                            result |= field_bit_t::mask;
                    }
                    else
                    {
                        if(value & datatype_mask)
                            result |= field_bit_t::mask;                        
                    }

                    next_t::template appendValue<TValueType, TFieldType, TInversionMask>(value, result);
                }
                
                static inline constexpr void appendReadValue(field_datatype_t fieldValue, datatype_t& result)
                {
                    if (my_transparent_count > 1)
                    {
                        result |= (fieldValue &
                                make_fieldbits_pos_mask<field_masktype_t, my_transparent_fieldbits_pos_t>::value) ^
                                make_fieldbits_pos_inversion_mask<field_masktype_t, my_transparent_fieldbits_pos_t>::value;

                        return brigand::unpack<my_nottransparent_fieldbits_pos_t, fieldbits_iterator>::appendReadValue(fieldValue, result);
                    }

                    if (my_serial_count > 1)
                    {
                        result |= fasthal::shift<TFieldBitPos::position - field_bit_t::number>(
                            (fieldValue ^ make_fieldbits_pos_inversion_mask<field_masktype_t, my_serial_pins_t>::value) & make_fieldbits_pos_mask<field_masktype_t, my_serial_pins_t>::value);                            

                        return brigand::unpack<my_nonserial_pins_t, fieldbits_iterator>::appendReadValue(fieldValue, result);
                    }

                    if(make_fieldbits_pos_inversion_mask<field_masktype_t, brigand::list<TFieldBitPos>>::value) {
                        if(!(fieldValue & field_bit_t::mask))
                            result |= datatype_mask;
                    }else {
                        if(fieldValue & field_bit_t::mask)
                            result |= datatype_mask;
                    }

                    next_t::appendReadValue(fieldValue, result);
                }
            };

            template<typename TField>
            // field fieldbits iterator - controls field stuff
            struct field_processor {
                static constexpr auto field = TField{};
                using field_masktype_t = field_mask_type<TField>;
                using field_datatype_t = field_data_type<TField>;

                // check if field bit belongs to TField
                template <typename TFieldBitPos>
                using is_my_field_bit_pos = std::is_same<typename TFieldBitPos::field_bit_t::field_t, TField>;

                // only TField field bits
                using my_fieldbits_pos_t = brigand::filter<all_fieldbits_pos_t, brigand::bind<is_my_field_bit_pos, brigand::_1>>;
                static constexpr auto my_fieldbits_count = brigand::size<my_fieldbits_pos_t>::value;
                using my_inverted_fieldbits_pos_t = brigand::filter<my_fieldbits_pos_t, brigand::bind<is_inverted_field_bit_pos, brigand::_1>>;
                using my_noninverted_fieldbits_pos_t = brigand::remove_if<my_fieldbits_pos_t, brigand::bind<is_inverted_field_bit_pos, brigand::_1>>;
                static constexpr auto my_inveted_count = brigand::size<my_inverted_fieldbits_pos_t>::value;

                template<class TIteratorFieldBits = my_fieldbits_pos_t,
                    typename T,
                    typename TIterator = brigand::unpack<TIteratorFieldBits, fieldbits_iterator>>                    
                static constexpr auto appendWriteValue(T value){
                    auto result = field_datatype_t{};
                    TIterator::template appendValue<datatype_t, field_datatype_t, make_fieldbits_pos_inversion_mask>(value, result);
                    return result;
                }

                template<class TIteratorFieldBits = my_fieldbits_pos_t,
                    typename T, T V,
                    typename TIterator = brigand::unpack<TIteratorFieldBits, fieldbits_iterator>>                    
                static constexpr auto appendWriteValue(integral_constant<T, V> value){
                    using result_t = integral_constant<field_datatype_t, appendWriteValue<TIteratorFieldBits>(V)>;
                    return result_t{};
                }

                template<class TIteratorFieldBits = my_fieldbits_pos_t,
                    typename T,
                    typename TIterator = brigand::unpack<TIteratorFieldBits, fieldbits_iterator>>                    
                static constexpr inline auto appendMaskValue(T value)
                {
                    auto result = field_masktype_t{};
                    TIterator::template appendValue<masktype_t, field_masktype_t, make_fieldbits_pos_false_inversion_mask>(value, result);
                    return result;
                }

                template<class TIteratorFieldBits = my_fieldbits_pos_t,
                    typename T, T V,
                    typename TIterator = brigand::unpack<TIteratorFieldBits, fieldbits_iterator>>                    
                static constexpr auto appendMaskValue(integral_constant<T, V> value){
                    using result_t = integral_constant<field_datatype_t, appendMaskValue<TIteratorFieldBits>(V)>;
                    return result_t{};
                }

                template<bool whole_field = my_fieldbits_count == (int)field_width<TField>(), bool dummy = false>
                struct write_helper{
                    template<typename T>
                    static constexpr auto write(T value){
                        // full field - apply inversion mask on value and write
                        return write_a(field, appendWriteValue(value));
                    }
                };

                template<bool dummy>
                struct write_helper<false, dummy>{
                    template<typename T>
                    static constexpr auto write(T value){
                        // partial field
                        // clear everything that belongs to us and set only result
                        constexpr auto my_mask = make_fieldbits_pos_mask<field_masktype_t, my_fieldbits_pos_t>{};
                        
                        return clear_set_a(field, my_mask, appendWriteValue(value));
                    }
                };

                template<typename T>
                static constexpr auto write(T value){
                    return write_helper<>::write(value);
                }	

                template<decltype(my_inveted_count) inverted = my_inveted_count, bool dummy = false>
                struct clear_set_helper{
                    static constexpr auto inversion_mask = make_fieldbits_pos_inversion_mask<field_masktype_t, my_fieldbits_pos_t>::value;

                    template<typename TV1, typename TV2>
                    static constexpr auto apply_inversion_mask(TV1 v1, TV2 v2){
                         return (v1 & ~inversion_mask) | (v2 & inversion_mask);
                    }

                    template<typename TV1, typename TV2, TV1 V1, TV2 V2>
                    static constexpr auto apply_inversion_mask(
                        integral_constant<TV1, V1> v1,
                        integral_constant<TV2, V2> v2){
                        using result_t = integral_constant<field_masktype_t, apply_inversion_mask(V1, V2)>;
                        return result_t{};
                    }

                    // some bits inverted, some not
                    template<typename TClear, typename TSet>
                    static constexpr auto clear_set(TClear clearMask, TSet setMask){
                        // TODO: Think how to optimize using integral_constant
                        auto resultC = appendMaskValue(clearMask);
                        auto resultS = appendMaskValue(setMask);

                        // clear - not inverted clear + inverted set
                        auto clearFieldMask = apply_inversion_mask(resultC, resultS);
                        // set - not inverted set + inverted clear
                        auto setFieldMask = apply_inversion_mask(resultS, resultC);

                        return clear_set_a(field, clearFieldMask, setFieldMask);
                    }

                    template<typename T>
                    static constexpr auto set(T value){
                        auto clearMask = appendMaskValue<my_inverted_fieldbits_pos_t>(value);
                        auto setMask = appendMaskValue<my_noninverted_fieldbits_pos_t>(value);
                        return clear_set_a(field, clearMask, setMask);
                    }

                    template<typename T>
                    static constexpr auto clear(T value){
                        auto clearMask = appendMaskValue<my_noninverted_fieldbits_pos_t>(value);
                        auto setMask = appendMaskValue<my_inverted_fieldbits_pos_t>(value);
                        return clear_set_a(field, clearMask, setMask);
                    }                    
                };

                template<bool dummy>
                struct clear_set_helper<0, dummy>
                {
                    // all bits not inverted
                    template<typename TClear, typename TSet>                    
                    static constexpr auto clear_set(TClear clearMask, TSet setMask){
                        return clear_set_a(field, appendMaskValue(clearMask), appendMaskValue(setMask));
                    }

                    template<typename T>
                    static constexpr auto set(T value){
                        return set_a(field, appendMaskValue(value));			
                    }

                    template<typename T>
                    static constexpr auto clear(T value){
                        return clear_a(field, appendMaskValue(value));			
                    }                    
                };

                template<bool dummy>
                struct clear_set_helper<my_fieldbits_count, dummy>
                {
                    // all bits inverted
                    template<typename TClear, typename TSet>
                    static constexpr auto clear_set(TClear clearMask, TSet setMask){
                        return clear_set_a(field, appendMaskValue(setMask), appendMaskValue(clearMask));
                    }

                    template<typename T>
                    static constexpr auto set(T value){
                        return clear_a(field, appendMaskValue(value));
                    }                    

                    template<typename T>
                    static constexpr auto clear(T value){
                        return set_a(field, appendMaskValue(value));					
                    }
                };

                template<typename TClear, typename TSet>
                static constexpr auto clear_set(TClear clearMask, TSet setMask){
                    return clear_set_helper<>::clear_set(clearMask, setMask);
                }

                template<typename T>
                static constexpr auto set(T value){
                    return clear_set_helper<>::set(value);
                }

                template<typename T>
                static constexpr auto clear(T value){
                   return clear_set_helper<>::clear(value);
                }
                template<typename T>
                static constexpr auto toggle(T value){
                    // Ignore inverted - toggle does not care
                    return toggle_a(field, appendMaskValue(value));
                }

                static constexpr auto read(){
                    return read_a(field);
                }

                template<typename TReadResult>
                static void read(datatype_t& result, TReadResult read){
                    auto fieldValue = get_a(field, read);
				    brigand::unpack<my_fieldbits_pos_t, fieldbits_iterator>::appendReadValue(fieldValue, result);
                }
            };

            template<class... TActions>
            static constexpr auto combine_vfield_actions(TActions... actions){
                return combine_a(actions...);
            }

            // field_iterator
            template<typename... TFields>
            struct fields_iterator{
                template<typename T>
                static constexpr auto write_a(T value){
                    return combine_vfield_actions(field_processor<TFields>::write(value)...);
                }

                template<typename TClear, typename TSet>
                static constexpr auto  clear_set_a(TClear clearMask, TSet setMask){
                    return combine_vfield_actions(field_processor<TFields>::clear_set(clearMask, setMask)...);
                }
                template<typename T>
                static constexpr auto  set_a(T value){
                    return combine_vfield_actions(field_processor<TFields>::set(value)...);
                }
                template<typename T>
                static constexpr auto  clear_a(T value){
                    return combine_vfield_actions(field_processor<TFields>::clear(value)...);
                }
                template<typename T>
                static constexpr auto  toggle_a(T value){
                    return combine_vfield_actions(field_processor<TFields>::toggle(value)...);
                }
                static constexpr auto read_a(){
                    return combine_vfield_actions(field_processor<TFields>::read()...);
                }
                template<class TReadResult>
                static datatype_t read(TReadResult read){
                    auto result = datatype_t {};
                    (field_processor<TFields>::read(result, read), ...);
                    return result;
                }
                static datatype_t read() { 
                    return read(apply(read_a()));
                }
            };

            template<class TFieldBit>
            using get_fieldbit_field = typename TFieldBit::field_t;

            // distinct fields affected by fieldbits
   			using fields_t = brigand::remove<
                brigand::no_duplicates< 
                    brigand::transform<brigand::list<TFieldBits...>, brigand::bind<get_fieldbit_field, brigand::_1>>>,
                null_field>;

            using impl_t = brigand::unpack<fields_t, fields_iterator>;

        };        
    }
}

#endif