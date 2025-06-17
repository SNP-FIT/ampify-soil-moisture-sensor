/*
    AmpifySoilMoisture_Interrupt

    This example demonstrates how to use AmpifySoilMoisture library
    to read moisture value from Ampify Soil Moisture sensor with an interrupt.

    This example is to be used with AVR board connected to Ampify Soil Moisture sensor.

    Available pin to use:
      - Arduino UNO R3, Nano, Mini (328-based): 2,3
      - Arduino MEGA: 2,3,18,19

    Timer1 cannot be used with this example.
*/

#include "AmpifySoilMoistureAsync.h"

const int dataPin = 2;
AmpifySoilMoistureAsync moistureSensor(dataPin);

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
