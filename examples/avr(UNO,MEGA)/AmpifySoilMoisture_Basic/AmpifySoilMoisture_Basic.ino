//TODO: Remove all unnecessary Serial.print
//TODO: Change Example folder structure remove top level folder 'avr(UNO,MEGA)' becuase this lib we use same code in every mcu.
#include "AmpifySoilMoisture.h"

AmpifySoilMoisture moistureSensor(2);

unsigned long moistureValue = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("hello, world");
  //TODO: Begin with on parameter (200ms)
  moistureSensor.begin(1000);
  Serial.println("after moisture begin");
}

void loop()
{
  //TODO: example with force = false (No parameter)
  moistureValue = moistureSensor.readMoisture(true);
  Serial.print("moisture value = ");
  Serial.println(moistureValue);
  //TODO: Delay 1000ms
  delay(500);
}
