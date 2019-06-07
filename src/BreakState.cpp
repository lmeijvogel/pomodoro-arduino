#ifndef BREAK_STATE_CPP
#define BREAK_STATE_CPP
#include "CountdownState.cpp"
#include "Light.hpp"

#include "Logger.cpp"

typedef Light* LightPtr;

class BreakState : public CountdownState {
  public:
    BreakState(long durationMillis, LightPtr* lights, int numberOfLights): CountdownState(durationMillis, lights, numberOfLights) { }

    virtual const char *name() {
      return "break";
    }

  protected:
    virtual void resetLightToDefaultState(LightPtr light) {
      light->turnOff();
    }

    virtual void turnLightsOn(double timeFractionPassed) {
      int numberOfActivatedLights = timeFractionPassed * this->numberOfLights;

      for (int i = 0 ; i < this->numberOfLights ; i++) {
        int lightIndex = this->numberOfLights - i - 1;
        LightPtr light = this->lights[lightIndex];

        if (i < numberOfActivatedLights) {
          light->turnOn();
        } else {
          light->turnOff();
        }
      }
    }
};
#endif
