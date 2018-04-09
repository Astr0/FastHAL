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
*/

#ifndef FH_PINS_FASTDUINO_H_
#define FH_PINS_FASTDUINO_H_



#define NUM_DIGITAL_PINS            70
#define NUM_ANALOG_INPUTS           16
#define analogInputToDigitalPin(p)  ((p < 16) ? (p) + 54 : -1)
#define digitalPinHasPWM(p)         (((p) >= 2 && (p) <= 13) || ((p) >= 44 && (p)<= 46))

#define PIN_SPI_SS    (53)
#define PIN_SPI_MOSI  (51)
#define PIN_SPI_MISO  (50)
#define PIN_SPI_SCK   (52)

static constexpr std::uint8_t SS   = PIN_SPI_SS;
static constexpr std::uint8_t MOSI = PIN_SPI_MOSI;
static constexpr std::uint8_t MISO = PIN_SPI_MISO;
static constexpr std::uint8_t SCK  = PIN_SPI_SCK;

#define PIN_WIRE_SDA        (20)
#define PIN_WIRE_SCL        (21)

static constexpr std::uint8_t SDA = PIN_WIRE_SDA;
static constexpr std::uint8_t SCL = PIN_WIRE_SCL;

#define LED_BUILTIN 13

#define PIN_A0   (54)
#define PIN_A1   (55)
#define PIN_A2   (56)
#define PIN_A3   (57)
#define PIN_A4   (58)
#define PIN_A5   (59)
#define PIN_A6   (60)
#define PIN_A7   (61)
#define PIN_A8   (62)
#define PIN_A9   (63)
#define PIN_A10  (64)
#define PIN_A11  (65)
#define PIN_A12  (66)
#define PIN_A13  (67)
#define PIN_A14  (68)
#define PIN_A15  (69)

static constexpr std::uint8_t A0 = PIN_A0;
static constexpr std::uint8_t A1 = PIN_A1;
static constexpr std::uint8_t A2 = PIN_A2;
static constexpr std::uint8_t A3 = PIN_A3;
static constexpr std::uint8_t A4 = PIN_A4;
static constexpr std::uint8_t A5 = PIN_A5;
static constexpr std::uint8_t A6 = PIN_A6;
static constexpr std::uint8_t A7 = PIN_A7;
static constexpr std::uint8_t A8 = PIN_A8;
static constexpr std::uint8_t A9 = PIN_A9;
static constexpr std::uint8_t A10 = PIN_A10;
static constexpr std::uint8_t A11 = PIN_A11;
static constexpr std::uint8_t A12 = PIN_A12;
static constexpr std::uint8_t A13 = PIN_A13;
static constexpr std::uint8_t A14 = PIN_A14;
static constexpr std::uint8_t A15 = PIN_A15;

// A majority of the pins are NOT PCINTs, SO BE WARNED (i.e. you cannot use them as receive pins)
// Only pins available for RECEIVE (TRANSMIT can be on any pin):
// (I've deliberately left out pin mapping to the Hardware USARTs - seems senseless to me)
// Pins: 10, 11, 12, 13,  50, 51, 52, 53,  62, 63, 64, 65, 66, 67, 68, 69

#define digitalPinToPCICR(p)    ( (((p) >= 10) && ((p) <= 13)) || \
                                  (((p) >= 50) && ((p) <= 53)) || \
                                  (((p) >= 62) && ((p) <= 69)) ? (&PCICR) : ((std::uint8_t *)0) )

#define digitalPinToPCICRbit(p) ( (((p) >= 10) && ((p) <= 13)) || (((p) >= 50) && ((p) <= 53)) ? 0 : \
                                ( (((p) >= 62) && ((p) <= 69)) ? 2 : \
                                0 ) )

#define digitalPinToPCMSK(p)    ( (((p) >= 10) && ((p) <= 13)) || (((p) >= 50) && ((p) <= 53)) ? (&PCMSK0) : \
                                ( (((p) >= 62) && ((p) <= 69)) ? (&PCMSK2) : \
                                ((std::uint8_t *)0) ) )

#define digitalPinToPCMSKbit(p) ( (((p) >= 10) && ((p) <= 13)) ? ((p) - 6) : \
                                ( ((p) == 50) ? 3 : \
                                ( ((p) == 51) ? 2 : \
                                ( ((p) == 52) ? 1 : \
                                ( ((p) == 53) ? 0 : \
                                ( (((p) >= 62) && ((p) <= 69)) ? ((p) - 62) : \
                                0 ) ) ) ) ) )

