#include <Adafruit_BMP280.h>

#include "Particle.h"
#include "application.h"
#include "math.h" 


//SYSTEM_MODE(MANUAL)----
SYSTEM_THREAD(ENABLED)

SerialLogHandler logHandler(LOG_LEVEL_ERROR, { // Logging level for non-application messages
    { "app", LOG_LEVEL_ALL } // Logging level for application messages
});



double cTemp = 0, fTemp = 0, pressure = 0, hum = 0;
#define Addr 0x76
Adafruit_BMP280 bmp; // I2C
float seaLevelhPa = 1021; //Nantes

void setup()
{
    Serial.begin();
    pinMode(A0, INPUT);
    
    if (bmp.begin(Addr)) {
        Particle.publish("DEBUG", "starting the environment timer...");
    } else {
        Particle.publish("WARN", "Could not find a valid BMP280 sensor, check wiring!");
    }
  
}

void loop() {
  static uint32_t ms = 0;
 
   
  if (millis() - ms < 5000)
      return;
  
  ms = millis();
  
  //Log.info("%d devices found", count);
  checkEnvironment();
 
 
 
  
}

void checkEnvironment() {

    //temperature - The sensor temperature in degrees Celsius.
    //pressure - The pressure in hPa = hectoPacal or 1 hPa = 1 milli bar = 1mbar
    //altitude - The altitude in meters.
    Particle.publish("environment/temperature", String(bmp.readTemperature()));
    Particle.publish("environment/pressure", String(bmp.readPressure()));
    Particle.publish("environment/altitude", String(bmp.readAltitude(seaLevelhPa)));
    Particle.publish("environment/humidity", String(analogRead(A0)), PRIVATE);
}
