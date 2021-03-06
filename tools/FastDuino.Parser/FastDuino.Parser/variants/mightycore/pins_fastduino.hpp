// ********************* Generated by a tool *********************
#ifndef FH_PINS_FASTDUINO_H_
#define FH_PINS_FASTDUINO_H_


/*

                 MIGHTYCORE STANDARD PINOUT
         ATmega8535, ATmega16, ATmega32, ATmega164, 
              ATmega324, ATmega644, ATmega1284
   	 
                         +---\/---+
         LED (D 0) PB0  1|        |40  PA0 (A 0 / D24)
             (D 1) PB1  2|        |39  PA1 (A 1 / D25)
        INT2 (D 2) PB2  3|        |38  PA2 (A 2 / D26)
         PWM (D 3) PB3  4|        |37  PA3 (A 3 / D27)
    PWM* /SS (D 4) PB4  5|        |36  PA4 (A 4 / D28)
        MOSI (D 5) PB5  6|        |35  PA5 (A 5 / D29)
 PWM** /MISO (D 6) PB6  7|        |34  PA6 (A 6 / D30)
  PWM** /SCK (D 7) PB7  8|        |33  PA7 (A 7 / D31)
                   RST  9|        |32  AREF
                   VCC 10|        |31  GND 
                   GND 11|        |30  AVCC
                 XTAL2 12|        |29  PC7 (D 23)
                 XTAL1 13|        |28  PC6 (D 22)
        RX0 (D 8)  PD0 14|        |27  PC5 (D 21) TDI
        TX0 (D 9)  PD1 15|        |26  PC4 (D 20) TDO
 (RX1)/INT0 (D 10) PD2 16|        |25  PC3 (D 19) TMS
 (TX1)/INT1 (D 11) PD3 17|        |24  PC2 (D 18) TCK
        PWM (D 12) PD4 18|        |23  PC1 (D 17) SDA
        PWM (D 13) PD5 19|        |22  PC0 (D 16) SCL
       PWM* (D 14) PD6 20|        |21  PD7 (D 15) PWM
                         +--------+
       PWM: ATmega8535/16/32/164/324/644/1284
       PWM*: ATmega164/324/644/1284
       PWM**: ATmega1284
       PCINT ONLY ON ATmega164/324/644/1284
   
PCINT15-8: D7-0    : bit 1
PCINT31-24: D15-8  : bit 3
PCINT23-16: D23-16 : bit 2
PCINT7-0: D31-24   : bit 0
*/

#define STANDARD_PINOUT
#define FH_NUM_DIGITAL_PINS            32
#define FH_NUM_ANALOG_INPUTS           8
#define EXTERNAL_NUM_INTERRUPTS     3
#define analogInputToDigitalPin(p)  ((p < FH_NUM_ANALOG_INPUTS) ? (p) + 24 : -1)
#define analogPinToChannel(p)       ((p) < FH_NUM_ANALOG_INPUTS ? (p) : (p) >= 24 ? (p) - 24 : -1)
#define digitalPinToInterrupt(p)    ((p) == 2 ? 2 : ((p) == 10 ? 0 : ((p) == 11 ? 1 : NOT_AN_INTERRUPT)))

#if defined(__AVR_ATmega8535__) || defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
#define digitalPinHasPWM(p)         ((p) == 3 || (p) == 12 || (p) == 13 || (p) == 15)

#elif defined(__AVR_ATmega164A__) || defined(__AVR_ATmega164P__) || defined(__AVR_ATmega324A__) || \
defined(__AVR_ATmega324P__) || defined(__AVR_ATmega324PA__) || defined(__AVR_ATmega644__) || \
defined(__AVR_ATmega644P__)
#define digitalPinHasPWM(p)         ((p) == 3 || (p) == 4 || (p) == 12 || (p) == 13 || (p) == 14 || (p) == 15)

#elif defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__)
 #define digitalPinHasPWM(p)        ((p) == 3 || (p) == 4 || (p) == 6 || (p) == 7 || (p) == 12 || (p) == 13 || (p) == 14 || (p) == 15)
#endif


static constexpr uint8_t LED = 0;
#define LED_BUILTIN 0

static constexpr uint8_t SS   = 4;
static constexpr uint8_t MOSI = 5;
static constexpr uint8_t MISO = 6;
static constexpr uint8_t SCK  = 7;

static constexpr uint8_t SDA = 17;
static constexpr uint8_t SCL = 16;

