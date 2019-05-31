#ifndef BLINKING_STATE_CPP
#define BLINKING_STATE_CPP

#include "CountdownState.cpp"

class BlinkingState : public CountdownState {
  public:
    BlinkingState(long durationMillis, int numberOfBlinks, LightPtr* lights, int numberOfLights): CountdownState(durationMillis, lights, numberOfLights) {
      this->numberOfBlinks = numberOfBlinks;
    }

    virtual void resetLightToDefaultState(LightPtr light) {
      light->turnOn();
    }

    virtual void turnLightsOn(double timeFractionPassed) {
      bool shouldTurnOn = this->inOnFraction(timeFractionPassed);

      for (int i = 0 ; i < this->numberOfLights ; i++) {
        LightPtr light = this->lights[i];

        if (shouldTurnOn) {
          light->turnOn();
        } else {
          light->turnOff();
        }
      }
    }

  private:
    int numberOfBlinks;

    bool inOnFraction(double timeFractionPassed) {
      for (int i = 0 ; i < numberOfBlinks ; i++) {

        double blinkStart = (1.0 / numberOfBlinks) * i;
        double blinkEnd = blinkStart + (1.0 / numberOfBlinks) / 4;

        if (blinkStart <= timeFractionPassed && timeFractionPassed < blinkEnd) {
          return true;
        }
      }

      return false;
    }
};

#endif