#define digitalPinToInterrupt(p) ((p) == 2 ? 0 : ((p) == 3 ? 1 : ((p) >= 18 && (p) <= 21 ? 23 - (p) : NOT_AN_INTERRUPT)))

#if (2 + 2 == 4)







constexpr std::uint8_t ino_port_num[] = {
	// PORTLIST		
	// -------------------------------------------		
	5	, // 5 0 ** 0 ** USART0_RX	
	5	, // 5 1 ** 1 ** USART0_TX	
	5	, // 5 4 ** 2 ** PWM2	
	5	, // 5 5 ** 3 ** PWM3	
	7	, // 7 5 ** 4 ** PWM4	
	5	, // 5 3 ** 5 ** PWM5	
	8	, // 8 3 ** 6 ** PWM6	
	8	, // 8 4 ** 7 ** PWM7	
	8	, // 8 5 ** 8 ** PWM8	
	8	, // 8 6 ** 9 ** PWM9	
	2	, // 2 4 ** 10 ** PWM10	
	2	, // 2 5 ** 11 ** PWM11	
	2	, // 2 6 ** 12 ** PWM12	
	2	, // 2 7 ** 13 ** PWM13	
	9	, // 9 1 ** 14 ** USART3_TX	
	9	, // 9 0 ** 15 ** USART3_RX	
	8	, // 8 1 ** 16 ** USART2_TX	
	8	, // 8 0 ** 17 ** USART2_RX	
	4	, // 4 3 ** 18 ** USART1_TX	
	4	, // 4 2 ** 19 ** USART1_RX	
	4	, // 4 1 ** 20 ** I2C_SDA	
	4	, // 4 0 ** 21 ** I2C_SCL	
	1	, // 1 0 ** 22 ** D22	
	1	, // 1 1 ** 23 ** D23	
	1	, // 1 2 ** 24 ** D24	
	1	, // 1 3 ** 25 ** D25	
	1	, // 1 4 ** 26 ** D26	
	1	, // 1 5 ** 27 ** D27	
	1	, // 1 6 ** 28 ** D28	
	1	, // 1 7 ** 29 ** D29	
	3	, // 3 7 ** 30 ** D30	
	3	, // 3 6 ** 31 ** D31	
	3	, // 3 5 ** 32 ** D32	
	3	, // 3 4 ** 33 ** D33	
	3	, // 3 3 ** 34 ** D34	
	3	, // 3 2 ** 35 ** D35	
	3	, // 3 1 ** 36 ** D36	
	3	, // 3 0 ** 37 ** D37	
	4	, // 4 7 ** 38 ** D38	
	7	, // 7 2 ** 39 ** D39	
	7	, // 7 1 ** 40 ** D40	
	7	, // 7 0 ** 41 ** D41	
	11	, // 11 7 ** 42 ** D42	
	11	, // 11 6 ** 43 ** D43	
	11	, // 11 5 ** 44 ** D44	
	11	, // 11 4 ** 45 ** D45	
	11	, // 11 3 ** 46 ** D46	
	11	, // 11 2 ** 47 ** D47	
	11	, // 11 1 ** 48 ** D48	
	11	, // 11 0 ** 49 ** D49	
	2	, // 2 3 ** 50 ** SPI_MISO	
	2	, // 2 2 ** 51 ** SPI_MOSI	
	2	, // 2 1 ** 52 ** SPI_SCK	
	2	, // 2 0 ** 53 ** SPI_SS	
	6	, // 6 0 ** 54 ** A0	
	6	, // 6 1 ** 55 ** A1	
	6	, // 6 2 ** 56 ** A2	
	6	, // 6 3 ** 57 ** A3	
	6	, // 6 4 ** 58 ** A4	
	6	, // 6 5 ** 59 ** A5	
	6	, // 6 6 ** 60 ** A6	
	6	, // 6 7 ** 61 ** A7	
	10	, // 10 0 ** 62 ** A8	
	10	, // 10 1 ** 63 ** A9	
	10	, // 10 2 ** 64 ** A10	
	10	, // 10 3 ** 65 ** A11	
	10	, // 10 4 ** 66 ** A12	
	10	, // 10 5 ** 67 ** A13	
	10	, // 10 6 ** 68 ** A14	
	10	, // 10 7 ** 69 ** A15	
};

