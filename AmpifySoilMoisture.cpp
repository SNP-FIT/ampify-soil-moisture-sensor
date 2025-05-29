#include "Arduino.h"
#include "AmpifySoilMoisture.h"

#define MIN_INTERVAL_MS 1000UL

// normal soil class
uint32_t AmpifySoilMoisture::_periodMs = 200;

AmpifySoilMoisture::AmpifySoilMoisture(int pin)
{
  _pin = pin;
}

void AmpifySoilMoisture::begin(unsigned long periodMs)
{
  _periodMs = periodMs;
  _lastReadTimeMicros = micros() - (MIN_INTERVAL_MS * 1000);

  pinMode(_pin, INPUT);
  _pinMask = digitalPinToBitMask(_pin);
  _pinPort = portInputRegister(digitalPinToPort(_pin));
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
    uint8_t currentState = _digitalReadFast();
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

uint8_t AmpifySoilMoisture::_digitalReadFast()
{
  return (*_pinPort & _pinMask) != 0;
}
// end normal soil class
