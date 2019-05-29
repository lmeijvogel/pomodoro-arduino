// the setup function runs once when you press reset or power the board
#include "LightController.h"
#include "LedLight.h"
#include "StateMachine.h"
#include "PhysicalButton.h"

const int LED_PINS[] = { 8, 9, 10, 11 };
typedef LightController* LightControllerPtr;
typedef Light* LightPtr;

const int NUMBER_OF_LIGHTS = 8;
const int NUMBER_OF_LEDS = 4;

const int TOTAL_NUMBER_OF_LIGHTS = NUMBER_OF_LIGHTS + NUMBER_OF_LEDS;

const int NUMBER_OF_BUTTONS = 2;

unsigned long startTime;

LightControllerPtr *createLightControllers(LightPtr *lights, int count, RandomGenerator *randomGenerator);

void pollButtons();
void readButtons(StateMachine *stateMachine, unsigned long currentTimeMs);
void showStatus(State state);

StateMachine *stateMachine;

LedLight *statusLight;

LedLight *statusOnLight;

boolean previousOffButtonPressed = false;

void setup() {
  Serial.begin(9600);

  Serial.println("Started!");

  statusOnLight = new LedLight(LED_BUILTIN);

  startTime = millis();

  RandomGenerator randomGenerator(startTime);

  LightPtr *lights = new LightPtr[TOTAL_NUMBER_OF_LIGHTS];

  for (int i = 0 ; i < NUMBER_OF_LEDS ; i++) {
    lights[NUMBER_OF_LIGHTS + i] = new LedLight(LED_PINS[i]);
  }

  LightControllerPtr *lightControllers = createLightControllers((LightPtr *)lights, TOTAL_NUMBER_OF_LIGHTS);

  stateMachine = new StateMachine(statusLedController);
}

// the loop function runs over and over again forever
void loop() {
  unsigned long now = millis();

  pollButtons();
  readButtons(stateMachine, now);

  stateMachine->clockTick(now);

  showStatus(stateMachine->getState());
}

LightControllerPtr *createLightControllers(LightPtr *lights, int count, RandomGenerator *randomGenerator) {
  LightControllerPtr *lightControllers = new LightControllerPtr[count];

  for (int i = 0 ; i < count ; i++) {
    LightController *lightController = new LightController(lights[i], randomGenerator, 5000, 3000);
    lightControllers[i] = lightController;
  }

  return lightControllers;
}

void pollButtons() {
  // TODO
}

void readButtons(StateMachine *stateMachine, unsigned long currentTimeMs ) {
  /* boolean offButtonPressed = offButton->isPressed(); */
  /* boolean gradualButtonPressed = gradualButton->isPressed(); */

  /* if (offButtonPressed && !previousOffButtonPressed) { */
    /* stateMachine->switchOff(); */
  /* } */

  /* previousOffButtonPressed = offButtonPressed; */
}

void showStatus(State state) {
  if (state == StateOff) {
    statusLight->turnOff();
  } else {
    statusLight->turnOn();
  }
}
