/*
    AmpifySoilMoisture_Basic

    This example demonstrates how to use AmpifySoilMoisture library
    to read moisture value from Ampify Soil Moisture sensor.

    This example is to be used with AVR board (such as Arduino UNO), esp32 and esp8266 connected to Ampify Soil Moisture sensor.
*/

#include "AmpifySoilMoisture.h"

#define MOISTURE_MIN_VALUE 0
#define MOISTURE_MAX_VALUE 30000

const int dataPin = 2;
AmpifySoilMoisture moistureSensor(dataPin);

unsigned long moistureValue = 0;
unsigned long moisturePercentage = 0;

void setup()
{
  Serial.begin(9600);
  moistureSensor.begin();
}

void loop()
{
  //This function has a default execution time of approximately 200ms. For a non-blocking alternative, refer to the examples/AmpifySoilMoisture_Async example and its associated limitations.
  moistureValue = moistureSensor.readMoisture();
  Serial.print("moisture value = ");
  Serial.println(moistureValue);

  moisturePercentage = map(moistureValue, MOISTURE_MIN_VALUE, MOISTURE_MAX_VALUE, 0, 100);
  Serial.print("moisture percentage = ");
  Serial.println(moisturePercentage);
  delay(1000);
}
