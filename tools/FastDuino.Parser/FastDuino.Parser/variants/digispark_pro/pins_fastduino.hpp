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

#define ATTINYX7 1

#ifndef __AVR_ATtiny167__
#define __AVR_ATtiny167__
#endif

#define SERIAL_BUFFER_SIZE 16



#define FH_NUM_DIGITAL_PINS            14
#define FH_NUM_ANALOG_INPUTS           10
#define analogInputToDigitalPin(p)  ((p == 3) ? 23 : (p == 5) ? 21 : (p < 13 && p > 5) ? p+14 : (p ==13) ? 24 : -1)

#define digitalPinHasPWM(p)         ((p) == 0 || (p) == 1)

#define SS   12
#define MOSI 10
#define MISO 8
#define SCK  11

static constexpr uint8_t SDA = 0;
static constexpr uint8_t SCL = 2;

//Ax constexprants cannot be used for digitalRead/digitalWrite/analogWrite functions, only analogRead().
static constexpr uint8_t A3 = FH_NUM_DIGITAL_PINS+9;
static constexpr uint8_t A5 = FH_NUM_DIGITAL_PINS+7;
static constexpr uint8_t A6 = FH_NUM_DIGITAL_PINS+0;
static constexpr uint8_t A7 = FH_NUM_DIGITAL_PINS+1;
static constexpr uint8_t A8 = FH_NUM_DIGITAL_PINS+2;
static constexpr uint8_t A9 = FH_NUM_DIGITAL_PINS+3;
static constexpr uint8_t A10 = FH_NUM_DIGITAL_PINS+4;
static constexpr uint8_t A11 = FH_NUM_DIGITAL_PINS+5;
static constexpr uint8_t A12 = FH_NUM_DIGITAL_PINS+6;
static constexpr uint8_t A13 = FH_NUM_DIGITAL_PINS+10;



//----------------------------------------------------------
//----------------------------------------------------------
//Core Configuration (used to be in core_build_options.h)

//If Software Serial communications doesn't work, run the TinyTuner sketch provided with the core to give you a calibrated OSCCAL value.
//Change the value here with the tuned value. By default this option uses the default value which the compiler will optimise out. 
#define TUNED_OSCCAL_VALUE                        OSCCAL
//e.g
//#define TUNED_OSCCAL_VALUE                        0x57

//Choosing not to initialise saves power and flash. 1 = initialise.
#define INITIALIZE_ANALOG_TO_DIGITAL_CONVERTER    1
#define INITIALIZE_SECONDARY_TIMERS               0

#define TIMER_TO_USE_FOR_MILLIS                   0

#define HAVE_BOOTLOADER                           1

/*
  Where to put the software serial? (Arduino Digital pin numbers)
*/
//WARNING, if using software, TX is on AIN0, RX is on AIN1. Comparator is favoured to use its interrupt for the RX pin.
#define USE_SOFTWARE_SERIAL						  0
//Please define the port on which the analog comparator is found.
#define ANALOG_COMP_DDR						 	  DDRA
#define ANALOG_COMP_PORT						  PORTA
#define ANALOG_COMP_PIN						 	  PINA
#define ANALOG_COMP_AIN0_BIT					  6
#define ANALOG_COMP_AIN1_BIT					  7

/*
  Analog reference bit masks.
*/
// VCC used as analog reference, disconnected from PA0 (AREF)
#define DEFAULT (0)
// External voltage reference at PA0 (AREF) pin, internal reference turned off
#define EXTERNAL (1)
// Internal 1.1V voltage reference
#define INTERNAL (2)


//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------



#define digitalPinToPCICR(p)    (&PCICR)
#define digitalPinToPCICRbit(p) ( ((p) >= 5 && (p) <= 12) ? PCIE0 : PCIE1 )
#define digitalPinToPCMSK(p)    ( ((p) >= 5 && (p) <= 12) ? (&PCMSK0) : (&PCMSK1) )
#define digitalPinToPCMSKbit(p) ( (((p) >= 0) && ((p) <= 2))  ? (p) :       \
                                ( (((p) >= 6) && ((p) <= 13)) ? ((p) - 6) : \
                                ( ((p) == 3) ? 6 :                          \
                                ( ((p) == 4) ? 3 :                          \
                                ( 7) ) ) ) ) /* pin 5 */

#if (2 + 2 == 4)

// On the Arduino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

// ATMEL ATTINY167
//
//                   +-\/-+
// RX   (D  0) PA0  1|    |20  PB0 (D  4)
// TX   (D  1) PA1  2|    |19  PB1 (D  5)
//     *(D 12) PA2  3|    |18  PB2 (D  6)
//      (D  3) PA3  4|    |17  PB3 (D  7)*
//            AVCC  5|    |16  GND
//            AGND  6|    |15  VCC
// INT1 (D 11) PA4  7|    |14  PB4 (D  8)
//      (D 13) PA5  8|    |13  PB5 (D  9)
//      (D 10) PA6  9|    |12  PB6 (D  2)* INT0
//      (D 14) PA7 10|    |11  PB7 (D 15)
//                   +----+
//
// * indicates PWM pin.

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)






constexpr std::uint8_t ino_port_num[] = 
{
	2, /* 0 */
	2,
	2, /* 2 */
	2, /* 3 */
	2, /* 4 */
	1,
	1,
	1,
	1,
	1,
	1, /* 10 */
	1,
	1,
	2, /* RESET */
};

constexpr std::uint8_t ino_pin_num[] = 
{
	0, /* 0 */
	1,
	2, /* 2 */
	6, /* 3 */
	3, /* 4 */
	7,
	0,
	1,
	2,
	3,
	4, /* 10 */
	5,
	6,
	7,
};

constexpr std::uint8_t ino_timer_oc_num[] = 
{
	(1'0), 
	(1'1),
	(1'0),
	(1'0),
	(1'1),
	255,
	255,
	255,
	(0'0),
	255,
	255,
	255,
	255,
	255,
	255,
	255,
};

#endif

#endif




//Old code, just here for temporary backup until I decide it is not needed.
//WARNING, if using software, RX must be on a pin which has a Pin change interrupt <= 7 (e.g. PCINT6, or PCINT1, but not PCINT8)
/*#define USE_SOFTWARE_SERIAL						  1
//These are set to match Optiboot pins.

#define SOFTWARE_SERIAL_PORT 					  PORTB
#define SOFTWARE_SERIAL_TX 						  0
#define SOFTWARE_SERIAL_PIN 					  PINB
#define SOFTWARE_SERIAL_RX 						  1*/
