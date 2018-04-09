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



#define NUM_DIGITAL_PINS            35
#define NUM_ANALOG_INPUTS           8
#define analogPinToChannel(P)  (  (pin >= 20) ? pin - 20 : pin )
#define analogInputToDigitalPin(p)  ((p < 8) ? (p) + 20 : -1)
#define digitalPinHasPWM(p)         ((p) == 9 || (p) == 10 || (p) == 11 || (p) == 12 || (p) == 13 || (p) == 14 || (p) == 15 || (p) == 19)

#define SS              8
#define SCK             28
#define MOSI            29
#define MISO            30

#define SCL             0
#define SDA             1

#define RX1             2
#define TX1             3

#define RX0             34
#define TX0             35

#define LED_RED         13
#define LED_GREEN       14
#define LED_BLUE        15
#define LED_BUILTIN     13

#define A0              20
#define A1              21
#define A2              22
#define A3              23
#define A4              24
#define A5              25
#define A6              26
#define A7              27

#define digitalPinToPCICR(p)    (((p) >=  8 && (p) <= 12) || \
                                 ((p) >= 28 && (p) <= 30) ? (&PCICR) : ((std::uint8_t *)0))

// Pins PCINT7:0 are mapped to PCIE0 interrupt on bit 0 of PCICR, only PCINT8 is mapped to PCIE1.
#define digitalPinToPCICRbit(p) (((p) >=  8 && (p) <= 12) || \
                                 ((p) >= 28 && (p) <= 30) ? 0 : 1)

#define digitalPinToPCMSK(p)    (((p) >=  8 && (p) <= 12) || \
                                 ((p) >= 28 && (p) <= 30) ? (&PCMSK0) : (&PCMSK1) )

#define digitalPinToPCMSKbit(p) (((p) == 8 ) ? 0 : \
                                 (((p) >= 9 && (p) <= 12) ? ((p) - 5) : (0)))

#define digitalPinToInterrupt(p)  ( ((p) >= 0 && (p) <= 3) ? ((p) + 2) : ( ((p) == 14 || (p) == 15) ? ((p) - 14) : ( ((p) == 16 || (p) == 17) ? ((p) - 10) : NOT_AN_INTERRUPT ) ) )

#if (2 + 2 == 4)







constexpr std::uint8_t ino_port_num[] = {
  // PORTLIST
  // ~: PWM, *: external interrupt
  // -------------------------------------------
  4  , // 4 0 ** D0  ** I2C_SCL*
  4  , // 4 1 ** D1  ** I2C_SDA*
  4  , // 4 2 ** D2  ** RX1*
  4  , // 4 3 ** D3  ** TX1*
  4  , // 4 4 ** D4  ** D4
  4  , // 4 5 ** D5  ** D5
  4  , // 4 6 ** D6  ** D6
  4  , // 4 7 ** D7  ** D7
  2  , // 2 0 ** D8  ** D8*
  2  , // 2 4 ** D9  ** D9~*
  2  , // 2 5 ** D10 ** D10~*
  2  , // 2 6 ** D11 ** D11~*
  2  , // 2 7 ** D12 ** D12~*
  5  , // 5 3 ** D13 ** LED_RED~
  5  , // 5 4 ** D14 ** LED_GREEN~*
  5  , // 5 5 ** D15 ** LED_BLUE~*
  5  , // 5 6 ** D16 ** D16*
  5  , // 5 7 ** D17 ** D17*
  5  , // 5 2 ** D18 ** D18
  7  , // 7 5 ** D19 ** D19~
  6  , // 6 0 ** D20 ** A0
  6  , // 6 1 ** D21 ** A1
  6  , // 6 2 ** D22 ** A2
  6  , // 6 3 ** D23 ** A3
  6  , // 6 4 ** D24 ** A4
  6  , // 6 5 ** D25 ** A5
  6  , // 6 6 ** D26 ** A6
  6  , // 6 7 ** D27 ** A7
  2  , // 2 1 ** D28 ** SCK*
  2  , // 2 2 ** D29 ** MOSI*
  2  , // 2 3 ** D30 ** MISO*
  7  , // 7 0 ** D31 ** D31
  7  , // 7 1 ** D32 ** D33
  7  , // 7 2 ** D33 ** D33
  5  , // 5 0 ** D34 ** RX0
  5  , // 5 1 ** D35 ** TX0
};

