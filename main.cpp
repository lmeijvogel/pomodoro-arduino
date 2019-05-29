#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <chrono>
#include <ncurses.h>
#include <string.h>

#include "src/PomodoroState.cpp"
#include "src/CountdownState.cpp"
#include "src/BreakState.cpp"
#include "src/PomodoroState.cpp"
#include "src/Light.hpp"
#include "src/GuiLight.cpp"
#include "Gui.hpp"
#include "NCursesGui.cpp"
#include "BareGui.cpp"

const int NUMBER_OF_LIGHTS = 5;

typedef GuiLight* GuiLightPtr;

GuiLight *statusOnLight = new GuiLight();

GuiLightPtr *createLights();

Gui *gui;

void handleInput(int ch, int elapsedTimeMs);
int secondsSinceEpoch();

void cleanupAndExit() {
  endwin();

  exit(1);
}

void sigIntReceived(int s) {
  cleanupAndExit();
}

int main() {
  auto startTime = std::chrono::system_clock::now();

  GuiLightPtr *lights = createLights();

  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = sigIntReceived;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

  sigaction(SIGINT, &sigIntHandler, NULL);

  CountdownState *counterState = new PomodoroState(2500, (LightPtr *)lights, NUMBER_OF_LIGHTS);
  // CountdownState *counterState = new BreakState(2500, (LightPtr *)lights, NUMBER_OF_LIGHTS);

  gui = new NCursesGui((LightPtr *)lights, NUMBER_OF_LIGHTS, statusOnLight);
  // gui = new BareGui((LightPtr *)lights, NUMBER_OF_LIGHTS, statusOnLight);

  counterState->reset(0);
  gui->init();

  gui->printIntro();

  while (true) {
    usleep(50000);

    auto now = std::chrono::system_clock::now();

    int elapsedTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();

    counterState->clockTick(elapsedTimeMs);

    if (counterState->isFinished(elapsedTimeMs)) {
      counterState->reset(elapsedTimeMs);
    }

    int ch = getch();

    if (ch == 'q') {
      delete[] lights;
      cleanupAndExit();
    } else if (ch != ERR) {
      handleInput(ch, elapsedTimeMs);
    }

    gui->update();
  }
}

GuiLightPtr *createLights() {
  GuiLightPtr *lights = new GuiLightPtr[NUMBER_OF_LIGHTS];

  for (int i = 0 ; i < NUMBER_OF_LIGHTS ; i++) {
    GuiLight *light = new GuiLight;
    lights[i] = light;
  }

  return lights;
}

void handleInput(int ch, int elapsedTimeMs) {
  // switch(ch) {
  // case 'z':
    // stateMachine.switchOff();
    // break;
  // case 'x':
    // stateMachine.switchGradual(elapsedTimeMs, GRADUAL_TRANSITION_PERIOD_MS);
    // break;
  // case 'c':
    // stateMachine.switchOn();
    // break;
  // }
}

int secondsSinceEpoch() {
  auto start = std::chrono::system_clock::now();

  return std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();
}