constexpr std::uint8_t ino_pin_num[] = {
	// PIN IN PORT		
	// -------------------------------------------		
	 0 	, // PE 0 ** 0 ** USART0_RX	
	 1 	, // PE 1 ** 1 ** USART0_TX	
	 4 	, // PE 4 ** 2 ** PWM2	
	 5 	, // PE 5 ** 3 ** PWM3	
	 5 	, // PG 5 ** 4 ** PWM4	
	 3 	, // PE 3 ** 5 ** PWM5	
	 3 	, // PH 3 ** 6 ** PWM6	
	 4 	, // PH 4 ** 7 ** PWM7	
	 5 	, // PH 5 ** 8 ** PWM8	
	 6 	, // PH 6 ** 9 ** PWM9	
	 4 	, // PB 4 ** 10 ** PWM10	
	 5 	, // PB 5 ** 11 ** PWM11	
	 6 	, // PB 6 ** 12 ** PWM12	
	 7 	, // PB 7 ** 13 ** PWM13	
	 1 	, // PJ 1 ** 14 ** USART3_TX	
	 0 	, // PJ 0 ** 15 ** USART3_RX	
	 1 	, // PH 1 ** 16 ** USART2_TX	
	 0 	, // PH 0 ** 17 ** USART2_RX	
	 3 	, // PD 3 ** 18 ** USART1_TX	
	 2 	, // PD 2 ** 19 ** USART1_RX	
	 1 	, // PD 1 ** 20 ** I2C_SDA	
	 0 	, // PD 0 ** 21 ** I2C_SCL	
	 0 	, // PA 0 ** 22 ** D22	
	 1 	, // PA 1 ** 23 ** D23	
	 2 	, // PA 2 ** 24 ** D24	
	 3 	, // PA 3 ** 25 ** D25	
	 4 	, // PA 4 ** 26 ** D26	
	 5 	, // PA 5 ** 27 ** D27	
	 6 	, // PA 6 ** 28 ** D28	
	 7 	, // PA 7 ** 29 ** D29	
	 7 	, // PC 7 ** 30 ** D30	
	 6 	, // PC 6 ** 31 ** D31	
	 5 	, // PC 5 ** 32 ** D32	
	 4 	, // PC 4 ** 33 ** D33	
	 3 	, // PC 3 ** 34 ** D34	
	 2 	, // PC 2 ** 35 ** D35	
	 1 	, // PC 1 ** 36 ** D36	
	 0 	, // PC 0 ** 37 ** D37	
	 7 	, // PD 7 ** 38 ** D38	
	 2 	, // PG 2 ** 39 ** D39	
	 1 	, // PG 1 ** 40 ** D40	
	 0 	, // PG 0 ** 41 ** D41	
	 7 	, // PL 7 ** 42 ** D42	
	 6 	, // PL 6 ** 43 ** D43	
	 5 	, // PL 5 ** 44 ** D44	
	 4 	, // PL 4 ** 45 ** D45	
	 3 	, // PL 3 ** 46 ** D46	
	 2 	, // PL 2 ** 47 ** D47	
	 1 	, // PL 1 ** 48 ** D48	
	 0 	, // PL 0 ** 49 ** D49	
	 3 	, // PB 3 ** 50 ** SPI_MISO	
	 2 	, // PB 2 ** 51 ** SPI_MOSI	
	 1 	, // PB 1 ** 52 ** SPI_SCK	
	 0 	, // PB 0 ** 53 ** SPI_SS	
	 0 	, // PF 0 ** 54 ** A0	
	 1 	, // PF 1 ** 55 ** A1	
	 2 	, // PF 2 ** 56 ** A2	
	 3 	, // PF 3 ** 57 ** A3	
	 4 	, // PF 4 ** 58 ** A4	
	 5 	, // PF 5 ** 59 ** A5	
	 6 	, // PF 6 ** 60 ** A6	
	 7 	, // PF 7 ** 61 ** A7	
	 0 	, // PK 0 ** 62 ** A8	
	 1 	, // PK 1 ** 63 ** A9	
	 2 	, // PK 2 ** 64 ** A10	
	 3 	, // PK 3 ** 65 ** A11	
	 4 	, // PK 4 ** 66 ** A12	
	 5 	, // PK 5 ** 67 ** A13	
	 6 	, // PK 6 ** 68 ** A14	
	 7 	, // PK 7 ** 69 ** A15	
};

