#ifndef FH_AVR_SPI_SYNC_H_
#define FH_AVR_SPI_SYNC_H_

#include "interrupts.hpp"

namespace fasthal{
    template<class TSpi>
    class spi_sync{
        static constexpr auto _spi= TSpi{};

        static void transfer(buffer_t buf, bsize_t count) {
            while (count--){
                // tx
                _spi.tx(*buf);
                // wait
                while (!ready_(_spi.irq));
                // read
                *buf++ = _spi.rx();
            }
        }
    public:
        static constexpr bool async() { return false; }

        template<class TArgs>
        static void transfer(TArgs& args){
            // detemplify
            transfer(args.buffer(), args.count());
        }
    };
}

#endif