#ifndef BREAK_DONE_STATE_CPP
#define BREAK_DONE_STATE_CPP

#include "BlinkingState.cpp"

class BreakDoneState : public BlinkingState {
  public:
    BreakDoneState(long durationMillis, int numberOfBlinks, LightPtr* lights, int numberOfLights): BlinkingState(durationMillis, numberOfBlinks, lights, numberOfLights) { }

    virtual std::string name() {
      return "break done";
    }
};

#endif
