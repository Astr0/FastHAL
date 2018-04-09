// ********************* Generated by a tool *********************
//
//
// Pins definitions for Wildfire 1284P
// Original file from Maniacbug's Mighty-1284 core
// This version by Victor Aprea (22nd September 2013)
//
//

#ifndef FH_PINS_FASTDUINO_H_
#define FH_PINS_FASTDUINO_H_



#define WILDFIRE_VERSION 3
#define CORE_WILDFIRE

//                    ATMEL ATMEGA1284P on Wildfire
//                    Actual implementation with TQFP
//
//                                +---\/---+
// PCINT13/ICP3/MOSI  (D11) PB5  1|        |44  PB4 (D 9)* PCINT12/OC0B/SS
// PCINT14/OC3A/MISO *(D12) PB6  2|        |43  PB3 (D 4)* PCINT11/AIN1/OC0A
//  PCINT15/OC3B/SCK *(D13) PB7  3|        |42  PB2 (D22) PCINT10/AIN0/INT2
//                        RESET  4|        |41  PB1 (D23) PCINT9/T1/CLK0
//                          VCC  5|        |40  PB0 (D21) PCINT8/XCK0/T0
//                          GND  6|        |39  GND
//                        XTAL2  7|        |38  VCC
//                        XTAL1  8|        |37  PA0 (D24/AIN0) PCINT0/ADC0
//    PCINT24/RXD0/T3 (D 0) PD0  9|        |36  PA1 (D25/AIN1) PCINT1/ADC1
//       PCINT25/TXD0 (D 1) PD1 10|        |35  PA2 (D26/AIN2) PCINT2/ADC2
//  PCINT26/RXD1/INT0 (D 2) PD2 11|        |34  PA3 (D27/AIN3) PCINT3/ADC3
//  PCINT27/TXD1/INT1 (D 3) PD3 12|        |33  PA4 (D28/AIN4) PCINT4/ADC4
// PCINT28/XCK1/OC1B *(D 8) PD4 13|        |32  PA5 (D29/AIN5) PCINT5/ADC5
//      PCINT29/OC1A *(D 5) PD5 14|        |31  PA6 (D30/AIN6) PCINT6/ADC6
//   PCINT30/OC2B/ICP (D 6) PD6 15|        |30  PA7 (D31/AIN7) PCINT7/ADC7
//       PCINT31/OC2A (D10) PD7 16|        |29  AREF
//                          VCC 17|        |28  GND
//                          GND 18|        |27  AVCC
//        PCINT16/SCL (D20) PC0 19|        |26  PC7 (D18) PCINT23/TOSC2
//        PCINT17/SDA (D19) PC1 20|        |25  PC6 (D17) PCINT22/TOSC1
//        PCINT17/SDA (D15) PC2 21|        |24  PC5 (D16) PCINT21/TDI
//        PCINT17/SDA (D 7) PC3 22|        |23  PC4 (D14) PCINT20/TDO
//                                +--------+
//
// D 2 dedicated to RFM69 IRQ                  *** do not set to output ***
// D22 dedicated to CC3000 IRQ                 *** do not set to ouput ***
// D 6 connected to LED (buffered) 
// D14 connected to tinyWDT pet interface 
// D 7 dedicated to RFM69 Chip Select          *** mutually exclusive output low ***
// D15 dedicated to External Flash Chip Select *** mutually exclusive output low ***
// D16 dedicated to SD Card Socket Chip Select *** mutually exclusive output low ***
// D21 dedicated to CC3000 Chip Select         *** mutually exclusive output low ***
// D17 dedicated to RTC crystal
// D18 dedicated to RTC crystal
// D23 dedicated to CC3000 VBAT
//
// * = PWM capable pin
// TOSCn = RTC Crystal pinout
// TCK/TMS/TDO/TDI = JTAG pinout

/*                       
                         PCICR
   PCINT7-0:   D31-24  : bit 0   
   PCINT15-8:  D15- 8  : bit 1
   PCINT23-16: D23-16  : bit 2
   PCINT31-24: D 7- 0  : bit 3
*/

#define NUM_DIGITAL_PINS            32
#define NUM_ANALOG_INPUTS           8

#define analogInputToDigitalPin(p)  ((p < 8) ? (p) + 24 : -1)
#define digitalPinHasPWM(p)         (((p) > 3) && ((p) < 14) && ((p) != 7) && ((p) != 11))

static constexpr std::uint8_t SS   = 9;
static constexpr std::uint8_t MOSI = 11;
static constexpr std::uint8_t MISO = 12;
static constexpr std::uint8_t SCK  = 13;

