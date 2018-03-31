#ifndef FH_FIELDACTIONS_H_
#define FH_FIELDACTIONS_H_

#include "info.hpp"

namespace fasthal{
    // common "fallback" actions for Fields. TODO: enable_if<is_field>
    template<class TField, typename TDataType = field_data_type<TField>>
    constexpr void write(TField field, TDataType value) 
    {
        TField::write(value);
    }

    template<class TField, typename TMaskType = field_mask_type<TField>>
    constexpr void clearAndSet(TField field, TMaskType clearMask, TMaskType setMask) 
    {
        TField::write((TField::read() & ~clearMask) | setMask);
    }

    template<class TField, typename TMaskType = field_mask_type<TField>>
    constexpr void set(TField field, TMaskType mask) 
    {
        TField::write(TField::read() | mask);
    }

    template<class TField, typename TMaskType = field_mask_type<TField>>
    constexpr void clear(TField field, TMaskType mask) {
        TField::write(TField::read() & ~mask);
    }
    template<class TField, typename TMaskType = field_mask_type<TField>>
    constexpr void toggle(TField field, TMaskType mask) {
        TField::write(TField::read() ^ mask);
    }

    template<class TField>
    constexpr field_data_type<TField> read(TField field) {
        return TField::read();
    }
    
    template<class TField, typename TMaskType = typename field_mask_types<TField>::OneBitMask>
    constexpr bool read(TField field, TMaskType pin){
        return TField::read() & pin;
    }
}

#endif