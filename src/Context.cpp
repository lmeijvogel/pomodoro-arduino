#ifndef CONTEXT_CPP
#define CONTEXT_CPP

#include "State.hpp"
#include "WaitingState.cpp"
#include "CountdownState.cpp"
#include "PomodoroState.cpp"
#include "WaitingState.cpp"
#include "BreakState.cpp"

#include "Logger.cpp"

class Context {
  public:
    Context(WaitingState *waitingForPomodoroState, CountdownState *pomodoroStartedState, PomodoroState *pomodoroState, CountdownState *pomodoroDoneState, WaitingState *waitingForBreakState, CountdownState *breakStartedState, BreakState *breakState, CountdownState *breakDoneState) {
      this->waitingForPomodoroState = waitingForPomodoroState;
      this->pomodoroStartedState = pomodoroStartedState;
      this->pomodoroState = pomodoroState;
      this->pomodoroDoneState = pomodoroDoneState;
      this->waitingForBreakState = waitingForBreakState;
      this->breakStartedState = breakStartedState;
      this->breakState = breakState;
      this->breakDoneState = breakDoneState;

      this->currentState = waitingForPomodoroState;
      // this->currentState = waitingForBreakState;
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
      Logger::print("Button pressed! Current state: ");
      Logger::print(this->currentState->name());

      this->buttonPressCounter++;

      if (this->currentState == this->waitingForPomodoroState) {
        this->transitionTo(this->pomodoroStartedState, currentTimeMillis);
      } else if (this->currentState == this->pomodoroState) {
        this->transitionTo(waitingForBreakState, currentTimeMillis);
      } else if (this->currentState == this->waitingForBreakState) {
        this->transitionTo(breakStartedState, currentTimeMillis);
      } else if (this->currentState == this->breakState) {
        this->transitionTo(waitingForPomodoroState, currentTimeMillis);
      } else {
        Logger::println("ERROR! No state transition found!");
      }

      Logger::print(", new state: ");
      Logger::println(this->currentState->name());
    }

    void setStateForTests(State *state) {
      this->currentState = state;
    }

    State *stateForTests() {
      return this->currentState;
    }

    const char* currentStateName() {
      return this->currentState->name();
    }

  private:
    State *currentState;
    State *waitingForPomodoroState;
    State *pomodoroStartedState;
    State *pomodoroState;
    State *pomodoroDoneState;
    State *waitingForBreakState;
    State *breakStartedState;
    State *breakState;
    State *breakDoneState;

    int buttonPressCounter = 0;

    void transitionTo(State *newState, long currentTimeMillis) {
      this->currentState = newState;
      this->currentState->reset(currentTimeMillis);
    }

    void currentStateFinished(long currentTimeMillis) {
      Logger::print("Current state finished! Current state: ");
      Logger::println(this->currentState->name());

      State *newState = NULL;

      if      (this->currentState == this->breakState)           { newState = this->breakDoneState;          }
      else if (this->currentState == this->breakStartedState)    { newState = this->breakState;              }
      else if (this->currentState == this->breakDoneState)       { newState = this->waitingForPomodoroState; }
      else if (this->currentState == this->pomodoroStartedState) { newState = this->pomodoroState;           }
      else if (this->currentState == this->pomodoroState)        { newState = this->pomodoroDoneState;       }
      else if (this->currentState == this->pomodoroDoneState)    { newState = this->waitingForBreakState;    }
      else {
        Logger::println("ERROR! No state transition found!");
      }

      if (newState) {
        this->transitionTo(newState, currentTimeMillis);
        Logger::print(", new state: ");
        Logger::println(newState->name());
      }
    }
};

#endif
