#ifndef CONTEXT_CPP
#define CONTEXT_CPP

#include "CounterState.hpp"

class Context {
  public:
    Context(CounterState *waitingForPomodoroState, CounterState *pomodoroState, CounterState *waitingForBreakState, CounterState *breakState) {
      this->waitingForPomodoroState = waitingForPomodoroState;
      this->pomodoroState = pomodoroState;
      this->waitingForBreakState = waitingForBreakState;
      this->breakState = breakState;

      this->currentState = waitingForPomodoroState;
    }

    void reset(long currentTimeMillis) {
      this->currentState->reset(currentTimeMillis);
    }

    void clockTick(long currentTimeMillis) {
      this->currentState->clockTick(currentTimeMillis);

      if (currentState->isFinished(currentTimeMillis)) {
        this->currentStateFinished(currentTimeMillis);
      }
    }

    void buttonPressed(long currentTimeMillis) {
      this->buttonPressCounter++;

      if (this->currentState == this->waitingForPomodoroState) {
        this->transitionTo(this->pomodoroState, currentTimeMillis);
      } else if (this->currentState == this->pomodoroState) {
        this->transitionTo(waitingForBreakState, currentTimeMillis);
      } else if (this->currentState == this->waitingForBreakState) {
        this->transitionTo(breakState, currentTimeMillis);
      } else if (this->currentState == this->breakState) {
        this->transitionTo(waitingForPomodoroState, currentTimeMillis);
      }
    }

    void setStateForTests(CounterState *state) {
      this->currentState = state;
    }

    CounterState *stateForTests() {
      return this->currentState;
    }

    std::string currentStateName() {
      return this->currentState->name();
    }

  private:
    CounterState *currentState;
    CounterState *waitingForPomodoroState;
    CounterState *pomodoroState;
    CounterState *waitingForBreakState;
    CounterState *breakState;

    int buttonPressCounter = 0;

    void transitionTo(CounterState *newState, long currentTimeMillis) {
      this->currentState = newState;
      this->currentState->reset(currentTimeMillis);
    }

    void currentStateFinished(long currentTimeMillis) {
      if (this->currentState == this->breakState) {
        this->transitionTo(this->waitingForPomodoroState, currentTimeMillis);
      } else if (this->currentState == this->pomodoroState) {
        this->transitionTo(this->waitingForBreakState, currentTimeMillis);
      }
    }
};

#endif
