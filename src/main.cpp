// #include "systemCTRL.h"

// void setup() 
// {
//   // Initialize the system
//   system_setup();
// } 

// void loop() 
// {
  
//   toggleMotor();
//   checkHopperLimitSwitch();
  
// }







// Test code for PB4 button → PB7 relay control (active-low)
#include <Arduino.h>
#define BUTTON_PIN PB4
#define RELAY_PIN  PB7
#define HOPPER_LIMIT_SWITCH_PIN PA15

bool relayOn = false;
bool lastButtonState = HIGH; // Released (due to INPUT_PULLUP)
const unsigned long debounceDelay = 50; // ms

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Button active-low
  pinMode(HOPPER_LIMIT_SWITCH_PIN, INPUT_PULLUP); // Hopper limit switch active-low
  pinMode(RELAY_PIN, OUTPUT);        // Relay output
  digitalWrite(RELAY_PIN, HIGH);     // Start with relay OFF (active-low)

  Serial.begin(9600);
  Serial.println("Relay Test Ready");
}

void loop() {
  bool buttonReading = digitalRead(BUTTON_PIN);
  bool isHopperClosed = digitalRead(HOPPER_LIMIT_SWITCH_PIN) == HIGH;

  // Button press detection (released → pressed)
  if (buttonReading == LOW && lastButtonState == HIGH) {
    delay(debounceDelay); // crude debounce
    if (digitalRead(BUTTON_PIN) == LOW) { // confirm still pressed
      relayOn = !relayOn;
      digitalWrite(RELAY_PIN, relayOn ? LOW : HIGH); // active-low control
      Serial.println(relayOn ? "Relay ON" : "Relay OFF");
    }
  }
  // Hopper limit switch check
  if (!isHopperClosed) {
    digitalWrite(RELAY_PIN, HIGH); // Ensure relay is OFF if hopper is open
    if (relayOn) {
      Serial.println("Hopper is open! Relay turned OFF.");
      relayOn = false; // Reset relay state
    }
  }

  lastButtonState = buttonReading;
}




