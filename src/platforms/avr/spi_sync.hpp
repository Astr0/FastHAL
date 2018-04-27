#ifndef FH_AVR_SPI_SYNC_H_
#define FH_AVR_SPI_SYNC_H_

#include "interrupts.hpp"

namespace fasthal{
    template<class TSpi>
    class spi_sync{
        static constexpr auto _spi= TSpi{};

        
    public:
        static constexpr bool async() { return false; }

        static std::uint8_t transfer(std::uint8_t v){
            _spi.tx(v);
            while (!ready_(_spi.irq));
            return _spi.rx();
        }

        static void transfer(std::uint8_t* buf, bsize_t count) {
            while (count--){
                *buf = transfer(*buf);
                ++buf;
            }
        }

        template<class TArgs>
        static void transfer(TArgs& args){
            // detemplify
            transfer(args.buffer(), args.count());
        }

        static void write(const std::uint8_t* buf, bsize_t count) {
            while (count--)
                transfer(*buf);
        }

        template<class TArgs>
        static void write(const TArgs& args){
            // detemplify
            write(args.buffer(), args.count());
        }
    };
}

#endif