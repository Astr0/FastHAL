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



// Workaround for wrong definitions in "iom32u4.h".
// This should be fixed in the AVR toolchain.
#undef UHCON
#undef UHINT
#undef UHIEN
#undef UHADDR
#undef UHFNUM
#undef UHFNUML
#undef UHFNUMH
#undef UHFLEN
#undef UPINRQX
#undef UPINTX
#undef UPNUM
#undef UPRST
#undef UPCONX
#undef UPCFG0X
#undef UPCFG1X
#undef UPSTAX
#undef UPCFG2X
#undef UPIENX
#undef UPDATX
#undef TCCR2A
#undef WGM20
#undef WGM21
#undef COM2B0
#undef COM2B1
#undef COM2A0
#undef COM2A1
#undef TCCR2B
#undef CS20
#undef CS21
#undef CS22
#undef WGM22
#undef FOC2B
#undef FOC2A
#undef TCNT2
#undef TCNT2_0
#undef TCNT2_1
#undef TCNT2_2
#undef TCNT2_3
#undef TCNT2_4
#undef TCNT2_5
#undef TCNT2_6
#undef TCNT2_7
#undef OCR2A
#undef OCR2_0
#undef OCR2_1
#undef OCR2_2
#undef OCR2_3
#undef OCR2_4
#undef OCR2_5
#undef OCR2_6
#undef OCR2_7
#undef OCR2B
#undef OCR2_0
#undef OCR2_1
#undef OCR2_2
#undef OCR2_3
#undef OCR2_4
#undef OCR2_5
#undef OCR2_6
#undef OCR2_7

#define NUM_DIGITAL_PINS   31
#define NUM_ANALOG_INPUTS  12

#define TXLED0			0
#define TXLED1			0
#define RXLED0			0
#define RXLED1			0
#define TX_RX_LED_INIT	0

static constexpr std::uint8_t SDA = 2;
static constexpr std::uint8_t SCL = 3;
#define LED_BUILTIN 13

// Map SPI port to 'new' pins D14..D17
static constexpr std::uint8_t SS   = 17;
static constexpr std::uint8_t MOSI = 16;
static constexpr std::uint8_t MISO = 14;
static constexpr std::uint8_t SCK  = 15;

// Mapping of analog pins as digital I/O
// A6-A11 share with digital pins
static constexpr std::uint8_t A0 = 18;
static constexpr std::uint8_t A1 = 19;
static constexpr std::uint8_t A2 = 20;
static constexpr std::uint8_t A3 = 21;
static constexpr std::uint8_t A4 = 22;
static constexpr std::uint8_t A5 = 23;
static constexpr std::uint8_t A6 = 24;	// D4
static constexpr std::uint8_t A7 = 25;	// D6
static constexpr std::uint8_t A8 = 26;	// D8
static constexpr std::uint8_t A9 = 27;	// D9
static constexpr std::uint8_t A10 = 28;	// D10
static constexpr std::uint8_t A11 = 29;	// D12

#define digitalPinToPCICR(p)    ((((p) >= 8 && (p) <= 11) || ((p) >= 14 && (p) <= 17) || ((p) >= A8 && (p) <= A10)) ? (&PCICR) : ((std::uint8_t *)0))
#define digitalPinToPCICRbit(p) 0
#define digitalPinToPCMSK(p)    ((((p) >= 8 && (p) <= 11) || ((p) >= 14 && (p) <= 17) || ((p) >= A8 && (p) <= A10)) ? (&PCMSK0) : ((std::uint8_t *)0))
#define digitalPinToPCMSKbit(p) ( ((p) >= 8 && (p) <= 11) ? (p) - 4 : ((p) == 14 ? 3 : ((p) == 15 ? 1 : ((p) == 16 ? 2 : ((p) == 17 ? 0 : (p - A8 + 4))))))

//	__AVR_ATmega32U4__ has an unusual mapping of pins to channels

#define analogPinToChannel(P)  ( ( ino_mux[(P)] ) )

#define digitalPinHasPWM(p) ((p) == 3 || (p) == 5 || (p) == 6 || (p) == 9 || (p) == 10 || (p) == 11 || (p) == 13)
#define digitalPinToInterrupt(p) ((p) == 0 ? 2 : ((p) == 1 ? 3 : ((p) == 2 ? 1 : ((p) == 3 ? 0 : ((p) == 7 ? 4 : NOT_AN_INTERRUPT)))))

#if (2 + 2 == 4)

// On the Arduino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

// ATMEL ATMEGA32U4 / ARDUINO LEONARDO / Flora
//
// D0				PD2					RXD1/INT2
// D1				PD3					TXD1/INT3
// D2				PD1		SDA			SDA/INT1
// D3#				PD0		PWM8/SCL	OC0B/SCL/INT0
// D4		A6		PD4					ADC8
// D5#				PC6		???			OC3A/#OC4A
// D6#		A7		PD7		FastPWM		#OC4D/ADC10
// D7				PE6					INT6/AIN0
//
// D8		A8		PB4					ADC11/PCINT4
// D9#		A9		PB5		PWM16		OC1A/#OC4B/ADC12/PCINT5
// D10#		A10		PB6		PWM16		OC1B/0c4B/ADC13/PCINT6
// D11#				PB7		PWM8/16		0C0A/OC1C/#RTS/PCINT7
// D12		A11		PD6					T1/#OC4D/ADC9
// D13#				PC7		PWM10		CLK0/OC4A
//
// A0		D18		PF7					ADC7
// A1		D19		PF6					ADC6
// A2		D20 	PF5					ADC5
// A3		D21 	PF4					ADC4
// A4		D22		PF1					ADC1
// A5		D23 	PF0					ADC0
//
// New pins D14..D17 to map SPI port to digital pins
//
// MISO		D14		PB3					MISO,PCINT3
// SCK		D15		PB1					SCK,PCINT1
// MOSI		D16		PB2					MOSI,PCINT2
// SS		D17		PB0					RXLED,SS/PCINT0
//
// TXLED			PD5
// RXLED		    PB0
// HWB				PE2					HWB

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)






