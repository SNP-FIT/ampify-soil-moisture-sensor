#ifndef AMPIFYSOILMOISTURE_H
#define AMPIFYSOILMOISTURE_H

#include "Arduino.h"
#include <stdint.h>

#if defined(ESP32)
#include "driver/pulse_cnt.h"
#endif // defined(ESP32)

class AmpifySoilMoisture
{
public:
  AmpifySoilMoisture(int pin);
  void begin(unsigned long periodMs = 200);
  void setPeriod(unsigned long periodMs);
  // static unsigned long getPeriod();
  unsigned long getPeriod();
  unsigned long readMoisture(bool force = false);

private:
  // static uint32_t _periodMs;
  uint32_t _periodMs = 200;
  int _pin;
  uint32_t _lastResult;

#if defined(ARDUINO_ARCH_AVR)
  uint8_t _lastState = 0;
  uint32_t _lastReadTimeMicros;

  uint8_t _pinMask;
  volatile uint8_t *_pinPort;
  uint8_t _digitalReadFast();
#endif // defined(ARDUINO_ARCH_AVR)

#if defined(ESP32)
  static pcnt_unit_handle_t _pcntUnit;
  uint32_t _lastReadTimeMs;
#endif // defined(ESP32)
};

#endif
