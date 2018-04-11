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

static constexpr auto i2c = i2c0;

// void debugi2c(const char* why){
//     // print(uart0, why);
//     // print(uart0, ' ');
//     // print(uart0, 'x');
//     // println(uart0, static_cast<std::uint8_t>(read_(i2c.status)), numberbase_hex);
// }

static constexpr auto address = i2c_address_v<0x23>;

template<typename TMode>
void bh1750_set_mode(TMode mode){
    // configure BH1750. TODO: Errors
    start(i2c);
    //debugi2c("start_w");
    // select BH1750
    select(i2c, i2c_write, address);
    //debugi2c("sla+w");
    // write mode command (CONTINUOUS_HIGH_RES_MODE)
    write(i2c, mode);
    //debugi2c("write");
    // stop i2c
    stop(i2c);
    //debugi2c("stop");
    // wait 
    delay_ms(120);
}

std::uint16_t bh1750_read(){
    start(i2c);
    //debugi2c("start_r");
    select(i2c, i2c_read, address);
    //debugi2c("sla+r");
    std::uint16_t result = read(i2c) << 8;
    //debugi2c("read");
    result |= read(i2c, i2c_last);
    //debugi2c("readl");
    stop(i2c);
    //debugi2c("stop");
    result = (result * 10) / 12;
    return result;
}

int main(){    
    apply(
        // activate internal pull ups for i2c
        set(ino<SDA>),
        set(ino<SCL>),
        //Init i2c in master mode
        begin(i2c),
        //init uart
        begin(uart0)
    );

    // wake up?
    bh1750_set_mode(std::uint8_t{0x0});

    println(uart0, "BH1750 Test begin");

    while (1){
        bh1750_set_mode(std::uint8_t{0x10});
        delay_ms(180);
        auto light = bh1750_read();
        print(uart0, "Lux: ");
        print(uart0, light);
        println(uart0, " lx");
        delay_ms(1000);
    }
}