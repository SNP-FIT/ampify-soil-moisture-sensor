#ifndef AMPIFYSOILMOISTUREASYNC_H
#define AMPIFYSOILMOISTUREASYNC_H

#include "Arduino.h"
#include <stdint.h>

#if defined(ESP32)
#define AMPIFYSOILMOISTURE_ESP32_MAX_DEVICE 14
#endif // defined(ESP32)

#if defined(ESP8266)
#include "Ticker.h"
#endif // defined(ESP8266)

class AmpifySoilMoistureAsync
{
public:
  AmpifySoilMoistureAsync(int pin);
  bool begin(unsigned long periodMs = 200);

  void setPeriod(unsigned long periodMs);
  unsigned long getPeriod();

#if defined(ARDUINO_ARCH_AVR)
  void setTimerCounter(unsigned long timerCounter);
  unsigned long getTimerCounter();
  void incrementTimerCounter();

  void setMoistureCounter(unsigned long moistureCounter);
  unsigned long getMoistureCounter();

  void setInternalCounter(unsigned long internalCounter);
  unsigned long getInternalCounter();

  static AmpifySoilMoistureAsync *instances[2];
#endif // defined(ARDUINO_ARCH_AVR)

#if defined(ESP32)
  void setTimerCounter(unsigned long timerCounter);
  unsigned long getTimerCounter();
  void incrementTimerCounter();

  void setMoistureCounter(unsigned long moistureCounter);
  unsigned long getMoistureCounter();

  void setInternalCounter(unsigned long internalCounter);
  unsigned long getInternalCounter();

  static int getInstanceIndex();
  static AmpifySoilMoistureAsync *instances[AMPIFYSOILMOISTURE_ESP32_MAX_DEVICE];
#endif // defined(ESP32)

  unsigned long readMoisture();

private:
  int _pin;
  uint32_t _periodMs = 200;

#if defined(ARDUINO_ARCH_AVR)
  static bool _isInitFirstTime;

  static void _sensorIsrExt0();
  static void _sensorIsrExt1();
  void _sensorIsr();

  unsigned long _timerCounter = 0;
  volatile unsigned long _vInternalMoistureCounter = 0;
  volatile unsigned long _vMoistureCounter = 0;
  unsigned long _lastEdge = 0;
#endif // defined(ARDUINO_ARCH_AVR)

#if defined(ESP32)
  static bool _isInitFirstTime;
  static int _instancesIdx;

  void IRAM_ATTR _sensorIsr();

  unsigned long _timerCounter = 0;
  volatile unsigned long _vInternalMoistureCounter = 0;
  volatile unsigned long _vMoistureCounter = 0;
  unsigned long _lastEdge = 0;
#endif // defined(ESP32)

#if defined(ESP8266)
  Ticker _ticker;

  void ICACHE_RAM_ATTR _sensorIsr();
  void _timerIsr();

  unsigned long _timerCounter = 0;
  volatile unsigned long _vInternalMoistureCounter = 0;
  volatile unsigned long _vMoistureCounter = 0;
  unsigned long _lastEdge = 0;
#endif // defined(ESP8266)
};

#endif // AMPIFYSOILMOISTUREASYNC_H