constexpr std::uint8_t ino_pin_num[] = {
  // PIN IN PORT
  // ~: PWM, *: external interrupt
  // -------------------------------------------
  PD0  , // PD 0 ** D0  ** I2C_SCL*
  PD1  , // PD 1 ** D1  ** I2C_SDA*
  PD2  , // PD 2 ** D2  ** RX1*
  PD3  , // PD 3 ** D3  ** TX1*
  PD4  , // PD 4 ** D4  ** D4
  PD5  , // PD 5 ** D5  ** D5
  PD6  , // PD 6 ** D6  ** D6
  PD7  , // PD 7 ** D7  ** D7
  PB0  , // PB 0 ** D8  ** D8*
  PB4  , // PB 4 ** D9  ** D9~*
  PB5  , // PB 5 ** D10 ** D10~*
  PB6  , // PB 6 ** D11 ** D11~*
  PB7  , // PB 7 ** D12 ** D12~*
  PE3  , // PE 3 ** D13 ** LED_RED~
  PE4  , // PE 4 ** D14 ** LED_GREEN~*
  PE5  , // PE 5 ** D15 ** LED_BLUE~*
  PE6  , // PE 6 ** D16 ** D16*
  PE7  , // PE 7 ** D17 ** D17*
  PE2  , // PE 2 ** D18 ** D18
  PG5  , // PG 5 ** D19 ** D19~
  PF0  , // PF 0 ** D20 ** A0
  PF1  , // PF 1 ** D21 ** A1
  PF2  , // PF 2 ** D22 ** A2
  PF3  , // PF 3 ** D23 ** A3
  PF4  , // PF 4 ** D24 ** A4
  PF5  , // PF 5 ** D25 ** A5
  PF6  , // PF 6 ** D26 ** A6
  PF7  , // PF 7 ** D27 ** A7
  PB1  , // PB 1 ** D28 ** SCK*
  PB2  , // PB 2 ** D29 ** MOSI*
  PB3  , // PB 3 ** D30 ** MISO*
  PG0  , // PG 0 ** D31 ** D31
  PG1  , // PG 1 ** D32 ** D33
  PG2  , // PG 2 ** D33 ** D33
  PE0  , // PE 0 ** D34 ** RX0
  PE1  , // PE 1 ** D35 ** TX0
};

constexpr std::uint8_t ino_timer_oc_num[] = {
  // TIMERS
  // ~: PWM, *: external interrupt
  // -------------------------------------------
  255  , // PD 0 ** D0  ** I2C_SCL*
  255  , // PD 1 ** D1  ** I2C_SDA*
  255  , // PD 2 ** D2  ** RX1*
  255  , // PD 3 ** D3  ** TX1*
  255  , // PD 4 ** D4  ** D4
  255  , // PD 5 ** D5  ** D5
  255  , // PD 6 ** D6  ** D6
  255  , // PD 7 ** D7  ** D7
  255  , // PB 0 ** D8  ** D8*
  (2'0)       , // PB 4 ** D9  ** D9~*
  (1'0)       , // PB 5 ** D10 ** D10~*
  (1'1)       , // PB 6 ** D11 ** D11~*
  (0'0)       , // PB 7 ** D12 ** D12~*
  (3'0)       , // PE 3 ** D13 ** LED_RED~
  (3'1)       , // PE 4 ** D14 ** LED_GREEN~*
  (3'2)       , // PE 5 ** D15 ** LED_BLUE~*
  255  , // PE 6 ** D16 ** D16*
  255  , // PE 7 ** D17 ** D17*
  255  , // PE 2 ** D18 ** D18
  (0'1)       , // PG 5 ** D19 ** D19~
  255  , // PF 0 ** D20 ** A0
  255  , // PF 1 ** D21 ** A1
  255  , // PF 2 ** D22 ** A2
  255  , // PF 3 ** D23 ** A3
  255  , // PF 4 ** D24 ** A4
  255  , // PF 5 ** D25 ** A5
  255  , // PF 6 ** D26 ** A6
  255  , // PF 7 ** D27 ** A7
  255  , // PB 1 ** D28 ** SCK*
  255  , // PB 2 ** D29 ** MOSI*
  255  , // PB 3 ** D30 ** MISO*
  255  , // PG 0 ** D31 ** D31
  255  , // PG 1 ** D32 ** D33
  255  , // PG 2 ** D33 ** D33
  255  , // PE 0 ** D34 ** RX0
  255  , // PE 1 ** D35 ** TX0
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
#define SERIAL_PORT_HARDWARE_OPEN   Serial1

#endif