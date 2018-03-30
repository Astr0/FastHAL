#ifndef FH_VFIELD_IMPL_H_
#define FH_VFIELD_IMPL_H_

#include "../mp/brigand_ex.hpp"
#include "../utils/mask.hpp"

namespace fasthal{
    namespace details{
        using namespace brigand;

        // hold fieldbit and position
        template<class TFieldBit, unsigned VPos>
        struct fieldbit_pos{
            using field_bit_t = TFieldBit;
            static constexpr auto position = VPos;
        };
        
        // make list with position
        template<unsigned N, class... TFieldBits>
        struct make_fieldbit_pos_list{
            using type = list<>;
        };

        template<unsigned N, class TFieldBit, class... TRest>
        struct make_fieldbit_pos_list<N, TFieldBit, TRest...>{
            using fieldbit_pos_t = fieldbit_pos<TFieldBit, N>;
            using type = push_front<
                typename make_fieldbit_pos_list<N + 1, TRest...>::type,
                fieldbit_pos_t>;
        };

        template<class TFieldBitPos>
        using is_inverted_field_bit_pos = bool_<TFieldBitPos::field_bit_t::inverted>;

        template <class TFieldBitPos>
        using fieldbit_pos_masktype = decltype(TFieldBitPos::field_bit_t::mask);

        template <class TFieldBitPos>
        using fieldbit_pos_mask = integral_constant<fieldbit_pos_masktype<TFieldBitPos>, TFieldBitPos::field_bit_t::mask>;

        // field mask for fieldbits_pos list
        template<typename TMask, class TFieldBitsPos>
        using make_fieldbits_pos_mask = fold<
            transform<TFieldBitsPos, bind<fieldbit_pos_mask, _1>>,
            integral_constant<TMask,TMask{}>, bitor_<_state, _element>>;

        template <class TFieldBitPos>
        using fieldbit_pos_inversion_mask = integral_constant<fieldbit_pos_masktype<TFieldBitPos>,
            TFieldBitPos::field_bit_t::inverted ? TFieldBitPos::field_bit_t::mask : fieldbit_pos_masktype<TFieldBitPos> {}>;

        // inverted bits mask
        template<typename TMask, class TFieldBitsPos>
        using make_fieldbits_pos_inversion_mask = fold<
            transform<TFieldBitsPos, bind<fieldbit_pos_inversion_mask, _1>>,
            integral_constant<TMask,TMask{}>, bitor_<_state, _element>>;

        // dummy inversion mask (always default)
        template<typename TMask, class TFieldBitsPos>
        using make_fieldbits_pos_false_inversion_mask = integral_constant<TMask, TMask{}>;  

        // Number of sequental bits
		template <class TFieldBitsPos> 
        struct get_fieldbit_pos_serial_count;

        template <class... TFieldBitsPos>
        struct get_fieldbit_pos_serial_count<list<TFieldBitsPos...>>
		{
			static constexpr int pinNumber = -1;
			static constexpr int bitPosition = -1;
			static constexpr int value = 0;
		};

		template <class TFieldBitPos, class... R>
		struct get_fieldbit_pos_serial_count<list<TFieldBitPos, R...>>
		{
            using next_t = get_fieldbit_pos_serial_count<list<R...>>;
			static constexpr int pinNumber = TFieldBitPos::field_bit_t::number;
			static constexpr int bitPosition = TFieldBitPos::position;
			static constexpr int value =
				(pinNumber == next_t::pinNumber - 1 && bitPosition == next_t::bitPosition - 1) 
                    ? next_t::value + 1 
                    : 1;
		};              

        template<class TFieldBitsPos>
        using fieldbit_pos_match = bool_<TFieldBitsPos::position == TFieldBitsPos::field_bit_t::number>;

        template<class... TFieldBits>
        struct vfield_impl
        {
            // calc data type and mask type
            using datatype_t = bytes_bitmask_type<maskSizeInBytes(count<TFieldBits...>::value)>;
            using masktype_t = bitmask_type<datatype_t>;

            // add position to each fieldbit
            using all_fieldbits_pos_t = typename make_fieldbit_pos_list<0, TFieldBits...>::type;

            // iterate through bits on one field and do something great
            template <class TFieldBitsPos>
            struct fieldbits_iterator;

            template<class... TFieldBitsPos>
            struct fieldbits_iterator<list<TFieldBitsPos...>>{
                template<typename TValueType, typename TFieldType, template<class, class> class TInversionMask>
                static inline void appendValue(TValueType value, TFieldType& result)
                {
                }

                static inline void appendReadValue(datatype_t fieldValue, datatype_t& result)
                {                    
                }
            };

            template<class TFieldBitPos, class... R>
            struct fieldbits_iterator<list<TFieldBitPos, R...>>{
                using field_bit_t = typename TFieldBitPos::field_bit_t;
                using field_t = typename field_bit_t::field_t;
                using field_datatype_t = field_data_type<field_t>;
                using field_masktype_t = field_mask_type<field_t>;

