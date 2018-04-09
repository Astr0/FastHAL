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
  
  11-4-2012: Added pin definitions for PB6 and PB7 (oscillator pins)
  to map to D22 and D23.
*/

#ifndef FH_PINS_FASTDUINO_H_
#define FH_PINS_FASTDUINO_H_



#define NUM_DIGITAL_PINS            22
#define NUM_ANALOG_INPUTS           8
#define analogInputToDigitalPin(p)  ((p < 6) ? (p) + 14 : -1)

#if defined(__AVR_ATmega8__)
#define digitalPinHasPWM(p)         ((p) == 9 || (p) == 10 || (p) == 11)
#else
#define digitalPinHasPWM(p)         ((p) == 3 || (p) == 5 || (p) == 6 || (p) == 9 || (p) == 10 || (p) == 11)
#endif

static constexpr std::uint8_t SS   = 10;
static constexpr std::uint8_t MOSI = 11;
static constexpr std::uint8_t MISO = 12;
static constexpr std::uint8_t SCK  = 13;

static constexpr std::uint8_t SDA = 18;
static constexpr std::uint8_t SCL = 19;
#define LED_BUILTIN 13

static constexpr std::uint8_t A0 = 14;
static constexpr std::uint8_t A1 = 15;
static constexpr std::uint8_t A2 = 16;
static constexpr std::uint8_t A3 = 17;
static constexpr std::uint8_t A4 = 18;
static constexpr std::uint8_t A5 = 19;
static constexpr std::uint8_t A6 = 20;
static constexpr std::uint8_t A7 = 21;

#define digitalPinToPCICR(p)    (((p) >= 0 && (p) <= 21) ? (&PCICR) : ((std::uint8_t *)0))
#define digitalPinToPCICRbit(p) (((p) <= 7) ? 2 : (((p) <= 13) ? 0 : 1))
#define digitalPinToPCMSK(p)    (((p) <= 7) ? (&PCMSK2) : (((p) <= 13) ? (&PCMSK0) : (((p) <= 21) ? (&PCMSK1) : ((std::uint8_t *)0))))
#define digitalPinToPCMSKbit(p) (((p) <= 7) ? (p) : (((p) <= 13) ? ((p) - 8) : ((p) - 14)))

#define digitalPinToInterrupt(p)  ((p) == 2 ? 0 : ((p) == 3 ? 1 : NOT_AN_INTERRUPT))

#if (2 + 2 == 4)

// On the Arduino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

// ATMEL ATMEGA8 & 168 / ARDUINO
//
//                  +-\/-+
//            PC6  1|    |28  PC5 (AI 5)
//      (D 0) PD0  2|    |27  PC4 (AI 4)
//      (D 1) PD1  3|    |26  PC3 (AI 3)
//      (D 2) PD2  4|    |25  PC2 (AI 2)
// PWM+ (D 3) PD3  5|    |24  PC1 (AI 1)
//      (D 4) PD4  6|    |23  PC0 (AI 0)
//            VCC  7|    |22  GND
//            GND  8|    |21  AREF
//      (D22) PB6  9|    |20  AVCC
//      (D23) PB7 10|    |19  PB5 (D 13)
// PWM+ (D 5) PD5 11|    |18  PB4 (D 12)
// PWM+ (D 6) PD6 12|    |17  PB3 (D 11) PWM
//      (D 7) PD7 13|    |16  PB2 (D 10) PWM
//      (D 8) PB0 14|    |15  PB1 (D 9) PWM
//                  +----+
//
// (PWM+ indicates the additional PWM pins on the ATmega168.)

// ATMEL ATMEGA1280 / ARDUINO
//
// 0-7 PE0-PE7   works
// 8-13 PB0-PB5  works
// 14-21 PA0-PA7 works 
// 22-29 PH0-PH7 works
// 30-35 PG5-PG0 works
// 36-43 PC7-PC0 works
// 44-51 PJ7-PJ0 works
// 52-59 PL7-PL0 works
// 60-67 PD7-PD0 works
// A0-A7 PF0-PF7
// A8-A15 PK0-PK7


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
	3, //Add ADC6 as A6
	3, //Add ADC7 as A7
	2, //Add 26 as D22
	2, //Add 27 as D23
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
	6, //Add ADC6 as A6
	7, //Add ADC7 as A7
	6, //Add PB6 as D22
	7, //Add PB7 as D23
};

constexpr std::uint8_t ino_timer_oc_num[] = {
	255, /* 0 - port D */
	255,
	255,
	// on the ATmega168, digital pin 3 has hardware pwm
#if defined(__AVR_ATmega8__)
	255,
#else
	(2'1),
#endif
	255,
	// on the ATmega168, digital pins 5 and 6 have hardware pwm
#if defined(__AVR_ATmega8__)
	255,
	255,
#else
	(0'1),
	(0'0),
#endif
	255,
	255, /* 8 - port B */
	(1'0),
	(1'1),
#if defined(__AVR_ATmega8__)
	(2'0),
#else
	(2'0),
#endif
	255,
	255,
	255,
	255, /* 14 - port C */
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255, //Adding PB6 as D22
	255, //Adding PB7 as D23
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
