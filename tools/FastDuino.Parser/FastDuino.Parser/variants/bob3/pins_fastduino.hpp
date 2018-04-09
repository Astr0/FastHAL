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

/*
  BOB3 library for ARDUINO
  License: BSD-License
  (c) 2013 by Nils Springob, nicai-systems
*/

#ifndef FH_PINS_FASTDUINO_H_
#define FH_PINS_FASTDUINO_H_



#define NUM_DIGITAL_PINS            22
#define NUM_ANALOG_INPUTS           6
#define analogInputToDigitalPin(p)  ((p < 6) ? (p) + 16 : -1)

#define digitalPinHasPWM(p)         ((p) == 3 || (p) == 5 || (p) == 6 || (p) == 9 || (p) == 10 || (p) == 11)

#define PIN_SPI_SS    (10)
#define PIN_SPI_MOSI  (11)
#define PIN_SPI_MISO  (12)
#define PIN_SPI_SCK   (13)

static constexpr std::uint8_t SS   = PIN_SPI_SS;
static constexpr std::uint8_t MOSI = PIN_SPI_MOSI;
static constexpr std::uint8_t MISO = PIN_SPI_MISO;
static constexpr std::uint8_t SCK  = PIN_SPI_SCK;

#define PIN_WIRE_SDA        (20)
#define PIN_WIRE_SCL        (21)

static constexpr std::uint8_t SDA = PIN_WIRE_SDA;
static constexpr std::uint8_t SCL = PIN_WIRE_SCL;

#define LED_BUILTIN 13

#define PIN_A0   (16)
#define PIN_A1   (17)
#define PIN_A2   (18)
#define PIN_A3   (19)
#define PIN_A4   (20)
#define PIN_A5   (21)
#define PIN_A6   (22)
#define PIN_A7   (23)

static constexpr std::uint8_t A0 = PIN_A0;
static constexpr std::uint8_t A1 = PIN_A1;
static constexpr std::uint8_t A2 = PIN_A2;
static constexpr std::uint8_t A3 = PIN_A3;
static constexpr std::uint8_t A4 = PIN_A4;
static constexpr std::uint8_t A5 = PIN_A5;
static constexpr std::uint8_t A6 = PIN_A6;
static constexpr std::uint8_t A7 = PIN_A7;

#define digitalPinToPCICR(p)    (((p) >= 0 && (p) <= 23) ? (&PCICR) : ((std::uint8_t *)0))
#define digitalPinToPCICRbit(p) (((p) <= 7) ? 2 : (((p) <= 15) ? 0 : 1))
#define digitalPinToPCMSK(p)    (((p) <= 7) ? (&PCMSK2) : (((p) <= 15) ? (&PCMSK0) : (((p) <= 23) ? (&PCMSK1) : ((std::uint8_t *)0))))
#define digitalPinToPCMSKbit(p) (((p) <= 7) ? (p) : (((p) <= 15) ? ((p) - 8) : ((p) - 16)))

#define digitalPinToInterrupt(p)  ((p) == 2 ? 0 : ((p) == 3 ? 1 : NOT_AN_INTERRUPT))

#if (2 + 2 == 4)

// On the Arduino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

// ATMEL ATMEGA88A
//
//                  +-\/-+
//            PC6  1|    |28  PC5 (AI 5)
//      (D 0) PD0  2|    |27  PC4 (AI 4)
//      (D 1) PD1  3|    |26  PC3 (AI 3)
//      (D 2) PD2  4|    |25  PC2 (AI 2)
// PWM  (D 3) PD3  5|    |24  PC1 (AI 1)
//      (D 4) PD4  6|    |23  PC0 (AI 0)
//            VCC  7|    |22  GND
//            GND  8|    |21  AREF
//     (D 14) PB6  9|    |20  AVCC
//     (D 15) PB7 10|    |19  PB5 (D 13)
// PWM  (D 5) PD5 11|    |18  PB4 (D 12)
// PWM  (D 6) PD6 12|    |17  PB3 (D 11) PWM
//      (D 7) PD7 13|    |16  PB2 (D 10) PWM
//      (D 8) PB0 14|    |15  PB1 (D 9) PWM
//                  +----+
//


// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)






constexpr std::uint8_t ino_port_num[] = {
	4, /* 0 */
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	2, /* 8 */
	2,
	2,
	2,
	2,
	2,
	2,
	2,
	3, /* 16 */
	3,
	3,
	3,
	3,
	3,
};

constexpr std::uint8_t ino_pin_num[] = {
	0, /* 0, port D */
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	0, /* 8, port B */
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	0, /* 16, port C */
	1,
	2,
	3,
	4,
	5,
};

constexpr std::uint8_t ino_timer_oc_num[] = {
	0, /* 0 - port D */
	0,
	0,
	(2 * 3 + 1),
	0,
	(0 * 3 + 1),
	(0 * 3 + 0),
	0,
	0, /* 8 - port B */
	(1 * 3 + 0),
	(1 * 3 + 1),
	(2 * 3 + 0),
	0,
	0,
	0,
	0,
	0,
	0, /* 16 - port C */
	0,
	0,
	0,
	0,
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
#define SERIAL_PORT_MONITOR   Serial
#define SERIAL_PORT_HARDWARE  Serial

#endif
