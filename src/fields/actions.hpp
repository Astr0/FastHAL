#ifndef FH_FIELDACTIONS_H_
#define FH_FIELDACTIONS_H_

#include "info.hpp"

namespace fasthal{
    template<class TField, typename DataType = field_data_type<TField>>
    constexpr void write(TField field, DataType value) 
    {
        TField::write(value);
    }

    template<class TField, typename MaskType = field_mask_type<TField>>
    constexpr void clearAndSet(TField field, MaskType clearMask, MaskType setMask) 
    {
        TField::write((TField::read() & ~clearMask) | setMask);
    }

    template<class TField, typename MaskType = field_mask_type<TField>>
    constexpr void set(TField field, MaskType mask) 
    {
        TField::write(TField::read() | mask);
    }

    template<class TField, typename MaskType = field_mask_type<TField>>
    constexpr void clear(TField field, MaskType mask) {
        TField::write(TField::read() & ~mask);
    }
    template<class TField, typename MaskType = field_mask_type<TField>>
    constexpr void toggle(TField field, MaskType mask) {
        TField::write(TField::read() ^ mask);
    }

    template<class TField>
    constexpr field_data_type<TField> read(TField field) {
        return TField::read();
    }
    
    template<class TField, typename MaskType = typename field_mask_types<TField>::OneBitMask>
    constexpr bool read(TField field, MaskType pin){
        return TField::read() & pin;
    }
}

#endif