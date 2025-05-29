#ifndef AMPIFYSOILMOISTUREINTERRUPT_H
#define AMPIFYSOILMOISTUREINTERRUPT_H

#include "Arduino.h"
#include <stdint.h>

class AmpifySoilMoistureInterrupt
{
public:
  AmpifySoilMoistureInterrupt(int pin);
  bool begin(unsigned long periodMs = 200);

  void setPeriod(unsigned long periodMs);
  static unsigned long getPeriod();

  unsigned long readMoisture();

private:
  int _pin;
  static uint32_t _periodMs;
};

#endif
