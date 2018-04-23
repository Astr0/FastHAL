#ifndef FH_AVR_SPI_H_
#define FH_AVR_SPI_H_

#include "../../std/std_types.hpp"

namespace fasthal{
    // leading edge names
    enum class spi_mode: std::uint8_t{
        rising_sample = 0b00,
        rising_setup = 0b01,
        falling_sample = 0b10,
        falling_setup = 0b11,
        def = rising_sample
    };

    // prescaler
    enum class spi_ps:  std::uint8_t{
        _2  = 0b100,
        _4  = 0b000,
        _8  = 0b101,
        _16 = 0b001,
        _32 = 0b110,
        _64 = 0b010,
        _64p = 0b111,
        _128 = 0b011
    };

    namespace details{
        template<unsigned VNum>
        struct spi_impl{
            static constexpr auto available = false;
            
        };
    }

    #include "spi_impl.hpp"
}

#endif
