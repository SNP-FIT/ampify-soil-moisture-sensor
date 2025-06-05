/*
    AmpifySoilMoisture_Basic

    This example demonstrates how to use AmpifySoilMoisture library
    to read moisture value from Ampify Soil Moisture sensor.

    This example is to be used with AVR board (such as Arduino UNO), esp32 and esp8266 connected to Ampify Soil Moisture sensor.
*/

#include "AmpifySoilMoisture.h"

const int dataPin = 2;
AmpifySoilMoisture moistureSensor(dataPin);

unsigned long moistureValue = 0;

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
  delay(1000);
}
