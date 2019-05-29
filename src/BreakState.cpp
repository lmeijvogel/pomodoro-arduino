#ifndef BREAK_STATE_CPP
#define BREAK_STATE_CPP
#include "CountdownState.cpp"
#include "Light.hpp"

typedef Light* LightPtr;

class BreakState : public CountdownState {
  public:
    BreakState(long durationMillis, LightPtr* lights, int numberOfLights): CountdownState(durationMillis, lights, numberOfLights) { }

  protected:
    virtual void turnLightsOn(double timeFractionPassed) {
      int numberOfActivatedLights = timeFractionPassed * this->numberOfLights;

      for (int i = 0 ; i < this->numberOfLights ; i++) {
        LightPtr light = this->lights[i];

        if (i <= numberOfActivatedLights) {
          light->turnOn();
        } else {
          light->turnOff();
        }
      }
    }

    virtual void resetLightToDefaultState(LightPtr light) {
      light->turnOff();
    }

};
#endif
