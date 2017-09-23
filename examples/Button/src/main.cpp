#define F_CPU 16000000UL

//#include "Arduino.h"
#include "fasthal.h"

using namespace fasthal;

FASTHAL_UART0(16, 16);

constexpr auto Uart0tw = MakeTextWriter<Uart0tx>();

typedef InvertedPin<PinD2> ButtonPin;

const auto DebouncTime = 10;
const auto LongPressTime = 500;
const auto SequenceTime = 250;

Button<Bounce<ButtonPin, DebouncTime>, LongPressTime, SequenceTime> button;

typedef PinB5 LedPin;
auto ledToggle = PinToggle<LedPin>(100, 100);

void setup()
{
  button.begin(PinMode::InputPullup);
  ledToggle.begin(true);

  // Serial.begin(9600);
  Uart0::begin(9600);
  Uart0tx::begin();
  Uart0rx::begin();  
}

void loop()
{
  ledToggle.update();

  auto evt = button.update();
  if (evt != ButtonEvent::None)
  {
    switch(evt){
      case ButtonEvent::Click:
        ledToggle.change(500, 500);
        break;
      case ButtonEvent::LongClick:
        ledToggle.change(2000, 0);
        break;
      case ButtonEvent::ClickClick:
        ledToggle.change(100, 100);
        break;
      case ButtonEvent::ClickLongClick:
        ledToggle.change(500, 2000);
        break;
      case ButtonEvent::LongClickClick:
        ledToggle.change(2000, 500);
        break;
      case ButtonEvent::LongClickLongClick:
        ledToggle.change(2000, 2000);
        break;       
      case ButtonEvent::None:
        break;
    }
    // Serial.print("Button: ");
    // Serial.println((uint8_t)evt, 2);

    Uart0tw.print("Button: ");
    Uart0tw.println((uint8_t)evt, 2);
  }
}