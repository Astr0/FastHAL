#if defined(TWI_vect_num)
    #define FH_IRQ_I2C0 TWI_vect_num

    constexpr auto irq_i2c0 = interrupt<FH_IRQ_I2C0>{};
    namespace details{
        template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_i2c0)>>: 
            func_fieldbit_enable<decltype(avr::twie0)>,
            func_fieldbit_ready_reset<decltype(avr::twint0)>
            {};
    }
    
#endif