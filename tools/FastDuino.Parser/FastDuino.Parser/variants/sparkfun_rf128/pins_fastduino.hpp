// ********************* Generated by a tool *********************
/*
  pins_fastduino.hpp - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: wiring.h 249 2007-02-03 16:52:51Z mellis $
*/

/*
	This version of pins_fastduino.hpp is for the Zigduino r1
	Pierce Nichols 2011 Oct 11
	Amended for the SparkFun ATmega128RFA1 -- Jim Lindblom 2013 Jun 29
*/

#ifndef FH_PINS_FASTDUINO_H_
#define FH_PINS_FASTDUINO_H_



#define NUM_DIGITAL_PINS            26
#define NUM_ANALOG_INPUTS           8
#define analogInputToDigitalPin(p)  ((p < 8) ? (p) + 26 : -1)
#define digitalPinHasPWM(p)         ((p) == 3 ||(p) == 4 ||(p) == 5 ||(p) == 8 ||(p) == 9 ||(p) == 19 ||(p) == 34 || (p) == 35)

// Dev board specific defines: RF RX and TX LEDs:
#define RXLED_DDR  DDRB
#define RXLED_PORT PORTB
#define RXLED_POS  PB6

#define TXLED_DDR  DDRB
#define TXLED_PORT PORTB
#define TXLED_POS  PB7  

constexpr static std::uint8_t SS   = 10;
constexpr static std::uint8_t MOSI = 11;
constexpr static std::uint8_t MISO = 12;
constexpr static std::uint8_t SCK  = 13;

constexpr static std::uint8_t SDA = 14;
constexpr static std::uint8_t SCL = 15;
constexpr static std::uint8_t LED = 34;
constexpr static std::uint8_t LED1 = 34;
constexpr static std::uint8_t LED2 = 35;

constexpr static std::uint8_t A0 = 26;
constexpr static std::uint8_t A1 = 27;
constexpr static std::uint8_t A2 = 28;
constexpr static std::uint8_t A3 = 29;
constexpr static std::uint8_t A4 = 30;
constexpr static std::uint8_t A5 = 31;
constexpr static std::uint8_t A6 = 32;
constexpr static std::uint8_t A7 = 33;

// A majority of the pins are NOT PCINTs, SO BE WARNED (i.e. you cannot use them as receive pins)
// Only pins available for RECEIVE (TRANSMIT can be on any pin):
// Pins: 7, 8, 9, 10, 11, 12, 13, 20

#define digitalPinToPCICR(p)    ( (((p) >= 7) && ((p) <= 13)) || \
                                  ((p) == 20) ? (&PCICR) : ((std::uint8_t *)0) )

#define digitalPinToPCICRbit(p) ( ((p) == 7) ? 1 : 0 ) 

#define digitalPinToPCMSK(p)    ( ((((p) >= 8) && ((p) <= 13)) || ((p) == 20)) ? (&PCMSK0) : \
                                ( ((p) == 7) ? (&PCMSK1) : \
                                ((std::uint8_t *)0) ) )

