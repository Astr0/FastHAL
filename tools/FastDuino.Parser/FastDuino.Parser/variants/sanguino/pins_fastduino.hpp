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

  Changelog
  -----------
  11/25/11  - ryan@ryanmsutton.com - Add pins for Sanguino 644P and 1284P
  07/15/12  - ryan@ryanmsutton.com - Updated for arduino0101

  Improvements by Kristian Sloth Lauszus, lauszus@gmail.com
*/

#ifndef FH_PINS_FASTDUINO_H_
#define FH_PINS_FASTDUINO_H_



static constexpr uint8_t SS   = 4;
static constexpr uint8_t MOSI = 5;
static constexpr uint8_t MISO = 6;
static constexpr uint8_t SCK  = 7;

static constexpr uint8_t SDA = 17;
static constexpr uint8_t SCL = 16;

#define LED_BUILTIN 0

static constexpr uint8_t A0 = 31;
static constexpr uint8_t A1 = 30;
static constexpr uint8_t A2 = 29;
static constexpr uint8_t A3 = 28;
static constexpr uint8_t A4 = 27;
static constexpr uint8_t A5 = 26;
static constexpr uint8_t A6 = 25;
static constexpr uint8_t A7 = 24;

// ATMEL ATMEGA644/ATMEGA1284 / SANGUINO
//
//                        +---\/---+
//            (D 0) PB0  1|        |40  PA0 (AI 0 / D31)
//            (D 1) PB1  2|        |39  PA1 (AI 1 / D30)
//       INT2 (D 2) PB2  3|        |38  PA2 (AI 2 / D29)
//        PWM (D 3) PB3  4|        |37  PA3 (AI 3 / D28)
//     SS PWM (D 4) PB4  5|        |36  PA4 (AI 4 / D27)
//       MOSI (D 5) PB5  6|        |35  PA5 (AI 5 / D26)
//       MISO (D 6) PB6  7|        |34  PA6 (AI 6 / D25)
//        SCK (D 7) PB7  8|        |33  PA7 (AI 7 / D24)
//                  RST  9|        |32  AREF
//                  VCC 10|        |31  GND
//                  GND 11|        |30  AVCC
//                XTAL2 12|        |29  PC7 (D 23)
//                XTAL1 13|        |28  PC6 (D 22)
//       RX0 (D 8)  PD0 14|        |27  PC5 (D 21) TDI
//       TX0 (D 9)  PD1 15|        |26  PC4 (D 20) TDO
//  INT0 RX1 (D 10) PD2 16|        |25  PC3 (D 19) TMS
//  INT1 TX1 (D 11) PD3 17|        |24  PC2 (D 18) TCK
//       PWM (D 12) PD4 18|        |23  PC1 (D 17) SDA
//       PWM (D 13) PD5 19|        |22  PC0 (D 16) SCL
//       PWM (D 14) PD6 20|        |21  PD7 (D 15) PWM
//                        +--------+
//
#define FH_NUM_DIGITAL_PINS            24
#define FH_NUM_ANALOG_INPUTS           8

#define analogInputToDigitalPin(p)  ((p < 8) ? 31 - (p): -1)
#define analogPinToChannel(p)       ((p < 8) ? (p) : 31 - (p))

#define digitalPinHasPWM(p)         ((p) == 3 || (p) == 4 || (p) == 12 || (p) == 13 || (p) == 14 || (p) == 15 )

#define digitalPinToPCICR(p)        ( (((p) >= 0) && ((p) <= 31)) ? (&PCICR) : ((uint8_t *)0) )

#define digitalPinToPCICRbit(p)     ( (((p) >= 24) && ((p) <= 31)) ? 0 : \
                                    ( (((p) >=  0) && ((p) <=  7)) ? 1 : \
                                    ( (((p) >= 16) && ((p) <= 23)) ? 2 : \
                                    ( (((p) >=  8) && ((p) <= 15)) ? 3 : \
                                    0 ) ) ) )

#define digitalPinToPCMSK(p)        ( (((p) >= 24) && ((p) <= 31)) ? (&PCMSK0) : \
                                    ( (((p) >=  0) && ((p) <=  7)) ? (&PCMSK1) : \
                                    ( (((p) >= 16) && ((p) <= 23)) ? (&PCMSK2) : \
                                    ( (((p) >=  8) && ((p) <= 15)) ? (&PCMSK3) : \
                                    ((uint8_t *)0) ) ) ) )


#define digitalPinToPCMSKbit(p)     ( (((p) >= 24) && ((p) <= 31)) ? (31 - (p)) : \
                                    ( (((p) >=  0) && ((p) <=  7)) ? (p) : \
                                    ( (((p) >= 16) && ((p) <= 23)) ? ((p) - 16) : \
                                    ( (((p) >=  8) && ((p) <= 15)) ? ((p) - 8) : \
                                    0 ) ) ) )

#define digitalPinToInterrupt(p)    ((p) == 10 ? 0 : ((p) == 11 ? 1 : ((p) == 2 ? 2 : NOT_AN_INTERRUPT)))

#if (2 + 2 == 4)
// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)




constexpr std::uint8_t ino_port_num[] =
{
        2, /* 0 */
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        4, /* 8 */
        4,
        4,
        4,
        4,
        4,
        4,
        4,
        3, /* 16 */
        3,
        3,
        3,
        3,
        3,
        3,
        3,
        1, /* 24 */
        1,
        1,
        1,
        1,
        1,
        1,
        1  /* 31 */
};
constexpr std::uint8_t ino_pin_num[] =
{
        0, /* 0, port B */
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        0, /* 8, port D */
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
        6,
        7,
        7, /* 24, port A */
        6,
        5,
        4,
        3,
        2,
        1,
        0
};
constexpr std::uint8_t ino_timer_oc_num[] =
{
        255,   /* 0  - PB0 */
        255,   /* 1  - PB1 */
        255,   /* 2  - PB2 */
        (0'0),        /* 3  - PB3 */
        (0'1),        /* 4  - PB4 */
        255,   /* 5  - PB5 */
        255,   /* 6  - PB6 */
        255,   /* 7  - PB7 */
        255,   /* 8  - PD0 */
        255,   /* 9  - PD1 */
        255,   /* 10 - PD2 */
        255,   /* 11 - PD3 */
        (1'1),        /* 12 - PD4 */
        (1'0),        /* 13 - PD5 */
        (2'1),        /* 14 - PD6 */
        (2'0),        /* 15 - PD7 */
        255,   /* 16 - PC0 */
        255,   /* 17 - PC1 */
        255,   /* 18 - PC2 */
        255,   /* 19 - PC3 */
        255,   /* 20 - PC4 */
        255,   /* 21 - PC5 */
        255,   /* 22 - PC6 */
        255,   /* 23 - PC7 */
        255,   /* 24 - PA0 */
        255,   /* 25 - PA1 */
        255,   /* 26 - PA2 */
        255,   /* 27 - PA3 */
        255,   /* 28 - PA4 */
        255,   /* 29 - PA5 */
        255,   /* 30 - PA6 */
        255   /* 31 - PA7 */
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

#if defined(UBRR1H)
    #define SERIAL_PORT_HARDWARE1       Serial1
    #define SERIAL_PORT_HARDWARE_OPEN   Serial1
#endif

#endif
