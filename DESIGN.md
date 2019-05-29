* Goal

Create a pomodoro timer with a few lights:

A pomodoro is 25 minutes, so show 5 "timing lights": 1 for every 5 minutes. Start with all lights on and gradually turn off the lights.

A typical rest break is 5 minutes. Use the same lights, but "in reverse": 1 light for every minute. Start with all lights off, turn them on one by one.

* Shopping list

An arduino Uno shield: https://www.kiwi-electronics.nl/adafruit-proto-shield-for-arduino-kit-stackable-version-r3

* High level design:

** Main programme, initializes the lights and buttons.

** A Button to manually transition between states, e.g. starting a Pomodoro.

** StateMachine to register current state (since we have one button):

State: Waiting for Pomodoro
  Button press: Transition to state Pomodoro

State: Pomodoro
  Button press: Transition to state Pomodoro Finished
  Transitions to state Pomodoro Finished

State: Pomodoro Finished (Flashes all lights x times)
  Button press: Transition to state Waiting For Break
  Transitions to Waiting For Break

State: Waiting For Break
  Button press: Transition to state Break

State: Break
  Button press: Transition to state Break Finished
  Transitions to state Break Finished

State press: Break Finished
  Button press: Transition to state Waiting For Pomodoro
  Transitions to: Transition to state Waiting For Pomodoro

** Use the State pattern for handling current state:

```
class State {
  public:
    /* Example constructor */
    State(long totalDuration, Light lights[], int numberOfLights);

    /* Resets the clock and `isFinished` and sets the lights to their first configuration
    void start();

    void clockTick(long currentTimeMillis);

    bool isFinished();
}
```

With States for:
- Nothing (does not do anything -- Used for state 'Waiting For Pomodoro' and 'Waiting For Break'),
- Pomodoro (counts down from 25)
- Break state (counts down)
- Blinking all (Signals end of Pomodoro/Break)

A State knows whether it's finished, but will be asked by the main loop? If finished, will trigger an event in the StateMachine
(Something like 'StateFinished') -- This makes handling all states (the Finished states that just blink a couple of times, as
well as the countdown states) work the same.

** A status LED to signify that the device is currently waiting (red) since there won't be any lights burning at that time.
