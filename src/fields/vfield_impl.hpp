#ifndef FH_VFIELD_IMPL_H_
#define FH_VFIELD_IMPL_H_

#include "../mp/brigand_ex.hpp"
#include "../utils/mask.hpp"

namespace fasthal{
    namespace details{
        template<class TFieldBit>
        struct get_fieldbit_field{
            using type = typename TFieldBit::field_t;
        };

        template<class... TFieldBits>
        struct vfield_impl
        {
            using datatype_t = bytes_bitmask_type<maskSizeInBytes(brigand::count<TFieldBits...>::value)>;
            using masktype_t = bitmask_type<datatype_t>;


            // field_iterator - catch all dummy
            template<typename... TFields>
            struct field_iterator{
                static void write(datatype_t value){}		
                static void clearAndSet(masktype_t clearMask, masktype_t setMask){}
                static void set(masktype_t value){}
                static void clear(masktype_t value){}
                static void toggle(masktype_t value){}

                static datatype_t read() { return datatype_t{}; }
            };
            
            // field_iterator - have some field
            template<class TField, class... TRestFields>
            struct field_iterator<TField, TRestFields...>{
                //using field_impl_t = vfield_field_impl<TField, TFieldBits, TDataType>;
                using next_t = field_iterator<TRestFields...>;

                static void write(datatype_t value){
                    //field_impl_t::write(value);
                    next_t::write(value);
                }		
                static void clearAndSet(masktype_t clearMask, masktype_t setMask){
                    next_t::clearAndSet(clearMask, setMask);
                }
                static void set(masktype_t value){
                    next_t::set(value);
                }
                static void clear(masktype_t value){
                    next_t::clear(value);
                }
                static void toggle(masktype_t value){
                    next_t::toggle(value);
                }

                static datatype_t read() { 
                    // TODO
                    return datatype_t{};
                }
            };

            template<class TFields>
            struct make_field_iterator;

            template<class... TFields>
            struct make_field_iterator<brigand::list<TFields...>>{
                using type = field_iterator<TFields...>;
            };

			using fields_t = brigand::no_duplicates< 
                brigand::transform<brigand::list<TFieldBits...>, details::get_fieldbit_field<brigand::_1>> >;
            using impl_t = typename make_field_iterator<fields_t>::type;
        };
    }
}

#endif