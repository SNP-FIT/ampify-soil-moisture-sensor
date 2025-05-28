#include "Arduino.h"
#include "AmpifySoilMoistureInterrupt.h"

#define GLITCH_US 5

// timer1 class
class Timer1Utils {
  public:
    Timer1Utils() {}

    unsigned long getMoistureCounter() {
      // volatile variables must be read from memory on every access
      unsigned long m = vMoistureCounter;
      return m;
    }

    static void sensorIsr() {
      if (micros() - lastEdge >= GLITCH_US) {
        vInternalMoistureCounter++;
        lastEdge = micros();
      }
    }

    static volatile unsigned long vInternalMoistureCounter;
    volatile unsigned long vMoistureCounter = 0;
    unsigned long timerCounter = 0;
    static unsigned long lastEdge;
};

Timer1Utils timer1;
volatile unsigned long Timer1Utils::vInternalMoistureCounter = 0;
unsigned long Timer1Utils::lastEdge = 0;

ISR(TIMER1_COMPA_vect) {
  timer1.timerCounter++;
  if (timer1.timerCounter >= AmpifySoilMoistureInterrupt::getPeriod()) {
    timer1.vMoistureCounter = timer1.vInternalMoistureCounter;
    timer1.vInternalMoistureCounter = 0;
    timer1.timerCounter = 0;
  }
}
// end timer1 class

uint32_t AmpifySoilMoistureInterrupt::_periodMs = 100;

AmpifySoilMoistureInterrupt::AmpifySoilMoistureInterrupt(int pin) {
  _pin = pin;
}


//TODO: Make parameter optional with default 200ms
bool AmpifySoilMoistureInterrupt::begin(unsigned long periodMs) {
  //TODO: Remove all unnecessary Serial.print
  Serial.println("in begin()");
  _periodMs = periodMs;

  // interrupt and counter setting
  if(_pin != 2 && _pin != 3){
    // invalid pin
    return false;
  }
  
  pinMode(_pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(_pin), timer1.sensorIsr, RISING);

  cli();
  //set timer1 interrupt at 1kHz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1khz increments
  OCR1A = 1999;// = (16*10^6) / (1000*8) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 bits for 8 prescaler
  TCCR1B |= (1 << CS11);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();

  return true;
}

void AmpifySoilMoistureInterrupt::setPeriod(unsigned long periodMs) {
  _periodMs = periodMs;
}

unsigned long AmpifySoilMoistureInterrupt::getPeriod() {
  return _periodMs;
}

//TODO: Make this function return Hz not Count (need to divide periodMs) in Integers
unsigned long AmpifySoilMoistureInterrupt::readMoisture() {
  return timer1.getMoistureCounter();
}
