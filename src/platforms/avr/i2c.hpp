#ifndef FH_AVR_I2C_H_
#define FH_AVR_I2C_H_

#include "registers.hpp"
#include "../../streams/sync_streams.hpp"

namespace fasthal{
    namespace details{
        template<unsigned VNum>
        struct i2c_impl{
            static constexpr bool available = false;
        };

        // enable, reset, ready for i2c impl
        template<unsigned VNum>
        struct func_fieldbit_impl<i2c_impl<VNum>>:
            func_fieldbit_enable<decltype(i2c_impl<VNum>::enable)>,
            func_fieldbit_ready_reset<decltype(i2c_impl<VNum>::ready)>
            { };

        inline constexpr std::uint8_t twi_ps_value(avr::tw_ps ps){
            using ps_t = avr::tw_ps;
            switch (ps){
                case ps_t::_1 : return 1;
                case ps_t::_4 : return 4;
                case ps_t::_16 : return 16;
                case ps_t::_64 : return 64;
                default: return 0;
            }
        }

        template<typename TFreq, typename TPs>
        inline constexpr std::uint8_t i2c_calc_twbr(TFreq freq, TPs ps){
            auto psv = twi_ps_value(ps);
            
            return ((F_CPU / freq) - 16) / (2 * psv);
        }

        template<typename TFreq, TFreq VFreq, avr::tw_ps VPs>
        inline constexpr auto i2c_calc_twbr(integral_constant<TFreq, VFreq> freq, integral_constant<avr::tw_ps, VPs> ps) {
            auto result = i2c_calc_twbr(VFreq, VPs);
            return integral_constant<std::uint8_t, result>{};
        }
        
        template<bool VRead, typename TAddress>
        inline constexpr auto i2c_build_sla(TAddress address){
            auto result = static_cast<std::uint8_t>(static_cast<std::uint8_t>(address) << 1);
            return VRead ? result | 1 : result;
        }
        template<bool VRead, typename TAddress, TAddress VAddress>
        inline constexpr auto i2c_build_sla(integral_constant<TAddress, VAddress> address){
            constexpr auto result = i2c_build_sla<VRead>(VAddress);
            return integral_constant<decltype(result), result>{};
        }

        template<unsigned VNum>
        struct i2c_func{
            static constexpr auto _i2c = details::i2c_impl<VNum>{};
            static_assert(_i2c.available, "I2C not available");

            template<typename... T>
            static inline void control(T... actions){
                apply(
                    clear(_i2c.control), 
                    enable(_i2c),
                    reset(_i2c),
                    actions...);
            }

            static inline void write(std::uint8_t v){
                write_(_i2c.data, v),
                control();
            }     

            static inline void wait(){
                wait_(_i2c);
            }
            static inline void wait_stop(){
                wait_lo(_i2c.stop);
            }
        };

        // transmitter target, don't depend on transmitter!
        template<unsigned VNum, template<typename> typename TTrans>
        struct i2c_tx_impl{
            static constexpr auto _i2c = i2c_impl<VNum>{};
            static constexpr auto _f = i2c_func<VNum>{};

            struct lazy{
                using trans_t = TTrans<i2c_tx_impl<VNum, TTrans>>;
                static constexpr auto async_tx = trans_t::async;
            };
       
            // write 1 byte, for transmitter communication
            static inline bool try_write(std::uint8_t c){
                // TODO: Check conditions
                _f.write(c);
                _f.wait();
                // TODO: Check conditions
                return true;
            }

             // tries to write something from transmitter
            static inline void try_write_sync(){
                static_assert(lazy::async_tx, "Not async i2c, shouldn't be here");
                //try_irq_force(uart_t::irq_txr);
            }

            // called by transmitter if it has some data
            static inline void write_async(){
                static_assert(lazy::async_tx, "Not async i2c, shouldn't be here");
                //enable_(uart_t::irq_txr);
            }

