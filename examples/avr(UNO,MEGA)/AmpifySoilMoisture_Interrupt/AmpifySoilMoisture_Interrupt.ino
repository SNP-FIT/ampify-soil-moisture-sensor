//TODO: Remove all unnecessary Serial.print
//TODO: Comment with Pin can be used in UNO,MEGA

#include "AmpifySoilMoistureInterrupt.h"

AmpifySoilMoistureInterrupt moistureSensor(2);

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
  moistureValue = moistureSensor.readMoisture();
  Serial.print("moisture value = ");
  Serial.println(moistureValue);
  delay(500);
}
