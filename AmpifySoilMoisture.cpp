#include "Arduino.h"
#include "AmpifySoilMoisture.h"

#define MIN_INTERVAL_MS 1000UL

// normal soil class
// uint32_t AmpifySoilMoisture::_periodMs = 200;

AmpifySoilMoisture::AmpifySoilMoisture(int pin)
{
  _pin = pin;
}

#if defined(ARDUINO_ARCH_AVR)
void AmpifySoilMoisture::begin(unsigned long periodMs)
{
  _periodMs = periodMs;
  _lastReadTimeMicros = micros() - (MIN_INTERVAL_MS * 1000);

  pinMode(_pin, INPUT);
  _pinMask = digitalPinToBitMask(_pin);
  _pinPort = portInputRegister(digitalPinToPort(_pin));
}
#endif // defined(ARDUINO_ARCH_AVR)

#if defined(ESP32)
pcnt_unit_handle_t AmpifySoilMoisture::_pcntUnit = NULL;

void AmpifySoilMoisture::begin(unsigned long periodMs)
{
  _periodMs = periodMs;
  _lastReadTimeMs = millis() - MIN_INTERVAL_MS;

  // install pcnt unit
  pcnt_unit_config_t unitConfig = {
      .low_limit = -100,
      .high_limit = 30000,
  };
  unitConfig.flags.accum_count = 1;
  pcnt_new_unit(&unitConfig, &_pcntUnit);

  // set glitch filter
  pcnt_glitch_filter_config_t filterConfig = {
      .max_glitch_ns = 1000,
  };
  pcnt_unit_set_glitch_filter(_pcntUnit, &filterConfig);

  // install pcnt channels
  pcnt_chan_config_t chanConfig = {
      .edge_gpio_num = _pin,
      .level_gpio_num = -1, // unused
  };
  pcnt_channel_handle_t pcntChan = NULL;
  ESP_ERROR_CHECK(pcnt_new_channel(_pcntUnit, &chanConfig, &pcntChan));

  // set edge and level action for pcnt channels
  pcnt_channel_set_edge_action(pcntChan, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_HOLD);

  // add watchpoint
  pcnt_unit_add_watch_point(_pcntUnit, 30000);

  // start pcnt unit
  pcnt_unit_enable(_pcntUnit);
  pcnt_unit_clear_count(_pcntUnit);
  pcnt_unit_start(_pcntUnit);
}
#endif // defined(ESP32)

void AmpifySoilMoisture::setPeriod(unsigned long periodMs)
{
  _periodMs = periodMs;
}

unsigned long AmpifySoilMoisture::getPeriod()
{
  return _periodMs;
}

#if defined(ARDUINO_ARCH_AVR)
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
#endif // defined(ARDUINO_ARCH_AVR)

#if defined(ESP32)
unsigned long AmpifySoilMoisture::readMoisture(bool force)
{
  unsigned long startMs = millis();

  if (!force && ((startMs - _lastReadTimeMs) < MIN_INTERVAL_MS))
  {
    return _lastResult;
  }

  _lastReadTimeMs = startMs;
  int count = 0;
  pcnt_unit_clear_count(_pcntUnit);
  delay(_periodMs);
  pcnt_unit_get_count(_pcntUnit, &count);

  // calculate Hz
  _lastResult = (count * 1000) / _periodMs;
  return _lastResult;
}
#endif // defined(ESP32)
// end normal soil class