#define digitalPinToPCMSKbit(p) ( ((p) == 7) ? 0 : \
                                ( ((p) == 8) ? 4 : \
                                ( ((p) == 9) ? 7 : \
                                ( ((p) == 10) ? 6 : \
                                ( ((p) == 11) ? 5 : \
                                ( ((p) == 12) ? 3 : \
                                ( ((p) == 13) ? 1 : \
                                ( ((p) == 20) ? 2 : \
                                0 ) ) ) ) ) )

#if (2 + 2 == 4)







constexpr std::uint8_t ino_port_num[] = {
	// PORTLIST		
	// -------------------------------------------		
	5	, // 5 0 ** 0 ** D0 / USART0_RX	
	5	, // 5 1 ** 1 ** D1 / USART0_TX	
	5	, // 5 2 ** 2 ** D2
	5	, // 5 3 ** 3 ** D3 / PWM
	5	, // 5 4 ** 4 ** D4 / PWM
	5	, // 5 5 ** 5 ** D5 / PWM
	5	, // 5 6 ** 6 ** D6
	5	, // 5 7 ** 7 ** D7
	2	, // 2 5 ** 8 ** D8 / PWM
	2	, // 2 4 ** 9 ** D9 / PWM
	2	, // 2 0 ** 10 ** D10 / SPI_SSN
	2	, // 2 2 ** 11 ** D11 / SPI_MOSI
	2	, // 2 3 ** 12 ** D12 / SPI_MISO
	2	, // 2 1 ** 13 ** D13 / SPI_SCK
	4	, // 4 1 ** 14 ** D14 / I2C_SDA
	4	, // 4 0 ** 15 ** D15 / I2C_SCL
	7	, // 7 0 ** 16 ** D16
	7	, // 7 1 ** 17 ** D17
	7	, // 7 2 ** 18 ** D18
	7	, // 7 5 ** 19 ** D19 / PWM
	4	, // 4 2 ** 20 ** D20 / USART1_RX
	4	, // 4 3 ** 21 ** D21 / USART1_TX
	4	, // 4 4 ** 22 ** D22
	4	, // 4 5 ** 23 ** D23
	4	, // 4 6 ** 24 ** D24
	4	, // 4 7 ** 25 ** D25
	6	, // 6 0 ** 26 ** A0 / D26
	6	, // 6 1 ** 27 ** A1 / D27
	6	, // 6 2 ** 28 ** A2 / D28
	6	, // 6 3 ** 29 ** A3 / D29
	6	, // 6 4 ** 30 ** A4 / D30	
	6	, // 6 5 ** 31 ** A5 / D31	
	6	, // 6 6 ** 32 ** A6 / D32	
	6	, // 6 7 ** 33 ** A7 / D33
	2	, // 2 6 ** 34 ** D34 / LED1 / LED / PWM
	2	, // 2 7 ** 35 ** D35 / LED2 / PWM
};

constexpr std::uint8_t ino_pin_num[] = {
	// PIN IN PORT		
	// -------------------------------------------		
	 0 	, // PE 0 ** 0 ** USART0_RX	
	 1 	, // PE 1 ** 1 ** USART0_TX	
	 2 	, // PE 2 ** 2 ** D2
	 3 	, // PE 3 ** 3 ** D3 / PWM
	 4 	, // PE 4 ** 4 ** D4 / PWM
	 5 	, // PE 5 ** 5 ** D5 / PWM
	 6 	, // PE 6 ** 6 ** D6
	 7 	, // PE 7 ** 7 ** D7
	 5 	, // PB 5 ** 8 ** D8 / PWM
	 4 	, // PB 4 ** 9 ** D9 / PWM
	 0 	, // PB 0 ** 10 ** D10 / SPI_SSN
	 2 	, // PB 2 ** 11 ** D11 / SPI_MOSI
	 3 	, // PB 3 ** 12 ** D12 / SPI_MISO
	 1 	, // PB 1 ** 13 ** D13 / SPI_SCK
	 1 	, // PD 1 ** 14 ** D14 / I2C_SDA
	 0 	, // PD 0 ** 15 ** D15 / I2C_SCL
	 0 	, // PG 0 ** 16 ** D16
	 1 	, // PG 1 ** 17 ** D17	
	 2 	, // PG 2 ** 18 ** D18	
	 5 	, // PG 5 ** 19 ** D19 / PWM
	 2 	, // PD 2 ** 20 ** D20 / USART1_RX
	 3 	, // PD 3 ** 21 ** D21 / USART1_TX
	 4 	, // PD 4 ** 22 ** D22
	 5   	, // PD 5 ** 23 ** D23
	 6   	, // PD 6 ** 24 ** D24
	 7 	, // PD 7 ** 25 ** D25
	 0 	, // PF 0 ** 26 ** A0 / D26
	 1 	, // PF 1 ** 27 ** A1 / D27
	 2 	, // PF 2 ** 28 ** A2 / D28
	 3 	, // PF 3 ** 29 ** A3 / D29
	 4 	, // PF 4 ** 30 ** A4 / D30	
	 5 	, // PF 5 ** 31 ** A5 / D31	
	 6 	, // PF 6 ** 32 ** A6 / D32	
	 7 	, // PF 7 ** 33 ** A7 / D33	
	 6 	, // PB 6 ** 34 ** D34 / LED1 / LED / PWM
	 7 	, // PB 7 ** 35 ** D35 / LED2 / PWM
};

// !!!
constexpr std::uint8_t ino_timer_oc_num[] = {
	// TIMERS		
	// -------------------------------------------	
	255	, // PE 0 ** 0 ** USART0_RX	
	255	, // PE 1 ** 1 ** USART0_TX	
	255	, // PE 2 ** 2 ** D2
	(3'0)			, // PE 3 ** 3 ** D3 / PWM
	(3'1)			, // PE 4 ** 4 ** D4 / PWM
	(3'2)			, // PE 5 ** 5 ** D5 / PWM
	255	, // PE 6 ** 6 ** D6
	255	, // PE 7 ** 7 ** D7
	(1'0)			, // PB 5 ** 8 ** D8 / PWM
	(2'0)			, // PB 4 ** 9 ** D9 / PWM
	255	, // PB 0 ** 10 ** D10 / SPI_SSN
	255	, // PB 2 ** 11 ** D11 / SPI_MOSI
	255	, // PB 3 ** 12 ** D12 / SPI_MISO
	255	, // PB 1 ** 13 ** D13 / SPI_SCK
	255	, // PD 1 ** 14 ** D14 / I2C_SDA
	255	, // PD 0 ** 15 ** D15 / I2C_SCL
	255	, // PG 0 ** 16 ** D16
	255	, // PG 1 ** 17 ** D17	
	255	, // PG 2 ** 18 ** D18	
	(0'1)			, // PG 5 ** 19 ** D19 / PWM
	255	, // PD 2 ** 20 ** D20 / USART1_RX
	255	, // PD 3 ** 21 ** D21 / USART1_TX
	255	, // PD 4 ** 22 ** D22
	255  	, // PD 5 ** 23 ** D23
	255  	, // PD 6 ** 24 ** D24
	255	, // PD 7 ** 25 ** D25
	255	, // PF 0 ** 26 ** A0 / D26
	255	, // PF 1 ** 27 ** A1 / D27
	255	, // PF 2 ** 28 ** A2 / D28
	255	, // PF 3 ** 29 ** A3 / D29
	255	, // PF 4 ** 30 ** A4 / D30	
	255	, // PF 5 ** 31 ** A5 / D31	
	255	, // PF 6 ** 32 ** A6 / D32	
	255	, // PF 7 ** 33 ** A7 / D33	
	(1'1)			, // PB 6 ** 34 ** D34/ PWM
	(1'2)			, // PB 7 ** 35 ** D35 / PWM
};	

#endif

#endif