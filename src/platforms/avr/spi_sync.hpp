#ifndef FH_AVR_SPI_SYNC_H_
#define FH_AVR_SPI_SYNC_H_

namespace fasthal{
    template<class TSpi>
    class spi_sync{
        static constexpr auto _spi= TSpi{};

        void transfer(buffer_t buf, bsize_t count){
            while (count-- != 0){
                // tx
                _spi.tx(*buf);
                // wait
                while (!ready(_spi.irq));
                // read
                *buf++ = _spi.rx();
            }
        }
    public:
        static constexpr bool async() { return false; }

        template<class TArgs>
        void transfer(TArgs& args){
            transfer(args.buffer(), args.count());
        }
    };
}

#endif