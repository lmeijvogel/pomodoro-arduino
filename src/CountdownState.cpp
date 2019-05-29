#ifndef COUNTDOWN_STATE_CPP
#define COUNTDOWN_STATE_CPP
#include <math.h>

#include "CounterState.hpp"
#include "Light.hpp"

typedef Light* LightPtr;

class CountdownState : public CounterState {
  public:
    CountdownState(long durationMillis, LightPtr* lights, int numberOfLights) {
      this->durationMillis = durationMillis;
      this->lights = lights;
      this->numberOfLights = numberOfLights;
    }

    virtual void reset(long currentTimeMillis) {
      this->startTimeMillis = currentTimeMillis;

      for (int i = 0 ; i < this->numberOfLights ; i++) {
        LightPtr light = this->lights[i];

        this->resetLightToDefaultState(light);
      }
    }

    virtual void clockTick(long currentTimeMillis) {
      long elapsedTimeMillis = currentTimeMillis - this->startTimeMillis;

      double timeFractionPassed = (double)elapsedTimeMillis / this->durationMillis;

      this->turnLightsOn(timeFractionPassed);
    }

    virtual bool isFinished(long currentTimeMillis) {
      return currentTimeMillis >= this->startTimeMillis + this->durationMillis;
    }

  protected:
    virtual void resetLightToDefaultState(LightPtr light) = 0;
    virtual void turnLightsOn(double timeFractionPassed) = 0;

    LightPtr* lights;
    int numberOfLights;

    long startTimeMillis;
    long durationMillis;
};
#endif

