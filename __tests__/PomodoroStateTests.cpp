#include "catch.hpp"
#include "../src/PomodoroState.cpp"
#include "../src/Light.hpp"

class MockLight : public Light {
public:
  virtual void turnOn();
  virtual void turnOff();

  bool isOn = false;
};

void MockLight::turnOn() {
  isOn = true;
}

void MockLight::turnOff() {
  isOn = false;
}

typedef MockLight* MockLightPtr;

void createLights(MockLightPtr *mockLights);

MockLightPtr mockLights[5];

void testLights(MockLightPtr *mockLights, bool expected0,
    bool expected1,
    bool expected2,
    bool expected3,
    bool expected4);

TEST_CASE("When it resets, it turns on all the lights") {
  createLights(mockLights);

  PomodoroState pomodoroState(5000, (LightPtr *)mockLights, 5);

  pomodoroState.reset(0);

  for (int i = 0 ; i < 5 ; i++) {
    MockLightPtr mockLight = mockLights[i];

    REQUIRE(mockLight->isOn);
  }
}

TEST_CASE("It transitions to turning the first light off") {
  createLights(mockLights);

  PomodoroState pomodoroState(5000, (LightPtr *)mockLights, 5);

  pomodoroState.reset(0);

  pomodoroState.clockTick(1000);
  testLights(mockLights, true, true, true, true, true);
  pomodoroState.clockTick(2000);
  testLights(mockLights, true, true, true, true, false);
  pomodoroState.clockTick(3000);
  testLights(mockLights, true, true, true, false, false);
  pomodoroState.clockTick(4000);
  testLights(mockLights, true, false, false, false, false);
  pomodoroState.clockTick(5000);
  testLights(mockLights, true, false, false, false, false);

  REQUIRE(pomodoroState.isFinished(5000));
}

void createLights(MockLightPtr *mockLights) {
  for (int i = 0 ; i < 5 ; i++) {
    MockLight *light = new MockLight();

    mockLights[i] = light;
  }
}

void testLights(MockLightPtr *mockLights,
    bool expected0,
    bool expected1,
    bool expected2,
    bool expected3,
    bool expected4) {
  REQUIRE(mockLights[0]->isOn == expected0);
  REQUIRE(mockLights[1]->isOn == expected1);
  REQUIRE(mockLights[2]->isOn == expected2);
  REQUIRE(mockLights[3]->isOn == expected3);
  REQUIRE(mockLights[4]->isOn == expected4);
}

