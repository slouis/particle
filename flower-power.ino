// This #include statement was automatically added by the Particle IDE.
#include <PowerShield.h>

PowerShield batteryMonitor;

//#define VCC 3.3
int PIN_TEMP = A3;

int photoresistor = A0; // This is where your photoresistor is plugged in. The other side goes to the "power" pin (below).
int humidity = A4;
int power = A5; // This is the other end of your photoresistor. The other side is plugged into the "photoresistor" pin (above).
// The reason we have plugged one side into an analog pin instead of to "power" is because we want a very steady voltage to be sent to the photoresistor.
// That way, when we read the value from the other side of the photoresistor, we can accurately calculate a voltage drop.

void setup() {

    RGB.control(true); 
    RGB.color(5, 5, 10);

    batteryMonitor.begin(); 
    batteryMonitor.quickStart();
    delay(500);
    
    //Spark.variable("temp_a", &tempanalog, DOUBLE);
    
    pinMode(photoresistor,INPUT);  // Our photoresistor pin is input (reading the photoresistor)
    pinMode(humidity, INPUT);
    pinMode(power,OUTPUT); // The pin powering the photoresistor is output (sending out consistent power)

    // Next, write one pin of the photoresistor to be the maximum possible, so that we can use this for power.
    digitalWrite(power,HIGH);
}

void loop() {
    
    double cellVoltage = batteryMonitor.getVCell();
    float stateOfCharge = batteryMonitor.getSoC();
    
    publish("soc", String(stateOfCharge),2000);
    
    double temp = tempAverage(1000, 10);
    double photo = map(analogRead(photoresistor), 0, 4095, 0, 100);
    double humi = map(analogRead(humidity), 0, 4095, 100, 0);
    
    publish("voltage", String(cellVoltage), 2000);
    publish("temp", String(temp), 2000);
    publish("lum", String(photo), 2000);
    publish("hum", String(humi), 2000);
    
    if(stateOfCharge > 40) {
        delay(3000);
        System.sleep(SLEEP_MODE_DEEP, 20*60);
    }else if(stateOfCharge > 30){
        delay(3000);
        System.sleep(SLEEP_MODE_DEEP, 60*60);
    }else{
        delay(3000);
        System.sleep(SLEEP_MODE_DEEP, 3*60*60);
    }
    
    
    //
}

void publish(String name, String value, int delayInMillis){
   Particle.publish(name, value);
   Spark.publish("plopup_" + name, value);
   delay(delayInMillis); 
}

double tempAverage(int lapsDelay, int t){
    double cumul = 0.0;
    for(int i=0; i<t; i++){
        double analogTension = analogRead(PIN_TEMP);
        cumul += (analogTension * 330) / 4095.0;
        delay(lapsDelay);
    }
    return cumul/t;
}