static constexpr uint8_t A0 = 24;
static constexpr uint8_t A1 = 25;
static constexpr uint8_t A2 = 26;
static constexpr uint8_t A3 = 27;
static constexpr uint8_t A4 = 28;
static constexpr uint8_t A5 = 29;
static constexpr uint8_t A6 = 30;
static constexpr uint8_t A7 = 31;


#if defined(__AVR_ATmega164A__) || defined(__AVR_ATmega164P__) || defined(__AVR_ATmega324A__) || \
defined(__AVR_ATmega324P__) || defined(__AVR_ATmega324PA__) || defined(__AVR_ATmega644__) || \
defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__)
#define digitalPinToPCICR(p)    (((p) >= 0 && (p) < FH_NUM_DIGITAL_PINS) ? (&PCICR) : ((uint8_t *)0))
#define digitalPinToPCICRbit(p) ( (p) <= 7 ? 1 : (p) <= 15 ? 3 : (p) <= 23 ? 2 : 0 )
#define digitalPinToPCMSK(p)    ( (p) <= 7 ? &PCMSK1 : (p) <= 15 ? &PCMSK3 : (p) <= 23 ? &PCMSK2 : &PCMSK0 )
#define digitalPinToPCMSKbit(p) ((p) % 8)
#endif

#if defined(__AVR_ATmega164A__) || defined(__AVR_ATmega164P__) || defined(__AVR_ATmega324A__) || \
defined(__AVR_ATmega324P__) || defined(__AVR_ATmega324PA__)
/**** Needed to get the SD library to work. 
Missing definitions in the iom164.h/iom324.h file ****/
#define SPR0 0
#define SPR1 1
#define CPHA 2
#define CPOL 3
#define MSTR 4
#define DORD 5
#define SPE 6
#define SPIE 7
#define SPSR _SFR_IO8(0x2D)
#define SPI2X 0
#define WCOL 6
#define SPIF 7
#define SPCR _SFR_IO8(0x2C)
#define SPDR _SFR_IO8(0x2E)
#endif


#if (2 + 2 == 4)

#define PA 1
#define PB 2
#define PC 3
#define PD 4

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)






constexpr std::uint8_t ino_port_num[] =
{
	2, /* D0 */
	2,
	2,
	2,
	2,
	2,
	2,
	2,
	4, /* D8 */
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	3, /* D16 */
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	1, /* D24 */
	1,
	1,
	1,
	1,
	1,
	1,
	1  /* D31 */
};

constexpr std::uint8_t ino_pin_num[] =
{
	0, /* D0, port B */
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	0, /* D8, port D */
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	0, /* D16, port C */
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	0, /* D24, port A */
	1,
	2,
	3,
	4,
	5,
	6,
	7
};


