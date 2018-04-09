// ********************* Generated by a tool *********************
#ifndef FH_PINS_FASTDUINO_H_
#define FH_PINS_FASTDUINO_H_



// ATMEL ATMEGA1284P
// (Notice that SODAQ Mbili is equiped with VQFN44 package. The layout below is
// just a convenience to see the port names and their usage.)
//                       +---\/---+
//           (D 0) PB0  1|        |40  PA0 (AI 0 / D24)
//           (D 1) PB1  2|        |39  PA1 (AI 1 / D25)
//      INT2 (D 2) PB2  3|        |38  PA2 (AI 2 / D26)
//       PWM (D 3) PB3  4|        |37  PA3 (AI 3 / D27)
//    PWM/SS (D 4) PB4  5|        |36  PA4 (AI 4 / D28)
//      MOSI (D 5) PB5  6|        |35  PA5 (AI 5 / D29)
//  PWM/MISO (D 6) PB6  7|        |34  PA6 (AI 6 / D30)
//   PWM/SCK (D 7) PB7  8|        |33  PA7 (AI 7 / D31)
//                 RST  9|        |32  AREF
//                 VCC 10|        |31  GND
//                 GND 11|        |30  AVCC
//               XTAL2 12|        |29  PC7 (D 23)
//               XTAL1 13|        |28  PC6 (D 22)
//      RX0 (D 8)  PD0 14|        |27  PC5 (D 21) TDI
//      TX0 (D 9)  PD1 15|        |26  PC4 (D 20) TDO
// RX1/INT0 (D 10) PD2 16|        |25  PC3 (D 19) TMS
// TX1/INT1 (D 11) PD3 17|        |24  PC2 (D 18) TCK
//      PWM (D 12) PD4 18|        |23  PC1 (D 17) SDA
//      PWM (D 13) PD5 19|        |22  PC0 (D 16) SCL
//      PWM (D 14) PD6 20|        |21  PD7 (D 15) PWM
//                       +--------+

//

/*
 * Arduino digital pin numbers:
 * 0..32
 * PD0..PD7, PB0..PB7, PC0..PC7, PA0..PA7
 */

#define NUM_DIGITAL_PINS            32
#define NUM_ANALOG_INPUTS           8

#define analogInputToDigitalPin(p)  ((p < NUM_ANALOG_INPUTS) ? (p) + 24 : -1)

// TODO, Verify this
#define digitalPinHasPWM(p)         ((p) == 3 || (p) == 4 || (p) == 6 || (p) == 7 || (p) == 12 || (p) == 13 || (p) == 14 || (p) == 15)

static constexpr std::uint8_t SS   = 12;
static constexpr std::uint8_t MOSI = 13;
static constexpr std::uint8_t MISO = 14;
static constexpr std::uint8_t SCK  = 15;

static constexpr std::uint8_t SDA = 17;
static constexpr std::uint8_t SCL = 16;

static constexpr std::uint8_t GROVEPWR = 22;
static constexpr std::uint8_t GROVEPWR_OFF = LOW;
static constexpr std::uint8_t GROVEPWR_ON = HIGH;

static constexpr std::uint8_t BEEDTR = 23;
static constexpr std::uint8_t BEERTS = 9;
static constexpr std::uint8_t BEECTS = 8;

static constexpr std::uint8_t LED2 = 8;                  // Green
static constexpr std::uint8_t LED1 = 9;                  // Red

static constexpr std::uint8_t BATVOLTPIN = 30;           // A6
#define BATVOLT_R1      47                      // in fact 4.7M
#define BATVOLT_R2      100                     // in fact 10M

static constexpr std::uint8_t A0 = 24;
static constexpr std::uint8_t A1 = 25;
static constexpr std::uint8_t A2 = 26;
static constexpr std::uint8_t A3 = 27;
static constexpr std::uint8_t A4 = 28;
static constexpr std::uint8_t A5 = 29;
static constexpr std::uint8_t A6 = 30;
static constexpr std::uint8_t A7 = 31;

/*
   PCINT31-24: D7-0   : bit 3
   PCINT15-8:  D15-8  : bit 1
   PCINT23-16: D23-16 : bit 2
   PCINT7-0:   D31-24 : bit 0 (also A0..A7)
*/

#define digitalPinToPCICR(p)    (((p) >= 0 && (p) < NUM_DIGITAL_PINS) ? (&PCICR) : ((std::uint8_t *)0))
#define digitalPinToPCICRbit(p) (((p) <= 7) ? 3         : (((p) <= 15) ? 1         : (((p) <= 23) ? 2         : 0)))
#define digitalPinToPCMSK(p)    (((p) <= 7) ? (&PCMSK3) : (((p) <= 15) ? (&PCMSK1) : (((p) <= 23) ? (&PCMSK2) : (&PCMSK0))))
#define digitalPinToPCMSKbit(p) ((p) % 8)

#if (2 + 2 == 4)

// These are used as index in port_to_XYZ arrays, right?
#define PA 1
#define PB 2
#define PC 3
#define PD 4

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)






constexpr std::uint8_t ino_port_num[] =
{
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
	6,
	7,
	0, /* 24, port A */
	1,
	2,
	3,
	4,
	5,
	6,
	7
};

constexpr std::uint8_t ino_timer_oc_num[] =
{
	255, 	/* 0  - PD0 */
	255, 	/* 1  - PD1 */
	255, 	/* 2  - PD2 */
	255, 	/* 3  - PD3 */
	(1'1),     	/* 4  - PD4 */
	(1'0),     	/* 5  - PD5 */
	(2'1),     	/* 6  - PD6 */
	(2'0),     	/* 7  - PD7 */
	255, 	/* 8  - PB0 */
	255, 	/* 9  - PB1 */
	255, 	/* 10 - PB2 */
	(0'0),     	/* 11 - PB3 */
	(0'1), 	/* 12 - PB4 */
	255, 	/* 13 - PB5 */
	(3'0), 	/* 14 - PB6 */
	(3'1),	/* 15 - PB7 */
	255, 	/* 16 - PC0 */
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

#endif // ARDUINO_MAIN

#endif // FH_PINS_FASTDUINO_H_
// vim:ai:cin:sts=2 sw=2 ft=cpp