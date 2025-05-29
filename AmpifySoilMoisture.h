#ifndef AMPIFYSOILMOISTURE_H
#define AMPIFYSOILMOISTURE_H

#include "Arduino.h"
#include <stdint.h>

class AmpifySoilMoisture
{
public:
  AmpifySoilMoisture(int pin);
  void begin(unsigned long periodMs = 200);
  void setPeriod(unsigned long periodMs);
  static unsigned long getPeriod();
  unsigned long readMoisture(bool force = false);

private:
  static uint32_t _periodMs;

  uint8_t _lastState = 0;
  uint32_t _lastResult;
  uint32_t _lastReadTimeMicros;

  int _pin;
  uint8_t _pinMask;
  volatile uint8_t *_pinPort;
  uint8_t _digitalReadFast();
};

#endif
