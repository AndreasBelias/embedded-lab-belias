# Traffic Light with Emergency Button

Arduino Uno traffic light controller developed for the Embedded Systems laboratory at the University of the Peloponnese. The circuit is simulated in Wokwi.

## Operation

The controller uses a finite state machine and `millis()` to repeat the following sequence without blocking execution:

**Green (4 s) → Yellow (1 s) → Red (4 s) → Green**

Pressing the emergency button switches to red as soon as the input is read and restarts the red timer. Red remains on while the button is held. Normal operation resumes once the button has been released for 40 ms and at least 4 seconds have elapsed since the last accepted press.

Debouncing is applied to the release, allowing an immediate response to a press while preventing repeated events from contact bounce. State changes and emergency events are printed to the Serial Monitor at **9600 baud**. Only one LED is active at a time.

## Wiringv

| Component | Arduino pin | Connection |
| --- | --- | --- |
| Red LED | D4 | Through a 220 Ω resistor; cathode to GND |
| Yellow LED | D7 | Through a 220 Ω resistor; cathode to GND |
| Green LED | D10 | Through a 220 Ω resistor; cathode to GND |
| Emergency button | D2 | Between D2 and GND, using `INPUT_PULLUP` |

## Build and Simulation

With Arduino CLI and the `arduino:avr` core installed, run from the repository root:

```sh
arduino-cli compile --fqbn arduino:avr:uno --output-dir lab-02/build lab-02
```

Open `lab-02.ino` in VS Code and run **Wokwi: Start Simulator**. The `wokwi.toml` configuration loads the compiled firmware from `build/`. Rebuild after changing the sketch.

## Verification

- Observe three complete cycles without pressing the button.
- Press the button during green and confirm the immediate switch to red.
- Press five times, allowing at least 40 ms of release between presses, and confirm red remains active.
- Hold the button for more than 4 seconds and confirm red remains on until release.

## PWM Experiment

The green LED was moved from D8 to PWM-capable D10. Replacing both green `digitalWrite(..., HIGH)` calls with `analogWrite(greenLedPin, 30)` produces a duty cycle of approximately 12%, reducing its brightness while the state sequence continues normally.

Normal operation uses `digitalWrite(greenLedPin, HIGH)` again. The PWM calls remain commented out for reference, and the LED remains on D10.

## Author

Andreas Belias  
Department of Informatics and Telecommunications, University of the Peloponnese
