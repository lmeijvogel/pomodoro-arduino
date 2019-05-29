#ifndef GUI_LIGHT_CPP
#define GUI_LIGHT_CPP

#include "Light.hpp"

class GuiLight : public Light {
  public:
    virtual void turnOn() {
      this->isOn = true;
    }

    virtual void turnOff() {
      this->isOn = false;
    }


    bool getState() {
      return this->isOn;
    }

  private:
    bool isOn = false;
};

#endif
