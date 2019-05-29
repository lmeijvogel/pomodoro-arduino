#ifndef POMODORO_STATE_CPP
#define POMODORO_STATE_CPP
#include <math.h>
#include <stdio.h>

#include "CountdownState.cpp"
#include "Light.hpp"

typedef Light* LightPtr;

class PomodoroState : public CountdownState {
  public:
    PomodoroState(long durationMillis, LightPtr* lights, int numberOfLights) : CountdownState(durationMillis, lights, numberOfLights) { }

  protected:
    virtual void resetLightToDefaultState(LightPtr light) {
      light->turnOn();
    }

    virtual void turnLightsOn(double timeFractionPassed) {
      double timeFractionRemaining = 1 - timeFractionPassed;

      int numberOfRemainingLights = timeFractionRemaining * this->numberOfLights;

      for (int i = 0 ; i < this->numberOfLights ; i++) {
        LightPtr light = this->lights[i];

        if (i <= numberOfRemainingLights) {
          light->turnOn();
        } else {
          light->turnOff();
        }
      }
    }

};
#endif