constexpr std::uint8_t ino_timer_oc_num[] = {
	// TIMERS		
	// -------------------------------------------		
	0	, // PE 0 ** 0 ** USART0_RX	
	0	, // PE 1 ** 1 ** USART0_TX	
	(3 * 3 + 1)	, // PE 4 ** 2 ** PWM2	
	(3 * 3 + 2)	, // PE 5 ** 3 ** PWM3	
	(0 * 3 + 1)	, // PG 5 ** 4 ** PWM4	
	(3 * 3 + 0)	, // PE 3 ** 5 ** PWM5	
	(4 * 3 + 0)	, // PH 3 ** 6 ** PWM6	
	(4 * 3 + 1)	, // PH 4 ** 7 ** PWM7	
	(4 * 3 + 2)	, // PH 5 ** 8 ** PWM8	
	(2 * 3 + 1)	, // PH 6 ** 9 ** PWM9	
	(2 * 3 + 0)	, // PB 4 ** 10 ** PWM10	
	(1 * 3 + 0)	, // PB 5 ** 11 ** PWM11	
	(1 * 3 + 1)	, // PB 6 ** 12 ** PWM12	
	(0 * 3 + 0)	, // PB 7 ** 13 ** PWM13	
	0	, // PJ 1 ** 14 ** USART3_TX	
	0	, // PJ 0 ** 15 ** USART3_RX	
	0	, // PH 1 ** 16 ** USART2_TX	
	0	, // PH 0 ** 17 ** USART2_RX	
	0	, // PD 3 ** 18 ** USART1_TX	
	0	, // PD 2 ** 19 ** USART1_RX	
	0	, // PD 1 ** 20 ** I2C_SDA	
	0	, // PD 0 ** 21 ** I2C_SCL	
	0	, // PA 0 ** 22 ** D22	
	0	, // PA 1 ** 23 ** D23	
	0	, // PA 2 ** 24 ** D24	
	0	, // PA 3 ** 25 ** D25	
	0	, // PA 4 ** 26 ** D26	
	0	, // PA 5 ** 27 ** D27	
	0	, // PA 6 ** 28 ** D28	
	0	, // PA 7 ** 29 ** D29	
	0	, // PC 7 ** 30 ** D30	
	0	, // PC 6 ** 31 ** D31	
	0	, // PC 5 ** 32 ** D32	
	0	, // PC 4 ** 33 ** D33	
	0	, // PC 3 ** 34 ** D34	
	0	, // PC 2 ** 35 ** D35	
	0	, // PC 1 ** 36 ** D36	
	0	, // PC 0 ** 37 ** D37	
	0	, // PD 7 ** 38 ** D38	
	0	, // PG 2 ** 39 ** D39	
	0	, // PG 1 ** 40 ** D40	
	0	, // PG 0 ** 41 ** D41	
	0	, // PL 7 ** 42 ** D42	
	0	, // PL 6 ** 43 ** D43	
	(5 * 3 + 2)	, // PL 5 ** 44 ** D44	
	(5 * 3 + 1)	, // PL 4 ** 45 ** D45	
	(5 * 3 + 0)	, // PL 3 ** 46 ** D46	
	0	, // PL 2 ** 47 ** D47	
	0	, // PL 1 ** 48 ** D48	
	0	, // PL 0 ** 49 ** D49	
	0	, // PB 3 ** 50 ** SPI_MISO	
	0	, // PB 2 ** 51 ** SPI_MOSI	
	0	, // PB 1 ** 52 ** SPI_SCK	
	0	, // PB 0 ** 53 ** SPI_SS	
	0	, // PF 0 ** 54 ** A0	
	0	, // PF 1 ** 55 ** A1	
	0	, // PF 2 ** 56 ** A2	
	0	, // PF 3 ** 57 ** A3	
	0	, // PF 4 ** 58 ** A4	
	0	, // PF 5 ** 59 ** A5	
	0	, // PF 6 ** 60 ** A6	
	0	, // PF 7 ** 61 ** A7	
	0	, // PK 0 ** 62 ** A8	
	0	, // PK 1 ** 63 ** A9	
	0	, // PK 2 ** 64 ** A10	
	0	, // PK 3 ** 65 ** A11	
	0	, // PK 4 ** 66 ** A12	
	0	, // PK 5 ** 67 ** A13	
	0	, // PK 6 ** 68 ** A14	
	0	, // PK 7 ** 69 ** A15	
};

#endif

// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
#define SERIAL_PORT_MONITOR         Serial
#define SERIAL_PORT_HARDWARE        Serial
#define SERIAL_PORT_HARDWARE1       Serial1
#define SERIAL_PORT_HARDWARE2       Serial2
#define SERIAL_PORT_HARDWARE3       Serial3
#define SERIAL_PORT_HARDWARE_OPEN   Serial1
#define SERIAL_PORT_HARDWARE_OPEN1  Serial2
#define SERIAL_PORT_HARDWARE_OPEN2  Serial3

#endif
