#ifdef FH_HAS_I2C0
namespace details{
    template<>
    struct i2c_impl<0>{
        static constexpr bool available = true;
        using status_t = avr::tw_s;

        static constexpr auto ps = ::fasthal::avr::twps0;
        static constexpr auto rate = ::fasthal::avr::twbr0;
        static constexpr auto status = ::fasthal::avr::tws0;
        static constexpr auto data = ::fasthal::avr::twdr0;
        static constexpr auto control = ::fasthal::avr::twcr0;
        
        static constexpr auto enable = ::fasthal::avr::twen0;
        static constexpr auto ready = ::fasthal::avr::twint0;
        static constexpr auto ack = ::fasthal::avr::twea0;
        static constexpr auto start = ::fasthal::avr::twsta0;
        static constexpr auto stop = ::fasthal::avr::twsto0;
        
        static constexpr auto irq = irq_i2c0;

        static bsize_t bytesToRead;
        //static i2c_state state;
    };

    bsize_t i2c_impl<0>::bytesToRead{};

    // enable for i2c.ack
    template<>
    struct func_fieldbit_impl<std::base_type_t<decltype(::fasthal::avr::twea0)>>:
        func_fieldbit_enable<decltype(::fasthal::avr::twea0)>
        { };
}
//static i2c_state details::i2c_impl<0>::state = i2c_state::off;
//constexpr auto i2c0 = details::i2c_impl<0>{};

#endif