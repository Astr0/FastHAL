#ifndef FH_AVR_TIMERS_H_
#define FH_AVR_TIMERS_H_

// ************* Timer0 **************
// Control
// ATTiny24: TCCR0A: COM0A1 COM0A0 COM0B1 COM0B0 �      �       WGM01  WGM00    TCCR0B: FOC0A FOC0B �      �      WGM02 CS02  CS01  CS00
// ATTiny25: TCCR0A: COM0A1 COM0A0 COM0B1 COM0B0 �      �       WGM01  WGM00    TCCR0B: FOC0A FOC0B �      �      WGM02 CS02  CS01  CS00
// Mega328:  TCCR0A: COM0A1 COM0A0 COM0B1 COM0B0 -      -       WGM01  WGM00    TCCR0B: FOC0A FOC0B -      -      WGM02 CS02  CS01  CS00
// Mega32u4: TCCR0A: COM0A1 COM0A0 COM0B1 COM0B0 �      �       WGM01  WGM00    TCCR0B: FOC0A FOC0B �      �      WGM02 CS02  CS01  CS00
// Mega2560: TCCR0A: COM0A1 COM0A0 COM0B1 COM0B0 �      �       WGM01  WGM00    TCCR0B: FOC0A FOC0B �      �      WGM02 CS02  CS01  CS00
// Mega8:                                                                       TCCR0:  �     �     �      �      �     CS02  CS01  CS00
// Mega32:                                                                      TCCR0:  FOC0  WGM00 COM01  COM00  WGM01 CS02  CS01  CS00
// Mega128:                                                                     TCCR0:  FOC0  WGM00 COM01  COM00  WGM01 CS02  CS01  CS00

// Values:
// TCNT0, [OCR0A + OCR0B / OCR0]

// Interrupts:
// ATTiny24: TIMSK0: �      �      �      �      �      OCIE0B  OCIE0A TOIE0    TIFR0:  �     �     �      �      �     OCF0B OCF0A TOV0
// ATTiny25: TIMSK:  �      OCIE1A OCIE1B OCIE0A OCIE0B TOIE1   TOIE0  �        TIFR:   �     OCF1A OCF1B  OCF0A  OCF0B TOV1  TOV0  �
// Mega328	 TIMSK0: �      �      �      �      �      OCIE0B  OCIE0A TOIE0    TIFR0:  �     �     �      �      �     OCFB  OCFA  TOV0  
// Mega32u4  TIMSK0: �      �      �      �      �      OCIE0B  OCIE0A TOIE0    TIFR0:  �     �     �      �      �     OCF0B OCF0A TOV0
// Mega2560  TIMSK0: �      �      �      �      �      OCIE0B  OCIE0A TOIE0    TIFR0:  �     �     �      �      �     OCF0B OCF0A TOV0
// Mega8     TIMSK:  OCIE2  TOIE2  TICIE1 OCIE1A OCIE1B TOIE1   �      TOIE0    TIFR:   OCF2  TOV2  ICF1   OCF1A  OCF1B TOV1  �     TOV0
// Mega32    TIMSK:  OCIE2  TOIE2  TICIE1 OCIE1A OCIE1B TOIE1   OCIE0  TOIE0    TIFR:   OCF2  TOV2  ICF1   OCF1A  OCF1B TOV1  OCF0  TOV0
// Mega128   TIMSK:  OCIE2  TOIE2  TICIE1 OCIE1A OCIE1B TOIE1   OCIE0  TOIE0    TIFR:   OCF2  TOV2  ICF1   OCF1A  OCF1B TOV1  OCF0  TOV0   


