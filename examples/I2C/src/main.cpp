// #include <Arduino.h>
// /*

//   Example of BH1750 library usage.

//   This example initialises the BH1750 object using the default high resolution
//   continuous mode and then makes a light level reading every second.

//   Connection:

//     VCC -> 3V3 or 5V
//     GND -> GND
//     SCL -> SCL (A5 on Arduino Uno, Leonardo, etc or 21 on Mega and Due, on esp8266 free selectable)
//     SDA -> SDA (A4 on Arduino Uno, Leonardo, etc or 20 on Mega and Due, on esp8266 free selectable)
//     ADD -> (not connected) or GND

//   ADD pin is used to set sensor I2C address. If it has voltage greater or equal to
//   0.7VCC voltage (e.g. you've connected it to VCC) the sensor address will be
//   0x5C. In other case (if ADD voltage less than 0.7 * VCC) the sensor address will
//   be 0x23 (by default).

// */

// #include <Wire.h>
// #include <BH1750.h>
// #include "fasthal.hpp"

// BH1750 lightMeter;


// void setup(){

//   Serial.begin(9600);

//   // Initialize the I2C bus (BH1750 library doesn't do this automatically)
//   Wire.begin();
//   // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);

//   lightMeter.begin();

//   Serial.println(F("BH1750 Test begin"));

// }


// void loop() {

//   uint16_t lux = lightMeter.readLightLevel();
//   Serial.print("Light: ");
//   Serial.print(lux);
//   Serial.println(" lx");
//   delay(1000);

// }


#include <avr/io.h>
#include "fastduino.hpp"

using namespace fasthal;
using namespace fasthal::duino;

static constexpr auto i2c0 = i2c<0>{};
static constexpr auto uart0 = uart<0>{};

// void debugi2c(const char* why){
//     print(uart0tx, why);
//     print(uart0tx, ' ');
//     print(uart0tx, 'x');
//     println(uart0tx, static_cast<std::uint8_t>(read_(i2c.status)), numberbase_hex);
// }

static constexpr auto address = i2c_address_v<0x23>;

bool bh1750_set_mode(std::uint8_t mode){
    // select BH1750 write
    if (!i2c0.mt_start(address))
        return false;
    // write mode command
    write(i2c0.tx, mode);
    if (!i2c0.stop())
        return false;
    // wait 
    delay_ms(120);
    return true;
}

std::uint16_t bh1750_read(std::uint8_t mode){
    if (!bh1750_set_mode(mode))
        return 0;
    if (!i2c0.mr_start(address, 2))
        return 0;
    auto result = std::uint16_t{ read(i2c0.rx) };
    result = (result << 8) | read(i2c0.rx);    
    if (!i2c0.stop())
        return 0;
    return (result * 10) / 12;
}

int main(){    
    apply(
        // activate internal pull ups for i2c
        set(ino<SDA>),
        set(ino<SCL>),
        //Init i2c in master mode
        i2c0.begin(),
        //init uart
        uart0.begin()
    );

    // wake up?
    bh1750_set_mode(0x0);

    println(uart0.tx, "BH1750 Test begin");

    while (1){
        //bh1750_set_mode(std::uint8_t{0x10});
        //delay_ms(180);
        auto light = bh1750_read(0x10);
        print(uart0.tx, "Lux: ");
        print(uart0.tx, light);
        println(uart0.tx, " lx");
        delay_ms(1000);
    }
}