            static inline void flush() {
                // TODO
            }
        };

        template<unsigned VNum, template<typename> typename TRecv>
        struct i2c_rx_impl{
            static constexpr auto _i2c = i2c_impl<VNum>{};
            static constexpr auto _f = i2c_func<VNum>{};

            struct lazy{
                using recv_t = TRecv<i2c_rx_impl<VNum, TRecv>>;
                //static constexpr auto async_rx = recv_t::async;
            };

            // read for sync receiver
            static std::uint8_t read(){
                // TODO: Check start condition
                // Ask for next byte
                _i2c.bytesToRead--;                
                _f.control(enable(_i2c.ack, _i2c.bytesToRead != 0));
                _f.wait();

                // TODO: Check status
                return read_(_i2c.data);
            }
        };
    }

    using i2c_address_t = std::uint8_t;
    template<i2c_address_t V>
    constexpr auto i2c_address_v = integral_constant<i2c_address_t, V>{};

    template<std::uint32_t V>
    constexpr auto i2c_freq_v = integral_constant<std::uint32_t, V>{};
    constexpr auto i2c_freq_def = i2c_freq_v<100000L>;

    template<unsigned VNum, template<typename> typename TTrans = sync_transmitter, template<typename> typename TRecv = sync_receiver>
    struct i2c{
        static constexpr auto _i2c = details::i2c_impl<VNum>{};
        static_assert(_i2c.available, "I2C not available");
        static constexpr auto _f = details::i2c_func<VNum>{};

        using tx_impl_t = details::i2c_tx_impl<VNum, TTrans>;
        using trans_t = TTrans<tx_impl_t>;
        static constexpr auto tx = trans_t{};

        using rx_impl_t = details::i2c_rx_impl<VNum, TRecv>;
        using recv_t = TRecv<rx_impl_t>;
        static constexpr auto rx = recv_t{};

        // begin
        template<typename TFreq = decltype(i2c_freq_def), typename TPs = decltype(avr::tw_ps::def)>
        static inline constexpr auto begin(TFreq freq = i2c_freq_def, TPs ps = avr::tw_ps::def){
            return combine(
                write(_i2c.rate, details::i2c_calc_twbr(freq, ps)),
                clear(_i2c.control),
                write(_i2c.ps, ps),
                enable(_i2c)
                //enable(i2c.irq),
                //enable(i2c.ack)
            );
        }
        template<typename TFreq = decltype(i2c_freq_def), typename TPs = decltype(avr::tw_ps::def)>
        static inline void begin_(TFreq freq = i2c_freq_def, TPs ps = avr::tw_ps::def){
            apply(begin(freq, ps));
        }

        // end i2c
        static inline constexpr auto end(){            
            return clear(_i2c.control);
        }
        static inline void end_() { 
            apply(end()); 
        }

        // begin master transfer
        template<typename TAddress>
        static bool mt_start(TAddress address){
            // TODO: Check begin condition
            auto sla = details::i2c_build_sla<false>(address);
            _f.control(set(_i2c.start));
            _f.wait();
            // check started
            _f.write(sla);
            _f.wait();
            // check select

            // TODO: Check end condition
            return true;
        }

        // begin master receive
        template<typename TAddress>
        static bool mr_start(TAddress address, bsize_t size){
            // TODO: Check begin condition
            auto sla = details::i2c_build_sla<true>(address);
            _f.control(set(_i2c.start));
            _f.wait();
            // check started
            _i2c.bytesToRead = size;
            _f.write(sla);
            _f.wait();
            // check select

            // TODO: Check end condition
            return true;
        }

        // stop master operation
        static bool stop(){
            // TODO: Check begin condition, return something more meaningful
            _f.control(set(_i2c.stop));
            _f.wait_stop();
            // TODO: Check end condition
            return true;
        }
    };

    #include "i2c_impl.hpp"
};

#endif