// ************* Timer1 **************
// !ATiny25 && ATTiny261 have different timer1!!!
// Control
// ATTiny24  TCCR1A: COM1A1 COM1A0 COM1B1 COM1B0 �      �       WGM11  WGM10    TCCR1B: ICNC1 ICES1 �      WGM13  WGM12 CS12  CS11  CS10   TCCR1C: FOC1A FOC1B � � � � � �
// !ATTiny25 TCCR1:  CTC1   PWM1A  COM1A1 COM1A0 CS13   CS12    CS11   CS10     GTCCR:  TSM   PWM1B COM1B1 COM1B0 FOC1B FOC1A PSR1  PSR0
// Mega328	 TCCR1A: COM1A1 COM1A0 COM1B1 COM1B0 -      -       WGM11  WGM10    TCCR1B: ICNC1 ICES1 -      WGM13  WGM12 CS12  CS11  CS10   TCCR1C: FOC1A FOC1B - - - - - -
// Mega32u4  TCCR1A: COM1A1 COM1A0 COM1B1 COM1B0 COM1C1 COM1C0  WGM11  WGM10    TCCR1B: ICNC1 ICES1 �      WGM13  WGM12 CS12  CS11  CS10   TCCR1C: FOC1A FOC1B FOC1C � � � � �
// Mega2560  TCCR1A: COM1A1 COM1A0 COM1B1 COM1B0 COM1C1 COM1C0  WGM11  WGM10    TCCR1B: ICNC1 ICES1 �      WGM13  WGM12 CS12  CS11  CS10   TCCR1C: FOC1A FOC1B FOC1C � � � � �
// Mega8     TCCR1A: COM1A1 COM1A0 COM1B1 COM1B0 FOC1A  FOC1B   WGM11  WGM10    TCCR1B: ICNC1 ICES1 �      WGM13  WGM12 CS12  CS11  CS10
// Mega32	 TCCR1A: COM1A1 COM1A0 COM1B1 COM1B0 FOC1A  FOC1B   WGM11  WGM10    TCCR1B: ICNC1 ICES1 �      WGM13  WGM12 CS12  CS11  CS10
// Mega128   TCCR1A: COM1A1 COM1A0 COM1B1 COM1B0 COM1C1 COM1C0  WGM11  WGM10    TCCR1B: ICNC1 ICES1 �      WGM13  WGM12 CS12  CS11  CS10   TCCR1C: FOC1A FOC1B FOC1C � � � � �

// Values
// TCNT1H/L, OCR1AH/L, OCR1BH/L, [OCR1CH/L,] ICR1H/L

// Interrupts
// ATTiny24	 TIMSK1: �	    �      ICIE1  �      �      OCIE1B  OCIE1A TOIE1    TIFR1:  �     �     ICF1   �      �     OCF1B OCF1A TOV1
// ATTiny25  TIMSK:  �      OCIE1A OCIE1B OCIE0A OCIE0B TOIE1   TOIE0  �        TIFR:   �     OCF1A OCF1B  OCF0A  OCF0B TOV1  TOV0  �
// Mega328   TIMSK1: -      -      ICIE1  -      -      OCIE1B  OCIE1A TOIE1    TIFR1:  -     -     ICF1   -      -     OCF1B OCF1A TOV1
// Mega32u4  TIMSK1: �      �      ICIE1  �      OCIE1C OCIE1B  OCIE1A TOIE1    TIFR1:  �     �     ICF1   �      OCF1C OCF1B OCF1A TOV1
// Mega2560  TIMSK1: �      �      ICIE1  �      OCIE1C OCIE1B  OCIE1A TOIE1    TIFR1:  �     �     ICF1   �      OCF1C OCF1B OCF1A TOV1
// Mega8     TIMSK:  OCIE2  TOIE2  TICIE1 OCIE1A OCIE1B TOIE1   �      TOIE0    TIFR:   OCF2  TOV2  ICF1   OCF1A  OCF1B TOV1  �     TOV0
// Mega32    TIMSK:  OCIE2  TOIE2  TICIE1 OCIE1A OCIE1B TOIE1   OCIE0  TOIE0    TIFR:   OCF2  TOV2  ICF1   OCF1A  OCF1B TOV1  OCF0  TOV0
// Mega128   TIMSK:  OCIE2  TOIE2  TICIE1 OCIE1A OCIE1B TOIE1   OCIE0  TOIE0   TIFR:   OCF2  TOV2  ICF1   OCF1A  OCF1B TOV1  OCF0  TOV0   


