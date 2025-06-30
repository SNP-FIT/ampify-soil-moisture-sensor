/*
    AmpifySoilMoisture_Async

    This example demonstrates how to use AmpifySoilMoisture library
    to read moisture value from Ampify Soil Moisture sensor without blocking the main program.

    This example is to be used with AVR (such as Arduino UNO), esp32 and esp8266 connected to Ampify Soil Moisture sensor.

    This non-blocking approach, while efficient, comes with some limitations. 
    It relies on external interrupts and, for certain boards, timer interrupts. 
    Because of this dependency, not all pins can be used for the sensor in this example

    Available pin to use:
      - Arduino UNO R3, Nano, Mini (328-based): 2,3
      - Arduino MEGA 2560: 2,3,18,19
      - ESP32: all usable gpio
      - ESP8266: 4(D2),5(D1),12(D6),13(D7),14(D5)

    For Arduino UNO/MEGA and ESP32, Timer1 is already in use and therefore cannot be used with this example..
    For ESP8266, Ticker library is used.
*/


#include "AmpifySoilMoistureAsync.h"

#define MOISTURE_MIN_VALUE 0
#define MOISTURE_MAX_VALUE 30000

const int dataPin = 2;
AmpifySoilMoistureAsync moistureSensor(dataPin);

unsigned long moistureValue = 0;
unsigned long moisturePercentage = 0;

void setup()
{
  Serial.begin(9600);
  moistureSensor.begin();
}

void loop()
{
  moistureValue = moistureSensor.readMoisture();
  Serial.print("moisture value = ");
  Serial.println(moistureValue);

  moisturePercentage = constrain(map(moistureValue, MOISTURE_MIN_VALUE, MOISTURE_MAX_VALUE, 100, 0),0,100);
  Serial.print("moisture percentage = ");
  Serial.println(moisturePercentage);
  delay(1000);
}
