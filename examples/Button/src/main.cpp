/**
* Blink
*
* Turns on an LED on for one second,
* then off for one second, repeatedly.
*/
#include "Arduino.h"
#include "fasthal.h"

using namespace fasthal;

FASTHAL_TIME_ARDUINO();

FASTHAL_UART0(16, 16);
constexpr auto Uart0tw = MakeTextWriterS<Uart0tx>();

typedef InvertedPin<arduino::PinD2> ButtonPin;

const auto DebouncTime = 10;
const auto LongPressTime = 500;
const auto SequenceTime = 250;

Button<Bounce<ButtonPin, DebouncTime>, LongPressTime, SequenceTime> button;

void setup()
{
  button.begin(PinMode::InputPullup);

  // Serial.begin(9600);
  Uart0::begin(9600);
  Uart0tx::begin();
  Uart0rx::begin();
}

void loop()
{
  auto evt = button.update();
  if (evt != ButtonEvent::None)
  {
    // Serial.print("Button: ");
    // Serial.println((uint8_t)evt, 2);

    Uart0tw.print("Button: ");
    Uart0tw.println((uint8_t)evt, 2);
  }
}