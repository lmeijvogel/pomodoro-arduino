#ifndef CONTEXT_CPP
#define CONTEXT_CPP

#include "State.hpp"

class Context {
  public:
    Context(State *waitingForPomodoroState, State *pomodoroState, State *pomodoroDoneState, State *waitingForBreakState, State *breakState, State *breakDoneState) {
      this->waitingForPomodoroState = waitingForPomodoroState;
      this->pomodoroState = pomodoroState;
      this->pomodoroDoneState = pomodoroDoneState;
      this->waitingForBreakState = waitingForBreakState;
      this->breakState = breakState;
      this->breakDoneState = breakDoneState;

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

    void setStateForTests(State *state) {
      this->currentState = state;
    }

    State *stateForTests() {
      return this->currentState;
    }

    std::string currentStateName() {
      return this->currentState->name();
    }

  private:
    State *currentState;
    State *waitingForPomodoroState;
    State *pomodoroState;
    State *pomodoroDoneState;
    State *waitingForBreakState;
    State *breakState;
    State *breakDoneState;

    int buttonPressCounter = 0;

    void transitionTo(State *newState, long currentTimeMillis) {
      this->currentState = newState;
      this->currentState->reset(currentTimeMillis);
    }

    void currentStateFinished(long currentTimeMillis) {
      if (this->currentState == this->breakState) {
        this->transitionTo(this->breakDoneState, currentTimeMillis);
      } else if (this->currentState == this->breakDoneState) {
        this->transitionTo(this->waitingForPomodoroState, currentTimeMillis);
      } else if (this->currentState == this->pomodoroState) {
        this->transitionTo(this->pomodoroDoneState, currentTimeMillis);
      } else if (this->currentState == this->pomodoroDoneState) {
        this->transitionTo(this->waitingForBreakState, currentTimeMillis);
      } else if (this->currentState == this->pomodoroDoneState) {
        this->transitionTo(this->waitingForBreakState, currentTimeMillis);
      }

    }
};

#endif
