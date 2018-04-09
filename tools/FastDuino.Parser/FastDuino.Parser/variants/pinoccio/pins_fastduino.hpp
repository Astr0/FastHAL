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



#define NUM_DIGITAL_PINS            32
#define NUM_ANALOG_INPUTS           8
#define analogInputToDigitalPin(p)  ((p < 8) ? (p) + 24 : -1)
#define digitalPinHasPWM(p)         ((p) == 2 || (p) == 3 || (p) == 4 || (p) == 5 || (p) == 21 || (p) == 22 || (p) == 23)

#define RX0             0
#define TX0             1

#define D2              2
#define D3              3
#define D4              4
#define D5              5
#define D6              6
#define D7              7
#define D8              8

#define SS              9
#define MOSI            10
#define MISO            11
#define SCK             12
#define RX1             13
#define TX1             14
#define SCL             15
#define SDA             16

#define VCC_ENABLE      17
#define BATT_ALERT      18
#define BACKPACK_BUS    19
#define CHG_STATUS      20
#define LED_BLUE        21
#define LED_RED         22
#define LED_GREEN       23
#define LED_BUILTIN     23
#define LED             23

#define A0              24
#define A1              25
#define A2              26
#define A3              27
#define A4              28
#define A5              29
#define A6              30
#define A7              31

// ~: PWM, *: external interrupt
// Pin num   Pin functions     Connected to / label on board
// D0:  PE0  RXD0/PCINT8       RX0 (Connected to 16u2 USB chip)
// D1:  PE1  TXD0              TX0 (Connected to 16u2 USB chip)
// D2:  PB7  OC0A/OC1C/PCINT7  D2~
// D3:  PE3  AIN1/OC3A         D3~
// D4:  PE4  INT4/OC3B         D4*~
// D5:  PE5  INT5/OC3C         D5*~
// D6:  PE2  AIN0/XCK0         D6
// D7:  PE6  INT6/T3           D7*
// D8:  PD5  XCK1              D8
// D9:  PB0  SS/PCINT0         SS
// D10: PB2  MOSI/PDI/PCINT2   MOSI
// D11: PB3  MISO/PDO/PCINT3   MISO
// D12: PB1  SCK/PCINT1        SCK
// D13: PD2  INT2/RXD1         RX1*
// D14: PD3  INT3/TXD1         TX1*
// D15: PD0  INT0/SCL          SCL*
// D16: PD1  INT1/SDA          SDA*
// D17: PD4  ICP1              VCC_ENABLE (Controls 3V3 pin)
// D18: PE7  INT7/ICP3/CLK0    BATT_ALERT* (Connected to charge controller)
// D19: PD6  T1                BACKPACK_BUS
// D20: PD7  T0                CHG_STATUS (Connected to charge controller)
// D21: PB4  OC2A/PCINT4       LED_BLUE~ (On-board RGB led)
// D22: PB5  OC1A/PCINT5       LED_RED~ (On-board RGB led)
// D23: PB6  OC1B/PCINT6       LED_GREEN~  (On-board RGB led)
// D24: PF0  ADC0              A0
// D25: PF1  ADC1              A1
// D26: PF2  ADC2/DIG2         A2
// D27: PF3  ADC3/DIG4         A3
// D28: PF4  ADC4/TCK          A4
// D29: PF5  ADC5/TMS          A5
// D30: PF6  ADC6/TDO          A6
// D31: PF7  ADC7/TDI          A7
//
// These pins are not connected / don't have a pin
// number:
// PG5 OC0B
// PG4 TOSC1 (Connected to RTC Xtal 32.768Khz)
// PG3 TOSC2 (Connected to RTC Xtal 32.768Khz)
// PG2 AMR
// PG1 DIG1
// PG0 DIG3
//
// Interrupt mapping
//   Note that the interrupt numbers (as passed to attachInterrupt) do
//   _not_ correspond directly to the INTx values in the datasheet,
//   due to the way the Arduino core maps them (probably for backward
//   compatibility).
//
// Number  Pin          Datasheet name
// 0       D4           INT4
// 1       D5           INT5
// 2       SCL          INT0
// 3       SDA          INT1
// 4       RX1          INT2
// 5       TX1          INT3
// 6       D7           INT6
// 7       BATT_ALERT   INT7
//

