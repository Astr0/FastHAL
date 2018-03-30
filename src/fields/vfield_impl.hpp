#ifndef FH_VFIELD_IMPL_H_
#define FH_VFIELD_IMPL_H_

#include "../mp/brigand_ex.hpp"
#include "../utils/mask.hpp"

namespace fasthal{
    namespace details{
        // hold fieldbit and position
        template<class TFieldBit, unsigned VPos>
        struct fieldbit_pos{
            using field_bit_t = TFieldBit;
            static constexpr auto position = VPos;
        };
        
        // make list with position
        template<unsigned N, class... TFieldBits>
        struct make_fieldbit_pos_list{
            using type = brigand::list<>;
        };

        template<unsigned N, class TFieldBit, class... TRest>
        struct make_fieldbit_pos_list<N, TFieldBit, TRest...>{
            using fieldbit_pos_t = fieldbit_pos<TFieldBit, N>;
            using type = brigand::push_front<
                typename make_fieldbit_pos_list<N + 1, TRest...>::type,
                fieldbit_pos_t>;
        };


        template<class TFieldBit>
        struct get_fieldbit_field{
            using type = typename TFieldBit::field_t;
        };

        template<class... TFieldBits>
        struct vfield_impl
        {
            // calc data type and mask type
            using datatype_t = bytes_bitmask_type<maskSizeInBytes(brigand::count<TFieldBits...>::value)>;
            using masktype_t = bitmask_type<datatype_t>;

            // add position to each fieldbit
            using all_fieldbits_pos_t = typename make_fieldbit_pos_list<0, TFieldBits...>::type;

            // distinct fields affected by fieldbits
   			using fields_t = brigand::no_duplicates< 
                brigand::transform<brigand::list<TFieldBits...>, details::get_fieldbit_field<brigand::_1>> >;


            template<typename TField>
            struct fieldbits_iterator{

                // check if field bit belongs to TField
                template <typename TFieldBitPos>
                struct is_my_field_bit_pos: std::is_same<typename TFieldBitPos::field_bit_t::field_t, TField>{};

                // only TField field bits
                using my_fieldbits_pos_t = brigand::filter<all_fieldbits_pos_t, is_my_field_bit_pos<brigand::_1>>;

                static void write(datatype_t value){}		
                static void clearAndSet(masktype_t clearMask, masktype_t setMask){}
                static void set(masktype_t value){}
                static void clear(masktype_t value){}
                static void toggle(masktype_t value){}

                static void read(datatype_t& result){}
            };

            // field_iterator - catch all dummy
            template<typename... TFields>
            struct field_iterator{
                static void write(datatype_t value){}		
                static void clearAndSet(masktype_t clearMask, masktype_t setMask){}
                static void set(masktype_t value){}
                static void clear(masktype_t value){}
                static void toggle(masktype_t value){}

                static void read(datatype_t& result){}
            };
            
            // field_iterator - have some field
            template<class TField, class... TRestFields>
            struct field_iterator<TField, TRestFields...>{
                using bits_iterator_t = fieldbits_iterator<TField>;
                using next_t = field_iterator<TRestFields...>;

                static void write(datatype_t value){
                    bits_iterator_t::write(value);
                    next_t::write(value);
                }		
                static void clearAndSet(masktype_t clearMask, masktype_t setMask){
                    bits_iterator_t::clearAndSet(clearMask, setMask);
                    next_t::clearAndSet(clearMask, setMask);
                }
                static void set(masktype_t value){
                    bits_iterator_t::set(value);
                    next_t::set(value);
                }
                static void clear(masktype_t value){
                    bits_iterator_t::clear(value);
                    next_t::clear(value);
                }
                static void toggle(masktype_t value){
                    bits_iterator_t::toggle(value);
                    next_t::toggle(value);
                }

                static void read(datatype_t& result) { 
                    bits_iterator_t::read(result);
                    next_t::read(result);
                }
            };

            template<class TFields>
            struct make_field_iterator;

            template<class... TFields>
            struct make_field_iterator<brigand::list<TFields...>>{
                using type = field_iterator<TFields...>;
            };

            using impl_t = typename make_field_iterator<fields_t>::type;
        };
    }
}

#endif