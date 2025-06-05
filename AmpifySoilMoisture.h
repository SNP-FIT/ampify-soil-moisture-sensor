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
  unsigned long getPeriod();
  unsigned long readMoisture(bool force = false);

private:
  uint32_t _periodMs = 200;
  int _pin;
  uint32_t _lastResult = 0;

  uint8_t _lastState = 0;
  uint32_t _lastReadTimeMicros;

#if defined(ARDUINO_ARCH_AVR)
  uint8_t _pinMask;
  volatile uint8_t *_pinPort;
  uint8_t _digitalReadFast();
#endif // defined(ARDUINO_ARCH_AVR)
};

#endif
