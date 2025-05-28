#include "Arduino.h"
#include "AmpifySoilMoisture.h"

//TODO: Change to 1000ms
#define MIN_INTERVAL_MS 2000UL

// normal soil class
uint32_t AmpifySoilMoisture::_periodMs = 100;

AmpifySoilMoisture::AmpifySoilMoisture(int pin)
{
  _pin = pin;
}

//TODO: Make parameter optional with default 200ms
void AmpifySoilMoisture::begin(unsigned long periodMs)
{
  Serial.println("in begin()");
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


//TODO: Make this function return Hz not Count (need to divide periodMs) in Integer
unsigned long AmpifySoilMoisture::readMoisture(bool force)
{
  Serial.println("in readMoisture");
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

  _lastResult = count;
  return count; // TODOS calculate percentage
}

uint8_t AmpifySoilMoisture::_digitalReadFast()
{
  return (*_pinPort & _pinMask) != 0;
}
// end normal soil class
