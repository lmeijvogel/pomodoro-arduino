#ifndef RANDOM_BLINKING_STATE_CPP
#define RANDOM_BLINKING_STATE_CPP

#include "CountdownState.cpp"
#include "Light.hpp"

#include "Logger.cpp"
#include "RandomGenerator.h"

typedef Light* LightPtr;
typedef const char** Pattern;

class RandomBlinkingState : public CountdownState {
  public:
    RandomBlinkingState(unsigned long durationMillis, LightPtr *lights, int numberOfLights, bool offToOn, const char *name) : CountdownState(durationMillis, lights, numberOfLights) {
      this->randomGenerator = new RandomGenerator(123);
      this->_name = name;

      this->offToOn = offToOn;
    }

    virtual const char* name() {
      return this->_name;
    }

    virtual void resetLightToDefaultState(LightPtr light) {
      if (this->offToOn) {
        light->turnOff();
      } else {
        light->turnOn();
      }
    }

    virtual void turnLightsOn(double timeFractionPassed) {
    // virtual void turnLightsOn(double) {
      for (int i = 0 ; i < this->numberOfLights ; i++) {
        Light *light = this->lights[i];

        long random = randomGenerator->getNext(1000);

        long scaledTimeFraction = (long)(timeFractionPassed * 1000L);

        if ((random < scaledTimeFraction) == this->offToOn) {
          light->turnOn();
        } else {
          light->turnOff();
        }
      }
    }

  private:
    RandomGenerator *randomGenerator;

    const char *_name;

    bool offToOn;
};
#endif