// ************* Timer2 ***************
// ATTiny24: N/A
// ATTiny25: N/A
// Mega32u4: N/A
// Control
// Mega328  TCCR1A: COM2A1 COM2A0 COM2B1 COM2B0 �      �       WGM21  WGM20     TCCR2B: FOC2A FOC2B �      �      WGM22 CS22  CS21  CS20
// Mega2560 TCCR1A: COM2A1 COM2A0 COM2B1 COM2B0 �      �       WGM21  WGM20     TCCR2B: FOC2A FOC2B �      �      WGM22 CS22  CS21  CS20
// Mega8                                                                        TCCR2:  FOC2  WGM20 COM21  COM20  WGM21 CS22  CS21  CS20
// Mega32                                                                       TCCR2:  FOC2  WGM20 COM21  COM20  WGM21 CS22  CS21  CS20
// Mega128                                                                      TCCR2:  FOC2  WGM20 COM21  COM20  WGM21 CS22  CS21  CS20

// Values
// TCNT2, [OCR2A+OCR2B/OCR2]

// Async
// Mega328   ASSR: - EXCLK AS2 TCN2UB OCR2AUB OCR2BUB TCR2AUB TCR2BUB   GTCCR: TSM - - - -    -   PSRASY PSRSYNC
// Mega2560  ASSR: � EXCLK AS2 TCN2UB OCR2AUB OCR2BUB TCR2AUB TCR2BUB   GTCCR: TSM - - - -    -   PSRASY PSRSYNC
// Mega8     ASSR: - -     -   -      AS2     TCN2UB  OCR2UB  TCR2UB    SFIOR: -   - - - ACME PUD PSR2   PSR10
// Mega32    ASSR: - -     -   -      AS2     TCN2UB  OCR2UB  TCR2UB    SFIOR: -   - - - ACME PUD PSR2   PSR10
// Mega128   N/A

// Interrupts
// Mega328  TIMSK2: �      �      �      �      �      OCIEB   OCIEA  TOIE      TIFR2:  �     �     �      �      �     OCFB  OCFA  TOV  
// Mega2560 TIMSK2: �      �      �      �      �      OCIE2B  OCIE2A TOIE2     TIFR2:  �     �     �      �      �     OCF2B OCF2A TOV2
// Mega8    TIMSK:  OCIE2  TOIE2  TICIE1 OCIE1A OCIE1B TOIE1   �      TOIE0     TIFR:   OCF2  TOV2  ICF1   OCF1A  OCF1B TOV1  �     TOV0
// Mega32   TIMSK:  OCIE2  TOIE2  TICIE1 OCIE1A OCIE1B TOIE1   OCIE0  TOIE0     TIFR:   OCF2  TOV2  ICF1   OCF1A  OCF1B TOV1  OCF0  TOV0
// Mega128  TIMSK:  OCIE2  TOIE2  TICIE1 OCIE1A OCIE1B TOIE1   OCIE0  TOIE0     TIFR:   OCF2  TOV2  ICF1   OCF1A  OCF1B TOV1  OCF0  TOV0

// ************ Timer 3 *********
// Same as timer1 on most AVR

// ************ Timer 4 *********
// Same as timer1 on most AVR
// Mega32u4 TCCR4A: COM4A1  COM4A0  COM4B1  COM4B0  FOC4A  FOC4B  PWM4A  PWM4B         TCCR4B: PWM4X PSR4  DTPS41 DTPS40 CS43  CS42 CS41  CS40    
//          TCCR4C: COM4A1S COM4A0S COM4B1S COMAB0S COM4D1 COM4D0 FOC4D  PWM4D         TCCR4D: FPIE4 FPEN4 FPNC4  FPES4  FPAC4 FPF4 WGM41 WGM40
//          TCCR4E: TLOCK4  ENHC4   OC4OE5  OC4OE4  OC4OE3 OC4OE2 OC4OE1 OC4OE0
// TCNT4, TC4H, OCR4A, OCR4B, OCR4C, OCR4D, DT4
//          TIMSK4: OCIE4D  OCIE4A  OCIE4B  -       -      TOIE4  -      -             TIFR4:  OCF4D OCF4A OCF4B  -      -     TOV4 -     -