constexpr std::uint8_t ino_port_num[30] = {
	4, // D0 - 42
	4,	// D1 - 43
	4, // D2 - 41
	4,	// D3 - 40
	4,	// D4 - 44
	3, // D5 - 36
	4, // D6 - 47
	5, // D7 - 56

	2, // D8 - 24
	2,	// D9 - 25
	2, // D10 - 26
	2,	// D11 - 27
	4, // D12 - 46
	3, // D13 - 37

	2,	// D14 - MISO - 23
	2,	// D15 - SCK - 21
	2,	// D16 - MOSI - 22
	2,	// D17 - SS - 20

	6,	// D18 - A0 - 67
	6, // D19 - A1 - 66
	6, // D20 - A2 - 65
	6, // D21 - A3 - 64
	6, // D22 - A4 - 61
	6, // D23 - A5 - 60

	4, // D24 / D4 - A6 - 44
	4, // D25 / D6 - A7 - 47
	2, // D26 / D8 - A8 - 24
	2, // D27 / D9 - A9 - 25
	2, // D28 / D10 - A10 - 26
	4, // D29 / D12 - A11 - 46
};

constexpr std::uint8_t ino_pin_num[30] = {
	2, // D0 - PD2
	3,	// D1 - PD3
	1, // D2 - PD1
	0,	// D3 - PD0
	4,	// D4 - PD4
	6, // D5 - PC6
	7, // D6 - PD7
	6, // D7 - PE6

	4, // D8 - PB4
	5,	// D9 - PB5
	6, // D10 - PB6
	7,	// D11 - PB7
	6, // D12 - PD6
	7, // D13 - PC7

	3,	// D14 - MISO - PB3
	1,	// D15 - SCK - PB1
	2,	// D16 - MOSI - PB2
	0,	// D17 - SS - PB0

	7,	// D18 - A0 - PF7
	6, // D19 - A1 - PF6
	5, // D20 - A2 - PF5
	4, // D21 - A3 - PF4
	1, // D22 - A4 - PF1
	0, // D23 - A5 - PF0

	4, // D24 / D4 - A6 - PD4
	7, // D25 / D6 - A7 - PD7
	4, // D26 / D8 - A8 - PB4
	5, // D27 / D9 - A9 - PB5
	6, // D28 / D10 - A10 - PB6
	6, // D29 / D12 - A11 - PD6
};

constexpr std::uint8_t ino_timer_oc_num[16] = {
	255,
	255,
	255,
	(0'1),		/* 3 */
	255,
	(3'0),		/* 5 */
	(4'0)D,		/* 6 */
	255,

	255,
	(1'0),		/* 9 */
	(1'1),		/* 10 */
	(0'0),		/* 11 */

	255,
	(4'0),		/* 13 */

	255,
	255,
};

constexpr std::uint8_t ino_mux[12] = {
	7,	// A0				PF7					ADC7
	6,	// A1				PF6					ADC6
	5,	// A2				PF5					ADC5
	4,	// A3				PF4					ADC4
	1,	// A4				PF1					ADC1
	0,	// A5				PF0					ADC0
	8,	// A6		D4		PD4					ADC8
	10,	// A7		D6		PD7					ADC10
	11,	// A8		D8		PB4					ADC11
	12,	// A9		D9		PB5					ADC12
	13,	// A10		D10		PB6					ADC13
	9	// A11		D12		PD6					ADC9
};

#endif /* ARDUINO_MAIN */


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
#define SERIAL_PORT_MONITOR        Serial
#define SERIAL_PORT_USBVIRTUAL     Serial
#define SERIAL_PORT_HARDWARE       Serial1
#define SERIAL_PORT_HARDWARE_OPEN  Serial1

// Alias SerialUSB to Serial
#define SerialUSB SERIAL_PORT_USBVIRTUAL

// Bootloader related fields
// Old Caterina bootloader places the MAGIC key into unsafe RAM locations (it can be rewritten
// by the running sketch before to actual reboot).
// Newer bootloaders, recognizable by the LUFA "signature" at the end of the flash, can handle both
// the usafe and the safe location. Check once (in USBCore.cpp) if the bootloader in new, then set the global
// _updatedLUFAbootloader variable to true/false and place the magic key consequently
#ifndef MAGIC_KEY
#define MAGIC_KEY 0x7777
#endif

#ifndef MAGIC_KEY_POS
#define MAGIC_KEY_POS 0x0800
#endif

#ifndef NEW_LUFA_SIGNATURE
#define NEW_LUFA_SIGNATURE 0xDCFB
#endif

#endif /* FH_PINS_FASTDUINO_H_ */