#if defined(__AVR_ATmega8535__) || defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
constexpr std::uint8_t ino_timer_oc_num[] =
{
	255, 	/* D0  - PB0 */
	255, 	/* D1  - PB1 */
	255, 	/* D2  - PB2 */
	(0'0),     	/* D3  - PB3 */
	255, 	/* D4  - PB4 */
	255, 	/* D5  - PB5 */
	255, 	/* D6  - PB6 */
	255,	/* D7  - PB7 */
	255, 	/* D8  - PD0 */
	255, 	/* D9  - PD1 */
	255, 	/* D10 - PD2 */
	255, 	/* D11 - PD3 */
	(1'1),     	/* D12 - PD4 */
	(1'0),     	/* D13 - PD5 */
	255,   /* D14 - PD6 */
	(2'0),     	/* D15 - PD7 */
	255, 	/* D16 - PC0 */
	255,   /* D17 - PC1 */
	255,   /* D18 - PC2 */
	255,   /* D19 - PC3 */
	255,   /* D20 - PC4 */
	255,   /* D21 - PC5 */
	255,   /* D22 - PC6 */
	255,   /* D23 - PC7 */
	255,   /* D24 - PA0 */
	255,   /* D25 - PA1 */
	255,   /* D26 - PA2 */
	255,   /* D27 - PA3 */
	255,   /* D28 - PA4 */
	255,   /* D29 - PA5 */
	255,   /* D30 - PA6 */
	255    /* D31 - PA7 */
};

#elif defined(__AVR_ATmega164A__) || defined(__AVR_ATmega164P__) || defined(__AVR_ATmega324A__) || \
defined(__AVR_ATmega324P__) || defined(__AVR_ATmega324PA__) || defined(__AVR_ATmega644__) || \
defined(__AVR_ATmega644P__)
constexpr uint8_t digital_pin_to_timer_PGM[] =
{
	NOT_ON_TIMER, 	/* D0  - PB0 */
	NOT_ON_TIMER, 	/* D1  - PB1 */
	NOT_ON_TIMER, 	/* D2  - PB2 */
	TIMER0A,     	/* D3  - PB3 */
	TIMER0B, 	/* D4  - PB4 */
	NOT_ON_TIMER, 	/* D5  - PB5 */
	NOT_ON_TIMER, 	/* D6  - PB6 */
	NOT_ON_TIMER,	/* D7  - PB7 */
	NOT_ON_TIMER, 	/* D8  - PD0 */
	NOT_ON_TIMER, 	/* D9  - PD1 */
	NOT_ON_TIMER, 	/* D10 - PD2 */
	NOT_ON_TIMER, 	/* D11 - PD3 */
	TIMER1B,     	/* D12 - PD4 */
	TIMER1A,     	/* D13 - PD5 */
	TIMER2B,     	/* D14 - PD6 */
	TIMER2A,     	/* D15 - PD7 */
	NOT_ON_TIMER, 	/* D16 - PC0 */
	NOT_ON_TIMER,   /* D17 - PC1 */
	NOT_ON_TIMER,   /* D18 - PC2 */
	NOT_ON_TIMER,   /* D19 - PC3 */
	NOT_ON_TIMER,   /* D20 - PC4 */
	NOT_ON_TIMER,   /* D21 - PC5 */
	NOT_ON_TIMER,   /* D22 - PC6 */
	NOT_ON_TIMER,   /* D23 - PC7 */
	NOT_ON_TIMER,   /* D24 - PA0 */
	NOT_ON_TIMER,   /* D25 - PA1 */
	NOT_ON_TIMER,   /* D26 - PA2 */
	NOT_ON_TIMER,   /* D27 - PA3 */
	NOT_ON_TIMER,   /* D28 - PA4 */
	NOT_ON_TIMER,   /* D29 - PA5 */
	NOT_ON_TIMER,   /* D30 - PA6 */
	NOT_ON_TIMER    /* D31 - PA7 */
};

#elif defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__)
constexpr uint8_t digital_pin_to_timer_PGM[] =
{
	NOT_ON_TIMER, 	/* D0  - PB0 */
	NOT_ON_TIMER, 	/* D1  - PB1 */
	NOT_ON_TIMER, 	/* D2  - PB2 */
	TIMER0A,     	/* D3  - PB3 */
	TIMER0B, 	/* D4  - PB4 */
	NOT_ON_TIMER, 	/* D5  - PB5 */
	TIMER3A, 	/* D6  - PB6 */
	TIMER3B,	/* D7  - PB7 */
	NOT_ON_TIMER, 	/* D8  - PD0 */
	NOT_ON_TIMER, 	/* D9  - PD1 */
	NOT_ON_TIMER, 	/* D10 - PD2 */
	NOT_ON_TIMER, 	/* D11 - PD3 */
	TIMER1B,     	/* D12 - PD4 */
	TIMER1A,     	/* D13 - PD5 */
	TIMER2B,     	/* D14 - PD6 */
	TIMER2A,     	/* D15 - PD7 */
	NOT_ON_TIMER, 	/* D16 - PC0 */
	NOT_ON_TIMER,   /* D17 - PC1 */
	NOT_ON_TIMER,   /* D18 - PC2 */
	NOT_ON_TIMER,   /* D19 - PC3 */
	NOT_ON_TIMER,   /* D20 - PC4 */
	NOT_ON_TIMER,   /* D21 - PC5 */
	NOT_ON_TIMER,   /* D22 - PC6 */
	NOT_ON_TIMER,   /* D23 - PC7 */
	NOT_ON_TIMER,   /* D24 - PA0 */
	NOT_ON_TIMER,   /* D25 - PA1 */
	NOT_ON_TIMER,   /* D26 - PA2 */
	NOT_ON_TIMER,   /* D27 - PA3 */
	NOT_ON_TIMER,   /* D28 - PA4 */
	NOT_ON_TIMER,   /* D29 - PA5 */
	NOT_ON_TIMER,   /* D30 - PA6 */
	NOT_ON_TIMER    /* D31 - PA7 */
};

#endif // Timer defs

#endif // ARDUINO_MAIN

#endif // FH_PINS_FASTDUINO_H_

// vim:ai:cin:sts=2 sw=2 ft=cpp
