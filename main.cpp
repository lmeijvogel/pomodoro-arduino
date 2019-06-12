#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <chrono>
#include <ncurses.h>
#include <string.h>

#include "src/Context.cpp"
#include "src/CountdownState.cpp"
#include "src/BreakState.cpp"
#include "src/WaitingState.cpp"
#include "src/PomodoroState.cpp"
#include "src/BlinkingState.cpp"
#include "src/PatternedBlinkingState.cpp"
#include "src/Light.hpp"
#include "src/GuiLight.cpp"
#include "Gui.hpp"
#include "NCursesGui.cpp"
#include "BareGui.cpp"

const int NUMBER_OF_LIGHTS = 5;

// const char *blinkingPattern[] = { "10001", "01010", "00100", "01010", "10001" };
const char *blinkingPattern[] = { "10000", "01000", "00100", "00010", "00001", "00010", "00100", "01000" };

const long BLINKING_PATTERN_STATE_DURATION = 1000;
const int NUMBER_OF_BLINKING_PATTERNS = 8;
const int NUMBER_OF_BLINKING_PATTERN_REPETITIONS = 2;
typedef GuiLight* GuiLightPtr;

GuiLight *statusOnLight = new GuiLight();

GuiLightPtr *createLights();

Gui *gui;

void handleInput(int ch, Context *context, int elapsedTimeMs);
int secondsSinceEpoch();

void cleanupAndExit() {
  endwin();

  exit(1);
}

void sigIntReceived(int) {
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

  PatternedBlinkingState pomodoroStartedState(BLINKING_PATTERN_STATE_DURATION, (LightPtr *)lights, NUMBER_OF_LIGHTS, "pomodoroStarted", blinkingPattern, NUMBER_OF_BLINKING_PATTERNS, NUMBER_OF_BLINKING_PATTERN_REPETITIONS);
  PomodoroState pomodoroState(2500, (LightPtr *)lights, NUMBER_OF_LIGHTS);
  BlinkingState pomodoroDoneState("pomodoroDone", 3000, 5, (LightPtr *)lights, NUMBER_OF_LIGHTS);
  WaitingState waitingForBreakState("waitingForBreak", "01010", (LightPtr *)lights, NUMBER_OF_LIGHTS);
  PatternedBlinkingState breakStartedState(BLINKING_PATTERN_STATE_DURATION, (LightPtr *)lights, NUMBER_OF_LIGHTS, "breakStarted", blinkingPattern, NUMBER_OF_BLINKING_PATTERNS, NUMBER_OF_BLINKING_PATTERN_REPETITIONS);
  BreakState breakState(2500, (LightPtr *)lights, NUMBER_OF_LIGHTS);
  BlinkingState breakDoneState("breakDone", 3000, 5, (LightPtr *)lights, NUMBER_OF_LIGHTS);
  WaitingState waitingForPomodoroState("waitingForPomodoro", "00100", (LightPtr *)lights, NUMBER_OF_LIGHTS);

  Context *context = new Context(&waitingForPomodoroState, &pomodoroStartedState, &pomodoroState, &pomodoroDoneState, &waitingForBreakState, &breakStartedState, &breakState, &breakDoneState);

  gui = new NCursesGui(context, (LightPtr *)lights, NUMBER_OF_LIGHTS, statusOnLight);
  // gui = new BareGui((LightPtr *)lights, NUMBER_OF_LIGHTS, statusOnLight);

  context->reset(0);

  gui->init();

  gui->printIntro();

  while (true) {
    usleep(50000);

    auto now = std::chrono::system_clock::now();

    int elapsedTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();

    context->clockTick(elapsedTimeMs);

    int ch = getch();

    if (ch == 'q') {
      delete[] lights;
      cleanupAndExit();
    } else if (ch != ERR) {
      handleInput(ch, context, elapsedTimeMs);
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

void handleInput(int ch, Context *context, int elapsedTimeMs) {
  switch(ch) {
  case ' ':
    context->buttonPressed(elapsedTimeMs);
    break;
  }
}

int secondsSinceEpoch() {
  auto start = std::chrono::system_clock::now();

  return std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();
}
