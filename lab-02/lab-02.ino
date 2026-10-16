#include <Arduino.h>

typedef struct ButtonState {
    int lastRawState;
    int stableState;
    unsigned long lastChangeTime;
} ButtonState;

// Function prototypes
bool buttonPressed(int pin, ButtonState &button);

// Pin configuration
const int emergencyButtonPin = 2;

const int redLedPin = 8;
const int yellowLedPin = 9;
const int greenLedPin = 10;

// Timing
const unsigned long debounceDelay = 40;

const unsigned long greenDuration = 4000;
const unsigned long yellowDuration = 1000;
const unsigned long redDuration = 4000;

unsigned long stateStartTime = 0;

// Finite State Machine
enum State {
    GREEN,
    YELLOW,
    RED
};

State currentState = GREEN;

// Button initialization
ButtonState emergencyButton = {HIGH, HIGH, 0};

// Button debouncing
bool buttonPressed(int pin, ButtonState &button) {
    int rawState = digitalRead(pin);

    if (rawState != button.lastRawState) {
        button.lastChangeTime = millis();
        button.lastRawState = rawState;
    }

    if (millis() - button.lastChangeTime >= debounceDelay) {
        if (rawState != button.stableState) {
            button.stableState = rawState;

            if (button.stableState == LOW) {
                return true;
            } else {
                return false;
            }
        }
    }

    return false;
}

void setup() {
    pinMode(emergencyButtonPin, INPUT_PULLUP);

    pinMode(redLedPin, OUTPUT);
    pinMode(yellowLedPin, OUTPUT);
    pinMode(greenLedPin, OUTPUT);

    Serial.begin(9600);

    digitalWrite(greenLedPin, HIGH);

    stateStartTime = millis();
}

void loop() {
    bool emergencyPressed = buttonPressed(emergencyButtonPin, emergencyButton);

    // Emergency button handling
    if(emergencyPressed){

    }

    // Traffic light FSM
    switch (currentState) {
        case GREEN:
            if((millis() - stateStartTime) >= greenDuration ){
              currentState = YELLOW;
              digitalWrite(greenLedPin, LOW);
              digitalWrite(yellowLedPin, HIGH);
              stateStartTime = millis();
            }
            break;

        case YELLOW:
            if((millis() - stateStartTime) >= yellowDuration ){
              currentState = RED;
              digitalWrite(yellowLedPin, LOW);
              digitalWrite(redLedPin, HIGH);
              stateStartTime = millis();
            }
            break;

        case RED:
            if((millis() - stateStartTime) >= redDuration ){
              currentState = GREEN;
              digitalWrite(redLedPin, LOW);
              digitalWrite(greenLedPin, HIGH);
              stateStartTime = millis();
            }
            break;
    }
}
