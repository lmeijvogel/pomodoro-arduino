#ifndef NCURSES_GUI_H
#define NCURSES_GUI_H

#include <ncurses.h>

#include <stdlib.h>

#include "Gui.hpp"
#include "src/Context.cpp"
#include "src/GuiLight.cpp"
#include "src/Light.hpp"

typedef Light* LightPtr;
typedef GuiLight* GuiLightPtr;

const int HEADER_SIZE = 4;

class NCursesGui : public Gui {
  public:
    NCursesGui(
        Context *context,
        LightPtr *lights,
        int numberOfLights,
        GuiLight *statusOnLight) {
      this->context = context;
      this->lights = lights;
      this->numberOfLights = numberOfLights;
      this->statusOnLight = statusOnLight;
    }

    virtual void init() {
      initscr();
      cbreak();
      noecho();
      nodelay(stdscr, TRUE);
      refresh();
    }

    virtual void printIntro() {
      this->print(0, (char *)"q: Quit\n");
      this->print(0, (char *)"<space>: Button press\n");
      this->print(0, (char *)"\n");

      refresh();
    }

    virtual void update() {
      this->printState();
      this->printLights();
      this->printStatusLed();

      refresh();

    }

    void finish() {
      endwin();
    }

  private:
    Context *context;
    LightPtr *lights;
    int numberOfLights;
    GuiLight *statusOnLight;

    void printState() {
      std::string stateName = this->context->currentStateName();

      char description[60];

      sprintf(description, "%-59s", stateName.c_str());

      this->print(HEADER_SIZE + 2, description);

      refresh();
    }

    void printLights() {
      int lineLength = this->numberOfLights * 2 + 2;
      char *line = (char *)malloc(lineLength);

      for (int i = 0 ; i < this->numberOfLights ; i++ ) {
        GuiLightPtr pLight = (GuiLightPtr)lights[i];

        line[2*i] = pLight->getState() ? '*' : '.';
        line[2*i+1] = ' ';
      }

      line[this->numberOfLights * 2] = '\n';
      line[this->numberOfLights * 2 + 1] = 0;

      this->print(HEADER_SIZE + 6, line);

      free(line);
    }

    void addLightToBuffer(GuiLight *pLight, int position, char *output) {
      output[2*position] = pLight->getState() ? '*' : '.';
      output[2*position+1] = ' ';
    }

    void printStatusLed() {
      char line[1 * 2 + 2];

      this->addLightToBuffer((GuiLightPtr)statusOnLight, 0, line);

      this->print(HEADER_SIZE + 8, line);
    }

    virtual void print(int position, char *line) {
      if (position == 0) {
        printw(line);
      } else {
        mvprintw(position, 0, line);
      }
    }
};

#endif
