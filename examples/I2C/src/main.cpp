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


#include "fasthal.hpp"

using namespace fasthal;
static constexpr auto i2c = i2c0;

void bh1750_set_mode(){
    // configure BH1750. TODO: Errors
    start(i2c);
    // select BH1750
    select(i2c, i2c_write, 0x23);
    // write mode command (CONTINUOUS_HIGH_RES_MODE)
    write(i2c, static_cast<std::uint8_t>(0x10));
    // stop i2c
    stop(i2c);
    // wait 
    delay_ms(120);
}

std::uint16_t bh1750_read(){
    start(i2c);
    select(i2c, i2c_read, 0x23);
    std::uint16_t result = (read(i2c) << 8) | read(i2c);
    stop(i2c);
    result /= 1.2;
    return result;
}

int main(){    
    apply(
        //Init i2c in master mode
        begin(i2c),
        //init uart
        begin(uart0)
    );

    while (1){
        bh1750_set_mode();
        auto level = bh1750_read();
        print(uart0, "Lux: ");
        println(uart0, level);
        delay_ms(1000);
    }
}