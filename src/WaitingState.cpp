#ifndef WAITING_STATE_CPP
#define WAITING_STATE_CPP
#include <math.h>

#include "State.hpp"
#include "Light.hpp"

typedef Light* LightPtr;

class WaitingState : public State {
  public:
    WaitingState(std::string name, LightPtr* lights, int numberOfLights) {
      this->_name = name;
      this->lights = lights;
      this->numberOfLights = numberOfLights;
    }

    virtual void reset(long currentTimeMillis) {
      for (int i = 0 ; i < this->numberOfLights ; i++) {
        LightPtr light = this->lights[i];

        light->turnOff();
      }
    }

    virtual void clockTick(long currentTimeMillis) { }

    virtual bool isFinished(long currentTimeMillis) {
      return false;
    }

    virtual std::string name() {
      return this->_name;
    }

  protected:
    LightPtr* lights;
    int numberOfLights;

    std::string _name;
};
#endif

