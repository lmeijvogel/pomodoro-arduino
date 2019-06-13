// the setup function runs once when you press reset or power the board
#include "LedLight.cpp"
#include "PhysicalButton.cpp"

#include "Context.cpp"
#include "CountdownState.cpp"
#include "BreakState.cpp"
#include "WaitingState.cpp"
#include "PomodoroState.cpp"
#include "PatternedBlinkingState.cpp"

const int LED_PINS[] = { 8, 9, 10, 11, 12 };
const int BUTTON_PINS[] = { 13 };
typedef Light* LightPtr;

const int NUMBER_OF_LEDS = 5;

const unsigned long POMODORO_DURATION_IN_MINUTES = 25L;
const unsigned long BREAK_DURATION_IN_MINUTES = 5L;

const char *pomodoroBlinkingPattern[] = {
    "00100", "00010", "00001", "10000",
    "01000", "00100", "00010", "00001",
    "10001", "01001", "00101", "00011",
    "10011", "01011", "00111", "10111",

    "10111", "01111", "01111", "11111" };

const char *breakBlinkingPattern[] = {
    "01010", "00101", "00010", "10001",
    "01000", "10100", "01010", "00101",
    "00010", "00001", "10001", "11001",
    "11101", "11111", "11110", "11100",

    "11000", "10000", "10000", "10000",
    "01000", "01000", "00100", "00010",
    "00001", "00001", "00010", "00010",
    "00010", "00100", "00100", "00100",

    "00100", "00100", "01000", "01000",
    "01000", "01000", "01000", "01000",
    "01000", "00100", "00100", "00100",
    "00100", "00100", "00010", "00010",

    "00010", "00001", "00000" };

const long BLINKING_PATTERN_STATE_DURATION = 1500;
const int NUMBER_OF_POMODORO_BLINKING_PATTERNS = 20;
const int NUMBER_OF_BREAK_BLINKING_PATTERNS = 51;
const int NUMBER_OF_POMODORO_PATTERN_REPETITIONS = 1;
const int NUMBER_OF_BREAK_PATTERN_REPETITIONS = 1;

void pollButtons();
PhysicalButton *button;

void readButtons(Context *context, unsigned long currentTimeMs);

LedLight *statusLight;

LedLight *statusOnLight;

LightPtr *lights;

Context *context;
PatternedBlinkingState *pomodoroStartedState;
PomodoroState *pomodoroState;
BlinkingState *pomodoroDoneState;
PatternedBlinkingState *breakStartedState;
BreakState *breakState;
BlinkingState *breakDoneState;
WaitingState *waitingForBreakState;
WaitingState *waitingForPomodoroState;

boolean previousButtonPressed = false;

void setup() {
  Serial.begin(9600);

  Serial.println("Started!");

  statusOnLight = new LedLight(LED_BUILTIN);

  lights = new LightPtr[NUMBER_OF_LEDS];

  for (int i = 0 ; i < NUMBER_OF_LEDS ; i++) {
    lights[i] = new LedLight(LED_PINS[i]);
  }

  long pomodoroDurationMillis = POMODORO_DURATION_IN_MINUTES * 60 * 1000;
  long breakDurationMillis = BREAK_DURATION_IN_MINUTES * 60 * 1000;

  pomodoroStartedState = new PatternedBlinkingState(BLINKING_PATTERN_STATE_DURATION, (LightPtr *)lights, NUMBER_OF_LEDS, "pomodoroStarted", pomodoroBlinkingPattern, NUMBER_OF_POMODORO_BLINKING_PATTERNS, NUMBER_OF_POMODORO_PATTERN_REPETITIONS);
  pomodoroState = new PomodoroState(pomodoroDurationMillis, (LightPtr *)lights, NUMBER_OF_LEDS);
  pomodoroDoneState = new BlinkingState("pomodoroDone", 2500, 5, (LightPtr *)lights, NUMBER_OF_LEDS);
  breakStartedState = new PatternedBlinkingState(BLINKING_PATTERN_STATE_DURATION, (LightPtr *)lights, NUMBER_OF_LEDS, "breakStarted", breakBlinkingPattern, NUMBER_OF_BREAK_BLINKING_PATTERNS, NUMBER_OF_BREAK_PATTERN_REPETITIONS);
  breakState = new BreakState(breakDurationMillis, (LightPtr *)lights, NUMBER_OF_LEDS);
  breakDoneState = new BlinkingState("breakDone", 2500, 5, (LightPtr *)lights, NUMBER_OF_LEDS);
  waitingForBreakState = new WaitingState("waitingForBreak", "01010", (LightPtr *)lights, NUMBER_OF_LEDS);
  waitingForPomodoroState = new WaitingState("waitingForPomodoro", "00100", (LightPtr *)lights, NUMBER_OF_LEDS);

  context = new Context(waitingForPomodoroState, pomodoroStartedState, pomodoroState, pomodoroDoneState, waitingForBreakState, breakStartedState, breakState, breakDoneState);
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
