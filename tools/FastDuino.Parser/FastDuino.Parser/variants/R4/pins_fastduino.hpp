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

#ifndef FH_PINS_FASTDUINO_H_
#define FH_PINS_FASTDUINO_H_



// #include "avr/iom328p.h"

#undef LED_BUILTIN
#define LED_BUILTIN  (13)

// Pin  0 -  7: PD0-7
// Pin  8 - 13: PB0-5
// Pin 14 - 19: PC0-5
// Pin 20 - 21: PE2-3
// Pin 22 - 23: PE0-1

#define FH_NUM_DIGITAL_PINS  (24) // R4 has 24 instead of 20 GPIOs.
#define FH_NUM_ANALOG_INPUTS   (8) // R4 has 8 instead of 6 analog inputs.

#define analogInputToDigitalPin(p)  (((p)<8)? (p)+14 : -1)

#define digitalPinHasPWM(p)  ((p)<=3) || (p)==5 || (p)==6 || (p)==9 || (p)==10 || (p)==11)

// SPI, SPI is also SPI0
static constexpr uint8_t SS   = 10; // PB2
static constexpr uint8_t MOSI = 11; // PB3
static constexpr uint8_t MISO = 12; // PB4
static constexpr uint8_t SCK  = 13; // PB5
// SPI0
static constexpr uint8_t SS0   = 10; // PB2
static constexpr uint8_t MOSI0 = 11; // PB3
static constexpr uint8_t MISO0 = 12; // PB4
static constexpr uint8_t SCK0  = 13; // PB5
// SPI1
static constexpr uint8_t SS1   = 20; // PE2
static constexpr uint8_t MOSI1 = 21; // PE3
static constexpr uint8_t MISO1 = 14; // PC0
static constexpr uint8_t SCK1  = 15; // PC1

// I2C / TWI, SDA/SCL is also SDA0/SCL0
static constexpr uint8_t SDA = 18; // PC4
static constexpr uint8_t SCL = 19; // PC5
// I2C0
static constexpr uint8_t SDA0 = 18; // PC4
static constexpr uint8_t SCL0 = 19; // PC5
// I2C1
static constexpr uint8_t SDA1 = 22; // PE0
static constexpr uint8_t SCL1 = 23; // PE1

// Analog inputs
static constexpr uint8_t A0 = 14; // PC0
static constexpr uint8_t A1 = 15; // PC1
static constexpr uint8_t A2 = 16; // PC2
static constexpr uint8_t A3 = 17; // PC3
static constexpr uint8_t A4 = 18; // PC4
static constexpr uint8_t A5 = 19; // PC5
static constexpr uint8_t A6 = 20; // PE2
static constexpr uint8_t A7 = 21; // PE3

// Pin Change Interrupt
#define digitalPinToPCICR(p)    (((p)<=23) ? (&PCICR) : ((uint8_t *)0))
                                // Port D         Port B          Port C     Port C
#define digitalPinToPCICRbit(p) (((p)<=7)? 2 : (((p)<=13)? 0 : (((p)<=19)? 1 : 3)))
#define digitalPinToPCMSK(p)    (((p)<=7)? (&PCMSK2) : (((p)<=13)? (&PCMSK0) : (((p)<=19)? (&PCMSK1) : (((p)<=23)? (&PCMSK3) : ((uint8_t *)0)))))
#define digitalPinToPCMSKbit(p) (((p)<=7)? (p) : (((p)<=13)? ((p)-8) : (((p)<=19)? ((p)-14) : ((p)<=21)? ((p)-17) : ((p)-22))))

#define digitalPinToInterrupt(p)  ((p) == 2 ? 0 : ((p) == 3 ? 1 : NOT_AN_INTERRUPT))

#if (2 + 2 == 4)


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
	3, /* 14 */
	3,
	3,
	3,
	3,
	3,
	5, /* 20 */
	5,
	5,
	5,
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
	0, /* 14, port C */
	1,
	2,
	3,
	4,
	5,
	2, /* 20, port E, PE2 is AD6 */
	3, // PE3 is AD7
	0, // PE0
	1, // PE1
};

constexpr std::uint8_t ino_timer_oc_num[] = {
	(3'0), /* 0 - port D */
	(4'0),
	(3'1),
	(2'1),
	255,
	(0'1),
	(0'0),
	255,
	255, /* 8 - port B */
	(1'0),
	(1'1),
	(2'0),
	255,
	255,
	255, /* 14 - port C */
	255,
	255,
	255,
	255,
	255,
	255, /* 20 - port E */
	255,
	255,
	255,
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
