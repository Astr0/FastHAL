#if defined(TWI_vect_num)
    #define FH_IRQ_I2C TWI_vect_num

    constexpr auto irq_i2c = interrupt<FH_IRQ_I2C>{};
    namespace details{
        template<> struct func_fieldbit_impl<std::base_type_t<decltype(irq_i2c)>>: 
            func_fieldbit_enable<decltype(avr::twie)>,
            func_fieldbit_ready_reset<decltype(avr::twint)>
            {};
    }
    
#endif