#pragma once

#ifndef AVR_TIMERS_H_
#define AVR_TIMERS_H_

// ************* Timer0 **************
// Control
// ATTiny24: TCCR0A: COM0A1 COM0A0 COM0B1 COM0B0 �      �       WGM01  WGM00    TCCR0B: FOC0A FOC0B �      �      WGM02 CS02  CS01  CS00
// ATTiny25: TCCR0A: COM0A1 COM0A0 COM0B1 COM0B0 �      �       WGM01  WGM00    TCCR0B: FOC0A FOC0B �      �      WGM02 CS02  CS01  CS00
// Mega328:  TCCR0A: COM0A1 COM0A0 COM0B1 COM0B0 -      -       WGM01  WGM00    TCCR0B: FOC0A FOC0B -      -      WGM02 CS02  CS01  CS00
// Values:
// TCNT0, [OCR0A + OCR0B / OCR0]

// Interrupts:
// ATTiny24: TIMSK0: �      �      �      �      �      OCIE0B  OCIE0A TOIE0    TIFR0:  �     �     �      �      �     OCF0B OCF0A TOV0
// ATTiny25: TIMSK:  �      OCIE1A OCIE1B OCIE0A OCIE0B TOIE1   TOIE0  �        TIFR:   �     OCF1A OCF1B  OCF0A  OCF0B TOV1  TOV0  �
// Mega328	 TIMSK0: �      �      �      �      �      OCIE0B  OCIE0A TOIE0    TIFR0:  �     �     �      �      �     OCFB  OCFA  TOV0  


// ************* Timer1 **************
// !ATiny25 && ATTiny261 have different timer1!!!
// Control
// ATTiny24  TCCR1A: COM1A1 COM1A0 COM1B1 COM1B0 �      �       WGM11  WGM10    TCCR1B: ICNC1 ICES1 �      WGM13  WGM12 CS12  CS11  CS10   TCCR1C: FOC1A FOC1B � � � � � �
// !ATTiny25 TCCR1:  CTC1   PWM1A  COM1A1 COM1A0 CS13   CS12    CS11   CS10     GTCCR:  TSM   PWM1B COM1B1 COM1B0 FOC1B FOC1A PSR1  PSR0
// Mega328	 TCCR1A: COM1A1 COM1A0 COM1B1 COM1B0 -      -       WGM11  WGM10    TCCR1B: ICNC1 ICES1 -      WGM13  WGM12 CS12  CS11  CS10   TCCR1C: FOC1A FOC1B - - - - - -
// ATTiny25  TIMSK:  �      OCIE1A OCIE1B OCIE0A OCIE0B TOIE1   TOIE0  �        TIFR:   �     OCF1A OCF1B  OCF0A  OCF0B TOV1  TOV0  �
// Mega328   TIMSK1: -      -      ICIE1  -      -      OCIE1B  OCIE1A TOIE1    TIFR1:  -     -     ICF1   -      -     OCF1B OCF1A TOV1


#include "timer0.h"
#include "timer1.h"
#include "timer2.h"

#endif