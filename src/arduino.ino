// the setup function runs once when you press reset or power the board
#include "LedLight.cpp"
#include "PhysicalButton.cpp"

#include "Context.cpp"
#include "CountdownState.cpp"
#include "BreakState.cpp"
#include "WaitingState.cpp"
#include "PomodoroState.cpp"

const int LED_PINS[] = { 8, 9, 10, 11, 12 };
const int BUTTON_PINS[] = { 13 };
typedef Light* LightPtr;

const int NUMBER_OF_LEDS = 5;

const unsigned long POMODORO_DURATION_IN_MINUTES = 25L;
const unsigned long BREAK_DURATION_IN_MINUTES = 5L;

void pollButtons();
PhysicalButton *button;

void readButtons(Context *context, unsigned long currentTimeMs);
/* void showStatus(State state); */

LedLight *statusLight;

LedLight *statusOnLight;

Context *context;
PomodoroState *pomodoroState;
BlinkingState *pomodoroDoneState;
BlinkingState *breakDoneState;
BreakState *breakState;
WaitingState *waitingForBreakState;
WaitingState *waitingForPomodoroState;

boolean previousButtonPressed = false;

void setup() {
  Serial.begin(9600);

  Serial.println("Started!");

  statusOnLight = new LedLight(LED_BUILTIN);

  LightPtr *lights = new LightPtr[NUMBER_OF_LEDS];

  for (int i = 0 ; i < NUMBER_OF_LEDS ; i++) {
    lights[i] = new LedLight(LED_PINS[i]);
  }

  long pomodoroDurationMillis = POMODORO_DURATION_IN_MINUTES * 60 * 1000;
  long breakDurationMillis = BREAK_DURATION_IN_MINUTES * 60 * 1000;

  pomodoroState = new PomodoroState(pomodoroDurationMillis, (LightPtr *)lights, NUMBER_OF_LEDS);
  pomodoroDoneState = new BlinkingState("pomodoroDone", 2500, 5, (LightPtr *)lights, NUMBER_OF_LEDS);
  breakDoneState = new BlinkingState("breakDone", 2500, 5, (LightPtr *)lights, NUMBER_OF_LEDS);
  breakState = new BreakState(breakDurationMillis, (LightPtr *)lights, NUMBER_OF_LEDS);
  waitingForBreakState = new WaitingState("waitingForBreak", "01010", (LightPtr *)lights, NUMBER_OF_LEDS);
  waitingForPomodoroState = new WaitingState("waitingForPomodoro", "00100", (LightPtr *)lights, NUMBER_OF_LEDS);

  context = new Context(waitingForPomodoroState, pomodoroState, pomodoroDoneState, waitingForBreakState, breakState, breakDoneState);
  context->reset(millis());

  button = new PhysicalButton(BUTTON_PINS[0]);
}

// the loop function runs over and over again forever
void loop() {
  unsigned long now = millis();

  pollButtons();
  readButtons(context, now);

  context->clockTick(now);
}

void pollButtons() {
  button->clockTick();
}

void readButtons(Context *context, unsigned long currentTimeMs ) {
  boolean buttonPressed = button->isPressed();

  if (!previousButtonPressed && buttonPressed) {
    context->buttonPressed(currentTimeMs);

    char buffer[100];
    sprintf(buffer, "New state: %s", context->currentStateName());
    Serial.println(buffer);
  }

  previousButtonPressed = buttonPressed;
}
