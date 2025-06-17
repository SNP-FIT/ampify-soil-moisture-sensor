#include "Arduino.h"
#include "AmpifySoilMoistureAsync.h"

#define GLITCH_US 5

#if defined(ESP32)
#include "FunctionalInterrupt.h"
#endif // defined(ESP32)

#if defined(ESP8266)
#include "FunctionalInterrupt.h"
#include "Ticker.h"
#endif // defined(ESP8266)

// common
AmpifySoilMoistureAsync::AmpifySoilMoistureAsync(int pin)
{
  _pin = pin;
}

void AmpifySoilMoistureAsync::setPeriod(unsigned long periodMs)
{
  _periodMs = periodMs;
}

unsigned long AmpifySoilMoistureAsync::getPeriod()
{
  return _periodMs;
}
// end common

// avr
#if defined(ARDUINO_ARCH_AVR)
ISR(TIMER1_COMPA_vect)
{
  if (AmpifySoilMoistureAsync::instances[0] != NULL)
  {
    AmpifySoilMoistureAsync *sensor = AmpifySoilMoistureAsync::instances[0];
    sensor->incrementTimerCounter();

    if (sensor->getTimerCounter() >= sensor->getPeriod())
    {
      sensor->setMoistureCounter(sensor->getInternalCounter());
      sensor->setInternalCounter(0);
      sensor->setTimerCounter(0);
    }
  }

  if (AmpifySoilMoistureAsync::instances[1] != NULL)
  {
    AmpifySoilMoistureAsync *sensor = AmpifySoilMoistureAsync::instances[1];
    sensor->incrementTimerCounter();

    if (sensor->getTimerCounter() >= sensor->getPeriod())
    {
      sensor->setMoistureCounter(sensor->getInternalCounter());
      sensor->setInternalCounter(0);
      sensor->setTimerCounter(0);
    }
  }

#if defined(ARDUINO_AVR_MEGA2560)
  if (AmpifySoilMoistureAsync::instances[2] != NULL)
  {
    AmpifySoilMoistureAsync *sensor = AmpifySoilMoistureAsync::instances[2];
    sensor->incrementTimerCounter();

    if (sensor->getTimerCounter() >= sensor->getPeriod())
    {
      sensor->setMoistureCounter(sensor->getInternalCounter());
      sensor->setInternalCounter(0);
      sensor->setTimerCounter(0);
    }
  }

  if (AmpifySoilMoistureAsync::instances[3] != NULL)
  {
    AmpifySoilMoistureAsync *sensor = AmpifySoilMoistureAsync::instances[3];
    sensor->incrementTimerCounter();

    if (sensor->getTimerCounter() >= sensor->getPeriod())
    {
      sensor->setMoistureCounter(sensor->getInternalCounter());
      sensor->setInternalCounter(0);
      sensor->setTimerCounter(0);
    }
  }
#endif // defined(ARDUINO_AVR_MEGA2560)
}

AmpifySoilMoistureAsync *AmpifySoilMoistureAsync::instances[AMPIFYSOILMOISTURE_AVR_MAX_DEVICE] = {};
bool AmpifySoilMoistureAsync::_isInitFirstTime = true;

bool AmpifySoilMoistureAsync::begin(unsigned long periodMs)
{
  _periodMs = periodMs;
  pinMode(_pin, INPUT);

  switch (_pin)
  {
  case 2:
    attachInterrupt(digitalPinToInterrupt(2), _sensorIsrExt0, RISING);
    instances[0] = this;
    break;

  case 3:
    attachInterrupt(digitalPinToInterrupt(3), _sensorIsrExt1, RISING);
    instances[1] = this;
    break;

#if defined(ARDUINO_AVR_MEGA2560)
  case 19:
    attachInterrupt(digitalPinToInterrupt(19), _sensorIsrExt4, RISING);
    instances[2] = this;
    break;

  case 18:
    attachInterrupt(digitalPinToInterrupt(18), _sensorIsrExt5, RISING);
    instances[3] = this;
    break;
#endif // defined(ARDUINO_AVR_MEGA2560)

  default:
    // unexpected pin
    return false;
    break;
  }

  if (_isInitFirstTime)
  {
    cli();
    // set timer1 interrupt at 1kHz
    TCCR1A = 0; // set entire TCCR1A register to 0
    TCCR1B = 0; // same for TCCR1B
    TCNT1 = 0;  // initialize counter value to 0
    // set compare match register for 1khz increments
    OCR1A = 1999; // = (16*10^6) / (1000*8) - 1 (must be <65536)
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS11 bits for 8 prescaler
    TCCR1B |= (1 << CS11);
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
    sei();

    _isInitFirstTime = false;
  }

  return true;
}