#define digitalPinToPCICR(p)    (((p) >= 0 && (p) <= 24) ? (&PCICR) : ((std::uint8_t *)0))
#define digitalPinToPCICRbit(p) (((p) <= 7) ? 2 : (((p) <= 13) ? 0 : 1))
#define digitalPinToPCMSK(p)    (((p) <= 7) ? (&PCMSK2) : (((p) <= 13) ? (&PCMSK0) : (((p) <= 24) ? (&PCMSK1) : ((std::uint8_t *)0))))
#define digitalPinToPCMSKbit(p) (((p) <= 7) ? (p) : (((p) <= 13) ? ((p) - 8) : ((p) - 14)))

#if (2 + 2 == 4)







constexpr std::uint8_t ino_port_num[] = {
  /*  D0: */ 5,
  /*  D1: */ 5,
  /*  D2: */ 2,
  /*  D3: */ 5,
  /*  D4: */ 5,
  /*  D5: */ 5,
  /*  D6: */ 5,
  /*  D7: */ 5,
  /*  D8: */ 4,
  /*  D9: */ 2,
  /* D10: */ 2,
  /* D11: */ 2,
  /* D12: */ 2,
  /* D13: */ 4,
  /* D14: */ 4,
  /* D15: */ 4,
  /* D16: */ 4,
  /* D17: */ 4,
  /* D18: */ 5,
  /* D19: */ 4,
  /* D20: */ 4,
  /* D21: */ 2,
  /* D22: */ 2,
  /* D23: */ 2,
  /* D24: */ 6,
  /* D25: */ 6,
  /* D26: */ 6,
  /* D27: */ 6,
  /* D28: */ 6,
  /* D29: */ 6,
  /* D30: */ 6,
  /* D31: */ 6,
};

constexpr std::uint8_t ino_pin_num[] = {
  /*  D0: */ PE0,
  /*  D1: */ PE1,
  /*  D2: */ PB7,
  /*  D3: */ PE3,
  /*  D4: */ PE4,
  /*  D5: */ PE5,
  /*  D6: */ PE2,
  /*  D7: */ PE6,
  /*  D8: */ PD5,
  /*  D9: */ PB0,
  /* D10: */ PB2,
  /* D11: */ PB3,
  /* D12: */ PB1,
  /* D13: */ PD2,
  /* D14: */ PD3,
  /* D15: */ PD0,
  /* D16: */ PD1,
  /* D17: */ PD4,
  /* D18: */ PE7,
  /* D19: */ PD6,
  /* D20: */ PD7,
  /* D21: */ PB4,
  /* D22: */ PB5,
  /* D23: */ PB6,
  /* D24: */ PF0,
  /* D25: */ PF1,
  /* D26: */ PF2,
  /* D27: */ PF3,
  /* D28: */ PF4,
  /* D29: */ PF5,
  /* D30: */ PF6,
  /* D31: */ PF7,
};

constexpr std::uint8_t ino_timer_oc_num[] = {
  /*  D0: */ 0,
  /*  D1: */ 0,
  /*  D2: */ (0 * 3 + 0),
  /*  D3: */ (3 * 3 + 0),
  /*  D4: */ (3 * 3 + 1),
  /*  D5: */ (3 * 3 + 2),
  /*  D6: */ 0,
  /*  D7: */ 0,
  /*  D8: */ 0,
  /*  D9: */ 0,
  /* D10: */ 0,
  /* D11: */ 0,
  /* D12: */ 0,
  /* D13: */ 0,
  /* D14: */ 0,
  /* D15: */ 0,
  /* D16: */ 0,
  /* D17: */ 0,
  /* D18: */ 0,
  /* D19: */ 0,
  /* D20: */ 0,
  /* D21: */ (2 * 3 + 0),
  /* D22: */ (1 * 3 + 0),
  /* D23: */ (1 * 3 + 1),
  /* D24: */ 0,
  /* D25: */ 0,
  /* D26: */ 0,
  /* D27: */ 0,
  /* D28: */ 0,
  /* D29: */ 0,
  /* D30: */ 0,
  /* D31: */ 0,
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
