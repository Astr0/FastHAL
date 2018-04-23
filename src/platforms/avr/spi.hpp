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
    template<spi_mode V>
    static constexpr auto spi_mode_v = integral_constant<spi_mode, V>{};

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
    template<spi_ps V>
    static constexpr auto spi_ps_v = integral_constant<spi_ps, V>{};

    static constexpr auto spi_msb_first = integral_constant<bool, true>{};
    static constexpr auto spi_lsb_first = integral_constant<bool, false>{};

    using spi_clock_t = std::uint32_t;
    template <spi_clock_t V>
    static constexpr auto spi_clock_v = integral_constant<spi_clock_t, V>{};

    static constexpr auto spi_clock_def = 4'000'000;

    namespace details{
        template<unsigned VNum>
        struct spi_impl{
            static constexpr auto available = false;            
        };

        constexpr spi_ps clock_to_ps(spi_clock_t clock){
            std::uint32_t clockSetting = F_CPU / 2;
            std::uint8_t ps = 0;
            while (ps < 6 && clock < clockSetting) {
                clockSetting >>= 1;
                ps++;
            }
            // Compensate for the duplicate _64p
            if (ps == 6)
                ps = 7;

            // Invert the SPI2X bit
            ps ^= 0b100;

            return static_cast<spi_ps>(ps);
        }

        template<spi_clock_t VClock>
        constexpr auto clock_to_ps(integral_constant<spi_clock_t, VClock>){
            auto ps = clock_to_ps(VClock);
            return spi_ps_v<ps>;
        }
    }

    template<unsigned VNum>
    struct spi: details::spi_impl<VNum>{
        using impl_t = details::spi_impl<VNum>;
        static_assert(impl_t::available, "SPI not available");

        template<
            typename TClock = decltype(spi_clock_def), 
            typename TMsbFirst = decltype(spi_msb_first), 
            typename TMode = decltype(spi_mode_v<spi_mode::def>)>
        static auto begin(
            TClock clock = spi_clock_def, 
            TMsbFirst msb_first = spi_msb_first, 
            TMode mode = spi_mode_v<spi_mode::def>){
            return combine(
                set(impl_t::spen),
                set(impl_t::spips, details::clock_to_ps(clock)),
                set(impl_t::dord, msb_first),
                set(impl_t::spimode, mode)
            );
        }

        template<
            typename TClock = decltype(spi_clock_def), 
            typename TMsbFirst = decltype(spi_msb_first), 
            typename TMode = decltype(spi_mode_v<spi_mode::def>)>
        static void begin_(
            TClock clock = spi_clock_def, 
            TMsbFirst msb_first = spi_msb_first, 
            TMode mode = spi_mode_v<spi_mode::def>){
            apply(begin(clock, msb_first, mode));
        }

        static void tx(std::uint8_t v) { write_(impl_t::spdr, v); }
        static std::uint8_t rx() { return read_(impl_t::spdr); }

        static auto end() { return clear(impl_t::spen); }
        static void end_() { clear_(impl_t::spen); }
    };

    #include "spi_impl.hpp"
}

#include "spi_async.hpp"

#endif
