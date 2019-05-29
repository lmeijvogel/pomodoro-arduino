#ifndef LED_LIGHT_CPP
#define LED_LIGHT_CPP

#include "Arduino.h"
#include "Light.h"

class LedLight : public Light {
public:
  LedLight(int pin) {
    this->pin = pin;
    pinMode(pin, OUTPUT);
  }

  virtual void turnOn()
  {
    digitalWrite(pin, HIGH);
  }

  virtual void turnOff() {
    digitalWrite(pin, LOW);
  }

private:
  int pin;
};
#endif