                using next_t = fieldbits_iterator<list<R...>>;
                static constexpr auto datatype_mask = bitmask_types<datatype_t>::bitToMask(TFieldBitPos::position);
                using my_fieldbits_pos_t = list<TFieldBitPos, R...>;
                using my_transparent_fieldbits_pos_t = filter<my_fieldbits_pos_t, bind<fieldbit_pos_match, _1>>;
                using my_nottransparent_fieldbits_pos_t = remove_if<my_fieldbits_pos_t, bind<fieldbit_pos_match, _1>>;
                static constexpr auto my_transparent_count = size<my_transparent_fieldbits_pos_t>::value;
                
                static constexpr auto my_serial_count = get_fieldbit_pos_serial_count<my_fieldbits_pos_t>::value;
                using my_serial_pins_split_t = split_at<my_fieldbits_pos_t, integral_constant<int, my_serial_count>>;
                using my_serial_pins_t = at_c<my_serial_pins_split_t, 0>;
                using my_nonserial_pins_t = at_c<my_serial_pins_split_t, 1>;

                template<typename TValueType, typename TFieldType, template<class, class> class TInversionMask>
                static inline void appendValue(TValueType value, TFieldType& result)
                {
                    if (my_transparent_count > 1)
                    {
                        // 1 to 1 bits
                        result |= (value & make_fieldbits_pos_mask<TFieldType, my_transparent_fieldbits_pos_t>::value) ^
                                TInversionMask<TFieldType, my_transparent_fieldbits_pos_t>::value;

                        return fieldbits_iterator<my_nottransparent_fieldbits_pos_t>::template appendValue<TValueType, TFieldType, TInversionMask>(value, result);
                    }

                    if (my_serial_count > 1)
                    { 
                        // shited bits
                        result |= (fasthal::shift<field_bit_t::number - TFieldBitPos::position>(value) & make_fieldbits_pos_mask<TFieldType, my_serial_pins_t>::value) 
                            ^ TInversionMask<TFieldType, my_serial_pins_t>::value;

                        return fieldbits_iterator<my_nonserial_pins_t>::template appendValue<TValueType, TFieldType, TInversionMask>(value, result);
                    }

                    // general case - check individual bit
                    if(TInversionMask<TFieldType, list<TFieldBitPos>>::value)
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
                
                static inline void appendReadValue(field_datatype_t fieldValue, datatype_t& result)
                {
                    if (my_transparent_count > 1)
                    {
                        result |= (fieldValue &
                                make_fieldbits_pos_mask<field_masktype_t, my_transparent_fieldbits_pos_t>::value) ^
                                make_fieldbits_pos_inversion_mask<field_masktype_t, my_transparent_fieldbits_pos_t>::value;

                        return fieldbits_iterator<my_nottransparent_fieldbits_pos_t>::appendReadValue(fieldValue, result);
                    }

                    if (my_serial_count > 1)
                    {
                        result |= fasthal::shift<TFieldBitPos::position - field_bit_t::number>(
                            (fieldValue ^ make_fieldbits_pos_inversion_mask<field_masktype_t, my_serial_pins_t>::value) & make_fieldbits_pos_mask<field_masktype_t, my_serial_pins_t>::value);                            

                        return fieldbits_iterator<my_nonserial_pins_t>::appendReadValue(fieldValue, result);
                    }

                    if(make_fieldbits_pos_inversion_mask<field_masktype_t, list<TFieldBitPos>>::value) {
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
            struct field_processor{
                static constexpr auto field = TField{};
                using field_masktype_t = field_mask_type<TField>;
                using field_datatype_t = field_data_type<TField>;

                // check if field bit belongs to TField
                template <typename TFieldBitPos>
                using is_my_field_bit_pos = std::is_same<typename TFieldBitPos::field_bit_t::field_t, TField>;

                // only TField field bits
                using my_fieldbits_pos_t = filter<all_fieldbits_pos_t, bind<is_my_field_bit_pos, _1>>;
                static constexpr auto my_fieldbits_count = size<my_fieldbits_pos_t>::value;
                using my_inverted_fieldbits_pos_t = filter<my_fieldbits_pos_t, bind<is_inverted_field_bit_pos, _1>>;
                using my_noninverted_fieldbits_pos_t = remove_if<my_fieldbits_pos_t, bind<is_inverted_field_bit_pos, _1>>;
                static constexpr auto my_inveted_count = size<my_inverted_fieldbits_pos_t>::value;

                // Iterator over TField field bits
                using bit_iterator_t = fieldbits_iterator<my_fieldbits_pos_t>;

                template<class TIterator = bit_iterator_t>
                static inline field_datatype_t appendWriteValue(datatype_t value)
                {
                    auto result = field_datatype_t{};
                    TIterator::template appendValue<datatype_t, field_datatype_t, make_fieldbits_pos_inversion_mask>(value, result);
                    return result;
                }

                template<class TIterator = bit_iterator_t>
                static inline field_masktype_t appendMaskValue(masktype_t value)
                {
                    auto result = field_masktype_t{};
                    TIterator::template appendValue<masktype_t, field_masktype_t, make_fieldbits_pos_false_inversion_mask>(value, result);
                    return result;
                }

                static void write(datatype_t value){
                    // Apply inversion mask on value
                    auto result = appendWriteValue(value);

                    if(my_fieldbits_count == (int)field_width<TField>()) {// whole Field
                        fasthal::write(field, result);
                    } else {
                        constexpr auto my_mask = make_fieldbits_pos_mask<field_masktype_t, my_fieldbits_pos_t>::value;

                        fasthal::clearAndSet(field, my_mask, result);
                    }
                }	

                static void clearAndSet(masktype_t clearMask, masktype_t setMask){
                    auto resultC = appendMaskValue(clearMask);
                    auto resultS = appendMaskValue(setMask);
                    if (my_inveted_count == 0) {
                        // All not inverted
                        fasthal::clearAndSet(field, resultC, resultS);
                    } else if (my_inveted_count == my_fieldbits_count) {
                        // All inverted
                        fasthal::clearAndSet(field, resultS, resultC);				
                    } else {
                        // Mix... calculate set and clear masks...
                        constexpr auto inversionMask = make_fieldbits_pos_inversion_mask<field_masktype_t, my_fieldbits_pos_t>::value;
                        // clear - not inverted clear + inverted set
                        auto clearMask = (resultC & ~inversionMask) | (resultS & inversionMask);
                        // set - not inverted set + inverted clear
                        auto setMask = (resultS & ~inversionMask) | (resultC & inversionMask);
                        fasthal::clearAndSet(field, clearMask, setMask);					
                    }

                }
                static void set(masktype_t value){
                    if (my_inveted_count == 0) {
                        auto result = appendMaskValue(value);
                        fasthal::set(field, result);			
                    } else if (my_inveted_count == my_fieldbits_count) {
                        // Invert inverted
                        auto result = appendMaskValue(value);
                        fasthal::clear(field, result);
                    } else {
                        // Do both
                        auto clearMask = appendMaskValue<fieldbits_iterator<my_inverted_fieldbits_pos_t>>(value);
                        auto setMask = appendMaskValue<fieldbits_iterator<my_noninverted_fieldbits_pos_t>>(value);
                        fasthal::clearAndSet(field, clearMask, setMask);
                    }

                }
                static void clear(masktype_t value){
                    if (my_inveted_count == 0){
                        auto result = appendMaskValue(value);
                        fasthal::clear(field, result);			
                    } else if (my_inveted_count == my_fieldbits_count){
                        // Invert inverted
                        auto result = appendMaskValue(value);
                        fasthal::set(field, result);					
                    } else{
                        // Do both
                        auto clearMask = appendMaskValue<fieldbits_iterator<my_noninverted_fieldbits_pos_t>>(value);
                        auto setMask = appendMaskValue<fieldbits_iterator<my_inverted_fieldbits_pos_t>>(value);
                        fasthal::clearAndSet(field, clearMask, setMask);
                    }

                }
                static void toggle(masktype_t value){
                    // Ignore inverted - toggle does not care
                    auto result = appendMaskValue(value);
                    fasthal::toggle(field, result);
                }

                static void read(datatype_t& result){
                    auto fieldValue = fasthal::read(field);
				    bit_iterator_t::appendReadValue(fieldValue, result);
                }
            };

            // fields_iterator - do everything for every field
            template<typename TFields>
            struct fields_iterator;

            // field_iterator - terminator
            template<typename... TFields>
            struct fields_iterator<list<TFields...>>{
                static void write(datatype_t value){}		
                static void clearAndSet(masktype_t clearMask, masktype_t setMask){}
                static void set(masktype_t value){}
                static void clear(masktype_t value){}
                static void toggle(masktype_t value){}

                static void read(datatype_t& result){}
            };
            
            // field_iterator - have some field
            template<class TField, class... TRestFields>
            struct fields_iterator<list<TField, TRestFields...>>{
                using field_processor_t = field_processor<TField>;
                using next_t = fields_iterator<list<TRestFields...>>;

                static void write(datatype_t value){
                    field_processor_t::write(value);
                    next_t::write(value);
                }		
                static void clearAndSet(masktype_t clearMask, masktype_t setMask){
                    field_processor_t::clearAndSet(clearMask, setMask);
                    next_t::clearAndSet(clearMask, setMask);
                }
                static void set(masktype_t value){
                    field_processor_t::set(value);
                    next_t::set(value);
                }
                static void clear(masktype_t value){
                    field_processor_t::clear(value);
                    next_t::clear(value);
                }
                static void toggle(masktype_t value){
                    field_processor_t::toggle(value);
                    next_t::toggle(value);
                }

                static void read(datatype_t& result) { 
                    field_processor_t::read(result);
                    next_t::read(result);
                }
            };

            template<class TFieldBit>
            using get_fieldbit_field = typename TFieldBit::field_t;

            // distinct fields affected by fieldbits
   			using fields_t = no_duplicates< 
                transform<list<TFieldBits...>, bind<get_fieldbit_field, _1>> >;

            using impl_t = fields_iterator<fields_t>;
        };
    }
}

#endif