// ************ Timer 5 *********
// Same as timer1 on most AVR

#include "registers.hpp"
#include "interrupts.hpp"
#include "../../fields/fieldbit.hpp"
#include "../../fields/func_fieldbit.hpp"
#include "../../std/std_types.hpp"

namespace fasthal{
    // compare
    enum class timer_oc: std::uint8_t{
        none   = 0b00,
        toggle = 0b01,
        clear  = 0b10,
        set    = 0b11
    };

    template<timer_oc V>
    static constexpr auto timer_oc_v = integral_constant<timer_oc, V>{};

    namespace details{
        // clock source
        enum class timer_csext: std::uint8_t{
            _       = 0b000,
            _1      = 0b001,
            _8      = 0b010,
            _64     = 0b011,
            _256    = 0b100,
            _1024   = 0b101,
            _fall   = 0b110,
            _rise   = 0b111,
            def     = _64
        };
        
        enum class timer_cs: std::uint8_t{
            _       = 0b000,
            _1      = 0b001,
            _8      = 0b010,
            _32     = 0b011,
            _64     = 0b100,
            _128    = 0b101,
            _256    = 0b110,
            _1024   = 0b111,
            def    = 64
        };

        // wave generation mode
        enum class timer_wgm3: std::uint8_t{
            normal      = 0b0000,
            pwm_pc8     = 0b0001,
            pwm_pc9     = 0b0010,
            pwm_pc10    = 0b0011,
            ctc_a       = 0b0100,
            pwm_fast8   = 0b0101,
            pwm_fast9   = 0b0110,
            pwm_fast10  = 0b0111,
            pwm_pcfci   = 0b1000,
            pwm_pcfca   = 0b1001,
            pwm_pci     = 0b1010,
            pwm_pca     = 0b1011,
            ctc_i       = 0b1100,
            //reserved  = 0b0101,
            pwm_fasti   = 0b1110,
            pwm_fasta   = 0b1111,
            pwm_pcdef   = pwm_pc8,
            pwm_fastdef = pwm_fast8,
            pwm_def     = pwm_pcdef
        };

        enum class timer_wgm2: std::uint8_t{
            normal      = 0b000,
            pwm_pcmax   = 0b001,
            ctc_a       = 0b010,
            pwm_fastmax = 0b011,
            /*reserved  = 0b100,*/
            pwm_pca     = 0b101,
            /*reserved  = 0b110,*/
            pwm_fasta   = 0b111,
            pwm_pcdef   = pwm_pcmax,
            pwm_fastdef = pwm_fastmax,
            pwm_def     = pwm_pcdef
        };

        enum class timer_wgm1: uint8_t{
            normal      = 0b00,
            pwm_pcmax   = 0b01,
            ctc_a       = 0b10,
            pwm_fastmax = 0b11,
            pwm_pcdef   = pwm_pcmax,
            pwm_fastdef = pwm_fastmax,            
            pwm_def     = pwm_pcdef
        };

        template<unsigned VNum>
        struct timer_impl{
            static constexpr auto available = false;
        };

        
        template<unsigned VNum, unsigned VComp>
        struct timer_oc_impl{
            static constexpr auto available = false;
        };
    }

