#ifndef BARE_GUI_H
#define BARE_GUI_H

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "Gui.hpp"
#include "src/GuiLight.cpp"
#include "src/Light.hpp"

typedef Light* LightPtr;
typedef GuiLight* GuiLightPtr;

class BareGui : public Gui {
  public:
    BareGui(LightPtr *lights,
        int numberOfLights,
        Light *statusOnLight) {
      this->lights = lights;
      this->numberOfLights = numberOfLights;
      this->statusOnLight = statusOnLight;
    }

    virtual void init() {
    }

    virtual void printIntro() {
      printf("The intro\n");
    }

    virtual void update() {
      char stateDescription[10];

      this->printState(stateDescription);

      printf("%s", stateDescription);

      char *lights = (char *)malloc(this->numberOfLights * 2 + 2);

      this->printLights(lights);

      printf("  %s", lights);
      free(lights);

      printf("\n");
    }

    virtual void finish() {
    }

  private:
    LightPtr *lights;
    int numberOfLights;
    Light *statusOnLight;


    void printState(char *output) {
      // State state = this->stateMachine->getState();

      // switch (state) {
      // case StateOff:
      // sprintf(output, "%-10s", "Off");
      // break;
      // case StateTurningOn:
      // sprintf(output, "%-10s", "TurningOn");
      // break;
      // case StateAnimating:
      // sprintf(output, "%-10s", "Animating");
      // break;
      // case StateOn:
      // sprintf(output, "%-10s", "On");
      // break;
      // case StateTurningOff:
      // sprintf(output, "%-10s", "TurningOff");
      // break;
      // }
    }

    void printLights(char *output) {
      for (int i = 0 ; i < this->numberOfLights ; i++ ) {
        GuiLightPtr pLight = (GuiLightPtr)lights[i];

        output[2*i] = pLight->getState() ? '*' : '.';
        output[2*i+1] = ' ';
      }
      output[this->numberOfLights * 2 - 1] = 0;
    }

    void addLightToBuffer(GuiLight *pLight, int position, char *output) {
      output[2*position] = pLight->getState() ? '*' : '.';
      output[2*position+1] = ' ';
    }

    void printStatusLed() {
      char output[1 * 2 + 2];

      this->addLightToBuffer((GuiLightPtr)statusOnLight, 2, output);
    }
};

#endif

