#pragma once

#ifndef AVR_TIMERS_H_
#define AVR_TIMERS_H_

// ATTiny24: TCCR0A: COM0A1 COM0A0 COM0B1 COM0B0 –      –       WGM01  WGM00    TCCR0B: FOC0A FOC0B –      –      WGM02 CS02  CS01  CS00   
//			 TCNT0, OCR0A, OCR0B																				  
//           TIMSK0: –      –      –      –      –      OCIE0B  OCIE0A TOIE0    TIFR0:  –     –     –      –      –     OCF0B OCF0A TOV0
//																												  
//           TCCR1A: COM1A1 COM1A0 COM1B1 COM1B0 –      –       WGM11  WGM10    TCCR1B: ICNC1 ICES1 –      WGM13  WGM12 CS12  CS11  CS10   TCCR1C: FOC1A FOC1B – – – – – –     
//			 TCNT1H/L, OCR1AH/L, OCR1BH/L, ICR1H/L																  
//			 TIMSK1: –	    –      ICIE1  –      –      OCIE1B  OCIE1A TOIE1    TIFR1:  –     –     ICF1   –      –     OCF1B OCF1A TOV1
																												  
// ATTiny25: TCCR0A: COM0A1 COM0A0 COM0B1 COM0B0 –      –       WGM01  WGM00    TCCR0B: FOC0A FOC0B –      –      WGM02 CS02  CS01  CS00   
//			 TCNT0, OCR0A, OCR0B
//
//			 TCCR1:  CTC1   PWM1A  COM1A1 COM1A0 CS13   CS12    CS11   CS10     GTCCR:  TSM   PWM1B COM1B1 COM1B0 FOC1B FOC1A PSR1 PSR0
//			 TCNT1, OCR1A, OCR1B, OCR1C
//           TIMSK:  –      OCIE1A OCIE1B OCIE0A OCIE0B TOIE1   TOIE0  –        TIFR:   –     OCF1A OCF1B  OCF0A  OCF0B TOV1  TOV0  –
// Mega328:  TCCR0A: COM0A1 COM0A0 COM0B1 COM0B0 -      -       WGM01  WGM00    TCCR0B: FOC0A FOC0B -      -      WGM02 CS02  CS01  CS00   //			 TCNT0, OCR0A, OCR0B//			 TIMSK0: –      –      –      –      –      OCIE0B  OCIE0A TOIE0    TIFR0:  –     –     –      –      –     OCFB  OCFA  TOV    TCCR1C: FOC1A FOC1B - - - - - -////			 TCCR1A: COM1   COM1   COM1   COM1   -      -       WGM11  WGM10    TCCR1B: ICNC1 ICES1 -      WGM13  WGM12 CS12  CS11  CS10//			 TCNT1H/L, OCR1AH/L, OCR1BH/L, ICR1H/L
//           TIMSK1: -      -      ICIE   -      -      OCIEB   OCIEA  TOIE     TIFR1:  -     -     ICF    -      -     OCFB  OCFA  TOV// Mega2560: TCCR0A: COM0A1 COM0A0 COM0B1 COM0B0 –      –       WGM01  WGM00    TCCR0B: FOC0A FOC0B –      –      WGM02 CS02  CS01  CS00   //           TCNT0, OCR0A, OCR0B//           TIMSK0: –      –      –      –      –      OCIE0B  OCIE0A TOIE0    TIFR0:  –     –     –      –      –     OCF0B OCF0A TOV0////           TCCR1A: COM1A1 COM1A0 COM1B1 COM1B0 COM1C1 COM1C0  WGM11  WGM10    TCCR1B: ICNC1 ICES1 –      WGM13  WGM12 CS12  CS11  CS10  TCCR1C: FOC1A FOC1B FOC1C – – – – –//           TCNT1H/L, OCR1AH/L, OCR1BH/L, OCR1CH/L, ICR1H/L//           TIMSK1: –      –      ICIE1  –      OCIE1C OCIE1B  OCIE1A TOIE1    TIFR1:  –     –     ICF1    –     OCF1C OCF1B OCF1A TOV1// Mega8:                                                                       TCCR0:  –     –     –      –      –     CS02  CS01  CS00   //			 TCNT0////           TCCR1A: COM1A1 COM1A0 COM1B1 COM1B0 FOC1A FOC1B WGM11 WGM10        TCCR1B: ICNC1 ICES1 –      WGM13  WGM12 CS12  CS11  CS10//           TCNT1H/L, OCR1AH/L, OCR1BH/L, ICR1H/L//           TIMSK:  OCIE2  TOIE2  TICIE1 OCIE1A OCIE1B TOIE1   –      TOIE0    TIFR:   OCF2  TOV2  ICF1   OCF1A  OCF1B TOV1  –     TOV0// Mega32:                                                                      TCCR0:  FOC0  WGM00 COM01  COM00  WGM01 CS02  CS01  CS00   //	         TCNT0, OCR0////			 TCCR1A: COM1A1 COM1A0 COM1B1 COM1B0 FOC1A  FOC1B   WGM11  WGM10    TCCR1B: ICNC1 ICES1 –      WGM13  WGM12 CS12  CS11  CS10//           TCNT1H/L, OCR1AH/L, OCR1BH/L, ICR1H/L//           TIMSK:  OCIE2  TOIE2  TICIE1 OCIE1A OCIE1B TOIE1   OCIE0  TOIE0    TIFR:   OCF2  TOV2  ICF1   OCF1A  OCF1B TOV1  OCF0  TOV0// Mega128:                                                                     TCCR0:  FOC0  WGM00 COM01  COM00  WGM01 CS02  CS0   CS00////			 TCCR1A: COM1A1 COM1A0 COM1B1 COM1B0  COM1C1 COM1C0 WGM11   WGM10   TCCR1B: ICNC1 ICES1 –      WGM13  WGM12 CS12  CS11  CS10   TCCR1C: FOC1A FOC1B FOC1C – – – – –//           TIMSK:  OCIE2  TOIE2  TICIE1 OCIE1A  OCIE1B TOIE1  OCIE0   TOIE0   TIFR:   OCF2  TOV2  ICF1   OCF1A  OCF1B TOV1  OCF0  TOV0   TCNT0 OCR0

#include "timer0.h"
//#include "timer1.h"

#endif