void AmpifySoilMoistureAsync::setTimerCounter(unsigned long timerCounter)
{
  _timerCounter = timerCounter;
}

unsigned long AmpifySoilMoistureAsync::getTimerCounter()
{
  return _timerCounter;
}

void AmpifySoilMoistureAsync::incrementTimerCounter()
{
  _timerCounter++;
}

void AmpifySoilMoistureAsync::setMoistureCounter(unsigned long moistureCounter)
{
  _vMoistureCounter = moistureCounter;
}

unsigned long AmpifySoilMoistureAsync::getMoistureCounter()
{
  // volatile variables must be read from memory on every access
  unsigned long m = _vMoistureCounter;
  return m;
}

void AmpifySoilMoistureAsync::setInternalCounter(unsigned long internalCounter)
{
  _vInternalMoistureCounter = internalCounter;
}

unsigned long AmpifySoilMoistureAsync::getInternalCounter()
{
  // volatile variables must be read from memory on every access
  unsigned long i = _vInternalMoistureCounter;
  return i;
}

unsigned long AmpifySoilMoistureAsync::readMoisture()
{
  return (getMoistureCounter() * 1000) / _periodMs;
}

void AmpifySoilMoistureAsync::_sensorIsrExt0()
{
  if (AmpifySoilMoistureAsync::instances[0] != NULL)
  {
    AmpifySoilMoistureAsync::instances[0]->_sensorIsr();
  }
}

void AmpifySoilMoistureAsync::_sensorIsrExt1()
{
  if (AmpifySoilMoistureAsync::instances[1] != NULL)
  {
    AmpifySoilMoistureAsync::instances[1]->_sensorIsr();
  }
}

#if defined(ARDUINO_AVR_MEGA2560)
void AmpifySoilMoistureAsync::_sensorIsrExt4()
{
  if (AmpifySoilMoistureAsync::instances[2] != NULL)
  {
    AmpifySoilMoistureAsync::instances[2]->_sensorIsr();
  }
}

void AmpifySoilMoistureAsync::_sensorIsrExt5()
{
  if (AmpifySoilMoistureAsync::instances[3] != NULL)
  {
    AmpifySoilMoistureAsync::instances[3]->_sensorIsr();
  }
}
#endif // defined(ARDUINO_AVR_MEGA2560)

void AmpifySoilMoistureAsync::_sensorIsr()
{
  unsigned long m = micros();
  if (m - _lastEdge >= GLITCH_US)
  {
    _vInternalMoistureCounter++;
    _lastEdge = m;
  }
}
#endif // defined(ARDUINO_ARCH_AVR)
// end avr

// esp32
#if defined(ESP32)
void IRAM_ATTR _timerIsr()
{
  for (size_t i = 0; i < AmpifySoilMoistureAsync::getInstanceIndex(); i++)
  {
    AmpifySoilMoistureAsync *sensor = AmpifySoilMoistureAsync::instances[i];
    sensor->incrementTimerCounter();

    if (sensor->getTimerCounter() >= sensor->getPeriod())
    {
      sensor->setMoistureCounter(sensor->getInternalCounter());
      sensor->setInternalCounter(0);
      sensor->setTimerCounter(0);
    }
  }
}

