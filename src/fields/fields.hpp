#ifndef FH_FIELDS_H_
#define FH_FIELDS_H_

#include "../mp/brigand.hpp"
#include "../mp/type_traits.hpp"

namespace fasthal
{

// Null port to get do-nothing bits for skipping something
struct NullField
{
    // static constexpr size_t width() { return 8 * sizeof(DataType); }

    static uint8_t read() { return 0; }
    static void write(uint8_t v) {}
};

// Represents one bit in field, possibly inverted
template <typename TField, unsigned VNumber, bool VInverted>
struct FieldBit
{
    using Field = TField;
    static constexpr unsigned Number = VNumber;
    static constexpr bool Inverted = VInverted;
};

// Bit field - field with custom collection of bits
template <typename... TFieldBits>
struct BitField
{
    using FieldBits = brigand::list<TFieldBits...>;
};


namespace details
{
// // is_field to check if T is field
// template <class T>
// struct is_field : mp::false_type
// {
// };

// // null field is
// template <>
// struct is_field<NullField> : mp::true_type
// {
// };

// is_field_bit to check if T is FieldBit
template <class T>
struct is_field_bit : std::false_type
{
};

// Only field bit is
template <typename TField, unsigned VNumber, bool VInverted>
struct is_field_bit<FieldBit<TField, VNumber, VInverted>> : std::true_type
{
};

template <class T>
struct is_bit_field: std::false_type{};

template <typename... T>
struct is_bit_field<BitField<T...>>:std::true_type{};

template <typename TField>
struct get_field_datatype{
    using type = decltype(TField::read());
};

};


// get field bit for number and field
template <unsigned VNumber, typename TField>
constexpr decltype(auto) fieldBit(TField field)
{
    //static_assert(details::is_field<TField>::value, "not a field");
    return FieldBit<TField, VNumber, false>{};
}

// invert field bit
template <typename TField, unsigned VNumber>
constexpr decltype(auto) inverted(FieldBit<TField, VNumber, false> fb)
{
    return FieldBit<TField, VNumber, true>();
}

// invert field bit
template <typename TField, unsigned VNumber>
constexpr decltype(auto) inverted(FieldBit<TField, VNumber, true> fb)
{
    return FieldBit<TField, VNumber, false>();
}

// make bit field from field bits
template <typename... TFieldBits>
constexpr decltype(auto) bitField(TFieldBits... bits)
{
    // TODO
    static_assert(brigand::all<brigand::list<TFieldBits...>, details::is_field_bit<brigand::_1> >::value, "not field bits");
    return BitField<TFieldBits...>{};
}

// get field bit from bit field specialization
template <unsigned VNumber, typename... TFieldBits>
constexpr decltype(auto) fieldBit(BitField<TFieldBits...> field)
{
    return brigand::at_c<brigand::list<TFieldBits...>, VNumber>{};
}

// nullfield instance
static constexpr auto nullField = NullField{};
// nullfield bit, used for alignment
static constexpr auto nullBit = fieldBit<0>(nullField);

};

#endif