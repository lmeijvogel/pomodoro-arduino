#ifndef POMODORO_DONE_STATE_CPP
#define POMODORO_DONE_STATE_CPP

#include "BlinkingState.cpp"

class PomodoroDoneState : public BlinkingState {
  public:
    PomodoroDoneState(long durationMillis, int numberOfBlinks, LightPtr* lights, int numberOfLights): BlinkingState(durationMillis, numberOfBlinks, lights, numberOfLights) { }

    virtual std::string name() {
      return "pomodoro done";
    }
};

#endif