    static constexpr std::uint16_t timer_cs_value(details::timer_cs cs){
        using cs_t = details::timer_cs;
        switch (cs){
            //case cs_t::_        : return 0;
            case cs_t::_1      : return 1U;
            case cs_t::_8      : return 8U;
            case cs_t::_32     : return 32U;
            case cs_t::_64     : return 64U;
            case cs_t::_128    : return 128U;
            case cs_t::_256    : return 256U;
            case cs_t::_1024   : return 1024U;
            default            : return 0U;
        };
    }
    static constexpr std::uint16_t timer_cs_value(details::timer_csext cs){
        using cs_t = details::timer_csext;
        switch (cs){
            //case cs_t::_        : return 0;
            case cs_t::_1      : return 1U;
            case cs_t::_8      : return 8U;
            case cs_t::_64     : return 64U;
            case cs_t::_256    : return 256U;
            case cs_t::_1024   : return 1024U;
            default            : return 0U;
        };
    }

    // max value to generate TOV
    static constexpr std::uint16_t timer_wgm_max(details::timer_wgm3 wgm){
        using wgm_t = details::timer_wgm3;
        switch (wgm){
            case wgm_t::normal      : return 0xFFFF;
            case wgm_t::pwm_pc8     : return 0x00FF;
            case wgm_t::pwm_pc9     : return 0x01FF;
            case wgm_t::pwm_pc10    : return 0x03FF;            
            case wgm_t::pwm_fast8   : return 0x00FF;
            case wgm_t::pwm_fast9   : return 0x01FF;
            case wgm_t::pwm_fast10  : return 0x03FF;
            default                 : return 0;
        };
    }

    // where TOV interrupts (true - top, false - bottom)
    static constexpr bool timer_wgm_tov(details::timer_wgm3 wgm){
        using wgm_t = details::timer_wgm3;
        switch (wgm){
            case wgm_t::normal      : return true;
            case wgm_t::pwm_pc8     : return false;
            case wgm_t::pwm_pc9     : return false;
            case wgm_t::pwm_pc10    : return false;
            case wgm_t::ctc_a       : return true;
            case wgm_t::pwm_fast8   : return true;
            case wgm_t::pwm_fast9   : return true;
            case wgm_t::pwm_fast10  : return true;
            case wgm_t::pwm_pcfci   : return false;
            case wgm_t::pwm_pcfca   : return false;
            case wgm_t::pwm_pci     : return false;
            case wgm_t::pwm_pca     : return false;
            case wgm_t::ctc_i       : return true;            
            case wgm_t::pwm_fasti   : return true;
            case wgm_t::pwm_fasta   : return true;
            default                 : return true;            
        };
    }

    // max value to generate TOV
    static constexpr std::uint8_t timer_wgm_max(details::timer_wgm2 wgm){
        using wgm_t = details::timer_wgm2;
        switch (wgm){
            case wgm_t::normal      : return 0xFF;
            case wgm_t::pwm_pcmax   : return 0xFF;
            case wgm_t::pwm_fastmax : return 0xFF;
            default                 : return 0;
        };
    }

    // where TOV interrupts (true - top, false - bottom)
    static constexpr bool timer_wgm_tov(details::timer_wgm2 wgm){
        using wgm_t = details::timer_wgm2;
        switch (wgm){
            case wgm_t::normal      : return true;
            case wgm_t::pwm_pcmax   : return false;
            case wgm_t::ctc_a       : return true;
            case wgm_t::pwm_fastmax : return true;
            case wgm_t::pwm_pca     : return false;
            case wgm_t::pwm_fasta   : return true;
            default                 : return true;            
        };
    }

        // max value to generate TOV
    static constexpr std::uint8_t timer_wgm_max(details::timer_wgm1 wgm){
        using wgm_t = details::timer_wgm1;
        switch (wgm){
            case wgm_t::normal      : return 0xFF;
            case wgm_t::pwm_pcmax   : return 0xFF;
            case wgm_t::pwm_fastmax : return 0xFF;
            default                 : return 0;
        };
    }

    // where TOV interrupts (true - top, false - bottom)
    static constexpr bool timer_wgm_tov(details::timer_wgm1 wgm){
        using wgm_t = details::timer_wgm1;
        switch (wgm){
            case wgm_t::normal      : return true;
            case wgm_t::pwm_pcmax   : return false;
            case wgm_t::ctc_a       : return true;
            case wgm_t::pwm_fastmax : return true;
            default                 : return true;            
        };
    }