AmpifySoilMoistureAsync *AmpifySoilMoistureAsync::instances[] = {};
bool AmpifySoilMoistureAsync::_isInitFirstTime = true;
int AmpifySoilMoistureAsync::_instancesIdx = 0;
hw_timer_t *_timerHandle = NULL;

bool AmpifySoilMoistureAsync::begin(unsigned long periodMs)
{
  _periodMs = periodMs;
  pinMode(_pin, INPUT);

  if (_instancesIdx >= AMPIFYSOILMOISTURE_ESP32_MAX_DEVICE)
  {
    return false;
  }

  attachInterrupt(digitalPinToInterrupt(_pin), std::bind(&AmpifySoilMoistureAsync::_sensorIsr, this), RISING);
  instances[_instancesIdx] = this;
  _instancesIdx++;

  if (_isInitFirstTime)
  {
    // Set timer frequency to 1Mhz
    _timerHandle = timerBegin(1000000);
    timerAttachInterrupt(_timerHandle, &_timerIsr);
    // Set alarm to call onTimer function every second (value in microseconds).
    // Repeat the alarm (third parameter) with unlimited count = 0 (fourth parameter).
    timerAlarm(_timerHandle, 1000, true, 0);

    _isInitFirstTime = false;
  }

  return true;
}

void AmpifySoilMoistureAsync::setTimerCounter(unsigned long timerCounter)
{
  _timerCounter = timerCounter;
}

unsigned long AmpifySoilMoistureAsync::getTimerCounter()
{
  return _timerCounter;
}

void AmpifySoilMoistureAsync::incrementTimerCounter()
{
  _timerCounter++;
}

void AmpifySoilMoistureAsync::setMoistureCounter(unsigned long moistureCounter)
{
  _vMoistureCounter = moistureCounter;
}

unsigned long AmpifySoilMoistureAsync::getMoistureCounter()
{
  // volatile variables must be read from memory on every access
  unsigned long m = _vMoistureCounter;
  return m;
}

void AmpifySoilMoistureAsync::setInternalCounter(unsigned long internalCounter)
{
  _vInternalMoistureCounter = internalCounter;
}

unsigned long AmpifySoilMoistureAsync::getInternalCounter()
{
  // volatile variables must be read from memory on every access
  unsigned long i = _vInternalMoistureCounter;
  return i;
}

int AmpifySoilMoistureAsync::getInstanceIndex()
{
  return _instancesIdx;
}

unsigned long AmpifySoilMoistureAsync::readMoisture()
{
  return (getMoistureCounter() * 1000) / _periodMs;
}

void IRAM_ATTR AmpifySoilMoistureAsync::_sensorIsr()
{
  unsigned long m = micros();
  if (m - _lastEdge >= GLITCH_US)
  {
    _vInternalMoistureCounter++;
    _lastEdge = m;
  }
}

#endif // defined(ESP32)
// end esp32

// esp8266
#if defined(ESP8266)
bool AmpifySoilMoistureAsync::begin(unsigned long periodMs)
{
  _periodMs = periodMs;
  pinMode(_pin, INPUT);

  attachInterrupt(digitalPinToInterrupt(_pin), std::bind(&AmpifySoilMoistureAsync::_sensorIsr, this), RISING);
  _ticker.attach_ms(1, [this]()
                    { _timerIsr(); });

  return true;
}

void ICACHE_RAM_ATTR AmpifySoilMoistureAsync::_sensorIsr()
{
  unsigned long m = micros();
  if (m - _lastEdge >= GLITCH_US)
  {
    _vInternalMoistureCounter++;
    _lastEdge = m;
  }
}

unsigned long AmpifySoilMoistureAsync::readMoisture()
{
  return (_vMoistureCounter * 1000) / _periodMs;
}

void AmpifySoilMoistureAsync::_timerIsr()
{
  _timerCounter++;

  if (_timerCounter >= _periodMs)
  {
    _vMoistureCounter = _vInternalMoistureCounter;
    _vInternalMoistureCounter = 0;
    _timerCounter = 0;
  }
}
#endif // defined(ESP8266)
// end esp8266
