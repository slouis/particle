#include "Adafruit_Sensor.h"
#include "Adafruit_BMP280.h"
#define BMP280_ADDRESS                (0x76) //Identified using I2C scan
// #define BMP_CS A2
// #define BMP_SCK A3
// #define BMP_MISO A4
// #define BMP_MOSI A5 

Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO, BMP_SCK);

// Update to reflect current pressure at sea level
float seaLevelhPa = 1021; //Nantes

void setup() {

  Particle.publish("DEBUG", "starting...");

  if (bmp.begin(BMP280_ADDRESS)) {
    Particle.publish("DEBUG", "starting the environment timer...");
  } else {
    Particle.publish("WARN", "Could not find a valid BMP280 sensor, check wiring!");
  }
  Particle.publish("DEBUG", "started!");
}

void loop() {
    delay(5000);
    checkEnvironment();

}

void checkEnvironment() {

    Particle.publish("DEBUG", "checking the environment...");
  
    //temperature - The sensor temperature in degrees Celsius.
    //pressure - The pressure in hPa = hectoPacal or 1 hPa = 1 milli bar = 1mbar
    //altitude - The altitude in meters.
    Particle.publish("environment/temperature", String(bmp.readTemperature()));
    Particle.publish("environment/pressure", String(bmp.readPressure()));
    Particle.publish("environment/altitude", String(bmp.readAltitude(seaLevelhPa)));
}