    #include "timers/timer0.hpp"
    #include "timers/timer1.hpp"
    #include "timers/timer2.hpp"
    #include "timers/timer3.hpp"
    #include "timers/timer4.hpp"
    #include "timers/timer5.hpp"

    template<unsigned VNum>
    struct timer: details::timer_impl<VNum>{
        using timer_t = details::timer_impl<VNum>;
        static_assert(timer_t::available, "Timer not available");
        
        using cs_t = typename timer_t::cs_t;
        using wgm_t = typename timer_t::wgm_t;

        template<
            typename Tcs = integral_constant<cs_t, cs_t::def>, 
            typename Twgm = integral_constant<wgm_t, wgm_t::pwm_def>>
        static inline constexpr auto begin(
            Tcs cs = integral_constant<cs_t, cs_t::def>{}, 
            Twgm wgm = integral_constant<wgm_t, wgm_t::pwm_def>{}){
            return combine(
                write(timer_t::cs, cs),
                write(timer_t::wgm, wgm)
            );
        }

        template<
            typename Tcs = integral_constant<cs_t, cs_t::def>, 
            typename Twgm = integral_constant<wgm_t, wgm_t::pwm_def>>
        static inline void begin_(
            Tcs cs = integral_constant<cs_t, cs_t::def>{}, 
            Twgm wgm = integral_constant<wgm_t, wgm_t::pwm_def>{}){
                apply(begin(cs, wgm));
        }
    };

    // from OC to timer
    template<unsigned VNum, unsigned VComp>
    inline constexpr auto oc_timer(details::timer_oc_impl<VNum, VComp> oc){
        return details::timer_impl<VNum>{};
    }

    // pwm, apply mode
    template<unsigned VNum, unsigned VComp, typename TVal>
    inline constexpr auto pwm(details::timer_oc_impl<VNum, VComp> oc, TVal val){        
        return combine(
            write(oc.comr, timer_oc_v<timer_oc::clear>),
            write(oc.ocr, val)
        );
    }
    template<unsigned VNum, unsigned VComp>
    inline constexpr auto nopwm(details::timer_oc_impl<VNum, VComp> oc)
    {
        return write(oc.comr, timer_oc_v<timer_oc::none>);
    }

    // pwm immediate
    template<unsigned VNum, unsigned VComp, typename TVal>
    inline void pwm_(details::timer_oc_impl<VNum, VComp> oc, TVal val){
        write_(oc.comr, timer_oc_v<timer_oc::clear>),
        write_(oc.ocr, val);
    }
    template<unsigned VNum, unsigned VComp>
    inline void nopwm_(details::timer_oc_impl<VNum, VComp> oc)
    {
        write_(oc.comr, timer_oc_v<timer_oc::none>);
    }

    namespace details{
        template<class TOC>
        struct func_fieldbit_pwn{
            using toc_t = TOC;
        };
    }

    // from OC to timer
    template<class T, typename TOC = typename details::func_fieldbit<T>::toc_t>
    inline constexpr auto oc_timer(T func){
        return timer(TOC{});
    }

    // pwm, apply mode
    template<class T, typename TVal, typename TOC = typename details::func_fieldbit<T>::toc_t>
    inline constexpr auto pwm(T func, TVal val){
        return pwm(TOC{}, val);
    }
    template<class T, typename TOC = typename details::func_fieldbit<T>::toc_t>
    inline constexpr auto nopwm(T func){
        return nopwm(TOC{});
    }

    // pwm immediate
    template<class T, typename TVal, typename TOC = typename details::func_fieldbit<T>::toc_t>
    inline void pwm_(T func, TVal val){
        pwm_(TOC{}, val);
    }
    template<class T, typename TOC = typename details::func_fieldbit<T>::toc_t>
    inline void nopwm_(T func){
        nopwm_(TOC{});
    }
}

#endif