#include "Arduino.h"
#include "AmpifySoilMoisture.h"

#define MIN_INTERVAL_MS 1000UL

AmpifySoilMoisture::AmpifySoilMoisture(int pin)
{
  _pin = pin;
}

void AmpifySoilMoisture::begin(unsigned long periodMs)
{
  _periodMs = periodMs;
  _lastReadTimeMicros = micros() - (MIN_INTERVAL_MS * 1000);
  pinMode(_pin, INPUT);

#if defined(ARDUINO_ARCH_AVR)
  _pinMask = digitalPinToBitMask(_pin);
  _pinPort = portInputRegister(digitalPinToPort(_pin));
#endif // defined(ARDUINO_ARCH_AVR)
}

void AmpifySoilMoisture::setPeriod(unsigned long periodMs)
{
  _periodMs = periodMs;
}

unsigned long AmpifySoilMoisture::getPeriod()
{
  return _periodMs;
}

unsigned long AmpifySoilMoisture::readMoisture(bool force)
{
  unsigned long startMicros = micros();

  if (!force && ((startMicros - _lastReadTimeMicros) < (MIN_INTERVAL_MS * 1000)))
  {
    return _lastResult;
  }

  _lastReadTimeMicros = startMicros;
  unsigned long currentMicros;
  unsigned long count = 0;

  do
  {
#if defined(ARDUINO_ARCH_AVR)
    uint8_t currentState = _digitalReadFast();
#else
    uint8_t currentState = digitalRead(_pin);
#endif // defined(ARDUINO_ARCH_AVR)
    currentMicros = micros();

    if (_lastState != currentState)
    {
      _lastState = currentState;

      if (currentState)
      {
        count++;
      }
    }

  } while (currentMicros - startMicros < (_periodMs * 1000));

  // calculate Hz
  _lastResult = (count * 1000) / _periodMs;
  return _lastResult;
}

#if defined(ARDUINO_ARCH_AVR)
uint8_t AmpifySoilMoisture::_digitalReadFast()
{
  return (*_pinPort & _pinMask) != 0;
}
#endif // defined(ARDUINO_ARCH_AVR)
