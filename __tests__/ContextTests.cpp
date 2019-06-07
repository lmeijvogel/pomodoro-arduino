#include <string>

#include "catch.hpp"
#include "../src/Context.cpp"
#include "../src/State.hpp"

class MockState : public State {
  public:
    bool receivedReset = false;
    bool _isFinished = false;

    long lastTick = 0;

    MockState(const char *name) {
      this->_name = name;
    }

    virtual void reset(long) {
      this->receivedReset = true;
    }

    virtual void clockTick(long currentTimeMillis) {
      this->lastTick = currentTimeMillis;
    }

    virtual bool isFinished(long) {
      return this->_isFinished;
    }

    virtual const char *name() {
      return this->_name;
    }

  private:
    const char * _name;
};

typedef MockState* MockStatePtr;

MockState waitingForPomodoro("waitingForPomodoro");
MockState pomodoro("pomodoro");
MockState waitingForBreak("waitingForBreak");
MockState breakState("break");

Context context(&waitingForPomodoro, &pomodoro, &waitingForBreak, &breakState);

TEST_CASE("It starts in the Waiting state") {
  REQUIRE(context.stateForTests()->name() == waitingForPomodoro.name());
}

TEST_CASE("It propagates clockTick to the current state") {
  context.clockTick(2000);

  REQUIRE(waitingForPomodoro.lastTick == 2000);
}

// ========================= buttonPressed ================================
//
TEST_CASE("It transitions from waitingForPomodoro to Pomodoro if button is pressed") {
  context.buttonPressed(2000);

  REQUIRE(context.stateForTests()->name() == pomodoro.name());

  REQUIRE(pomodoro.receivedReset);
}

TEST_CASE("It transitions from pomodoro to waitingForBreak if button is pressed") {
  context.setStateForTests(&pomodoro);

  context.buttonPressed(2000);

  REQUIRE(context.stateForTests()->name() == waitingForBreak.name());
}

TEST_CASE("It transitions from waitingForBreak to break if button is pressed") {
  context.setStateForTests(&waitingForBreak);

  context.buttonPressed(2000);

  REQUIRE(context.stateForTests()->name() == breakState.name());
}

TEST_CASE("It transitions from break to waitingForPomodoro if button is pressed") {
  context.setStateForTests(&breakState);

  context.buttonPressed(2000);

  REQUIRE(context.stateForTests()->name() == waitingForPomodoro.name());
}

// ========================= isFinished ================================

TEST_CASE("It transitions from break to waitingForPomodoro if state is finished after clock tick") {
  context.setStateForTests(&breakState);

  breakState._isFinished = true;

  context.clockTick(2000);

  REQUIRE(context.stateForTests()->name() == waitingForPomodoro.name());
}

TEST_CASE("It transitions from pomodoro to waitingForBreak if state is finished after clock tick") {
  context.setStateForTests(&pomodoro);

  pomodoro._isFinished = true;

  context.clockTick(2000);

  REQUIRE(context.stateForTests()->name() == waitingForBreak.name());
}
