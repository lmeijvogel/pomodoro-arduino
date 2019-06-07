#ifndef WAITING_STATE_CPP
#define WAITING_STATE_CPP
#include <math.h>

#include "State.hpp"
#include "Light.hpp"

typedef Light* LightPtr;

class WaitingState : public State {
  public:
    WaitingState(const char* name, const char *pattern, LightPtr* lights, int numberOfLights) {
      this->_name = name;
      this->pattern = pattern;
      this->lights = lights;
      this->numberOfLights = numberOfLights;
    }

    virtual void reset(long) {
      for (int i = 0 ; i < this->numberOfLights ; i++) {
        LightPtr light = this->lights[i];

        light->turnOff();
      }
    }

    virtual void clockTick(long) {
      for (int i = 0 ; i < numberOfLights ; i++) {
        LightPtr light = this->lights[i];

        char value = pattern[i];

        if (value == '1') {
          light->turnOn();
        } else {
          light->turnOff();
        }
      }
    }

    virtual bool isFinished(long) {
      return false;
    }

    virtual const char* name() {
      return this->_name;
    }

  protected:
    const char *pattern;
    LightPtr* lights;
    int numberOfLights;

    const char* _name;
};
#endif

