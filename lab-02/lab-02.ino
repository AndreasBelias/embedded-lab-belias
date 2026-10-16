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

const int redLedPin = 4;
const int yellowLedPin = 7;
const int greenLedPin = 8;

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

    Serial.println("GREEN");
    digitalWrite(greenLedPin, HIGH);

    stateStartTime = millis();
}

void loop() {
    bool emergencyPressed = buttonPressed(emergencyButtonPin, emergencyButton);

    // Emergency button handling
    if (emergencyPressed) {
        currentState = RED;
        Serial.println("EMERGENCY! -> RED");
        digitalWrite(greenLedPin, LOW);
        digitalWrite(yellowLedPin, LOW);
        digitalWrite(redLedPin, HIGH);

        stateStartTime = millis();
    }

    // Traffic light FSM
    switch (currentState) {
        case GREEN:
            if((millis() - stateStartTime) >= greenDuration ){
              currentState = YELLOW;
              Serial.println("YELLOW");
              digitalWrite(greenLedPin, LOW);
              digitalWrite(yellowLedPin, HIGH);
              stateStartTime = millis();
            }
            break;

        case YELLOW:
            if((millis() - stateStartTime) >= yellowDuration ){
              currentState = RED;
              Serial.println("RED");
              digitalWrite(yellowLedPin, LOW);
              digitalWrite(redLedPin, HIGH);
              stateStartTime = millis();
            }
            break;

        case RED:
            // A pending press also blocks green until debouncing completes.
            if (emergencyButton.lastRawState == HIGH &&
                emergencyButton.stableState == HIGH &&
                (millis() - stateStartTime) >= redDuration) {
              currentState = GREEN;
              Serial.println("GREEN");
              digitalWrite(redLedPin, LOW);
              digitalWrite(greenLedPin, HIGH);
              stateStartTime = millis();
            }
            break;
    }
}
