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
#include "../../std/std_types.hpp"

namespace fasthal{
    namespace avr{
        // compare
        enum class timer_com: std::uint8_t{
            none   = 0b00,
            toggle = 0b01,
            clear  = 0b10,
            set    = 0b11
        };

        // clock source
        enum class timer_csext: std::uint8_t{
            _       = 0b000,
            _0      = 0b001,
            _8      = 0b010,
            _64     = 0b011,
            _256    = 0b100,
            _1024   = 0b101,
            _fall   = 0b110,
            _rise   = 0b111
        };
        
        enum class timer_cs: std::uint8_t{
            _       = 0b000,
            _0      = 0b001,
            _8      = 0b010,
            _32     = 0b011,
            _64     = 0b100,
            _128    = 0b101,
            _256    = 0b110,
            _1024   = 0b111
        };

        // wave generation mode
        enum class timer_wgm3: std::uint8_t{
            Normal     = 0b0000,
            PwmPc8     = 0b0001,
            PwmPc9     = 0b0010,
            PwmPc10    = 0b0011,
            CtcA       = 0b0100,
            PwmFast8   = 0b0101,
            PwmFast9   = 0b0110,
            PwmFast10  = 0b0111,
            PwmPcFcI   = 0b1000,
            PwmPcFcA   = 0b1001,
            PwmPcI     = 0b1010,
            PwmPcA     = 0b1011,
            CtcI       = 0b1100,
            //Reserved = 0b0101,
            PwmFastI   = 0b1110,
            PwmFastA   = 0b1111
        };

        enum class timer_wgm2: std::uint8_t{
            Normal     = 0b000,
            PwmPcMax   = 0b001,
            CtcA       = 0b010,
            PwmFastMax = 0b011,
            /*Reserved = 0b100,*/
            PwmPcA     = 0b101,
            /*Reserved = 0b110,*/
            PwmFastA   = 0b111
        };

        enum class timer_wgm1: uint8_t{
            Normal     = 0b00,
            PwmPcMax   = 0b01,
            CtcA       = 0b10,
            PwmFastMax = 0b11
        };


        template<unsigned VNum>
        struct timer{
            static constexpr auto available = false;
        };
    }
}

#endif