static constexpr std::uint8_t SDA = 19;
static constexpr std::uint8_t SCL = 20;
static constexpr std::uint8_t LED_BUILTIN = 6;

static constexpr std::uint8_t A0 = 24;
static constexpr std::uint8_t A1 = 25;
static constexpr std::uint8_t A2 = 26;
static constexpr std::uint8_t A3 = 27;
static constexpr std::uint8_t A4 = 28;
static constexpr std::uint8_t A5 = 29;
static constexpr std::uint8_t A6 = 30;
static constexpr std::uint8_t A7 = 31;

#define digitalPinToPCICR(p)    (((p) >= 0 && (p) < NUM_DIGITAL_PINS) ? (&PCICR) : ((std::uint8_t *)0))
#define digitalPinToPCICRbit(p) ((p) > 23 ? 0 : (((p) > 13 && (p) < 21) || ((p) == 7) ? 2 : (((p) > 10) || ((p) == 9) || ((p) == 4) ? 1 : 3)))
#define digitalPinToPCMSK(p)    ((p) < 32 ? ((p) > 23 ? (&PCMSK0) : (((p) > 13 && (p) < 21) || ((p) == 7) ? (&PCMSK2) : (((p) > 10) || ((p) == 9) || ((p) == 4) ? (&PCMSK1) : (&PCMSK3)))) : ((std::uint8_t *)0))

#ifndef NOT_AN_INTERRUPT
#define NOT_AN_INTERRUPT (-1)
#endif
#define digitalPinToInterrupt(p)  ((p) == 2 ? 0 : ((p) == 3 ? 1 : ((p) == 22 ? 2 : NOT_AN_INTERRUPT)))

#ifndef ARDUINO_MAIN
extern constexpr std::uint8_t digital_pin_to_PCMSK_bit_PGM;
#endif

#define digitalPinToPCMSKbit(p) ((digital_pin_to_PCMSK_bit_PGM + p))

#ifdef ARDUINO_MAIN
#define PA 1
#define PB 2
#define PC 3
#define PD 4

// this was just too complicated to express as a formula, and we have plenty of flash memory 
constexpr std::uint8_t digital_pin_to_PCMSK_bit_PGM[] = {
  0,
  1,
  2,
  3,
  3,
  5,
  6,
  3,
  4,
  4,
  7,
  5,
  6,
  7,
  4,
  2,
  5,
  6,
  7,
  1,
  0,
  0,
  2,
  1,
  0,
  1,
  2,
  3,
  4,
  5,
  6,
  7
};

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)






constexpr std::uint8_t ino_port_num[] =
{
  4, 
  4,
  4,
  4,
  2,
  4,
  4,
  3,
  
  4, 
  2,
  4,
  2,
  2,
  2,
  3,
  3,
  
  3,  
  3,
  3,
  3,
  3,
  2,
  2,
  2,
  
  1, 
  1,
  1,
  1,
  1,
  1,
  1,
  1
};


constexpr std::uint8_t ino_pin_num[] =
{
  0, // PD
  1, // PD
  2, // PD
  3, // PD
  3, // PB
  5, // PD
  6, // PD
  3, // PC
  4, // PD
  4, // PB
  7, // PD
  5, // PB
  6, // PB
  7, // PB
  4, // PC
  2, // PC
  5, // PC
  6, // PC
  7, // PC
  1, // PC
  0, // PC
  0, // PB
  2, // PB
  1, // PB
  0, // PA
  1, // PA
  2, // PA
  3, // PA
  4, // PA
  5, // PA
  6, // PA
  7  // PA
};

constexpr std::uint8_t ino_timer_oc_num[] =
{
  0, // PD0
  0, // PD1
  0, // PD2
  0, // PD3
  (0 * 3 + 0),      // PB3
  (1 * 3 + 0),      // PD5
  (2 * 3 + 1),      // PD6
  0, // PC3

  (1 * 3 + 1),      // PD4
  (0 * 3 + 1),      // PB4
  (2 * 3 + 0),      // PD7
  0, // PB5
  (3 * 3 + 0),      // PB6
  (3 * 3 + 1),      // PB7
  0, // PC4
  0, // PC2

  0, // PC5
  0, // PC6
  0, // PC7
  0, // PC1
  0, // PC0
  0, // PB0
  0, // PB2
  0, // PB1

  0, // PA0
  0, // PA1
  0, // PA2
  0, // PA3
  0, // PA4
  0, // PA5
  0, // PA6
  0, // PA7
};

#endif // ARDUINO_MAIN

#endif // FH_PINS_FASTDUINO_H_
// vim:ai:cin:sts=2 sw=2 ft=cpp
