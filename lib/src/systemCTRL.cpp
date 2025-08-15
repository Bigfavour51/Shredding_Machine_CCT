// Arduino framework version: This project uses the Arduino framework version 2.0.0.
// Target board: This project is intended to run on an STM32 BluePill board, such as the Arduino BluePill F103C8T6.
/*
  Scope of the code:

  1. The code is a simple Arduino sketch that read a momentary button press
     and toggles the state of a Motor conntected via a relay.

  2. A limit switch is connected to the Hopper of an enclosure where the motor is residing.
     When the Hopper is opened the motor stops if previously on and running. Motor stays off if
     if Hopper is opened and motor isnt running. The Only condition to allow motor to run 
     is that the hopper is closed.

  3. Have efficient comments within the code and Serial communications to allow for proper debugging.

*/

#include "systemCTRL.h"

bool relayState {false};
bool lastButtonState {HIGH}; 
bool hopperOpenPrev {false}; 
bool lastHopperState {HIGH};
unsigned long lastHopperDebounceTime {};
const unsigned long debounceDelay{50};



void system_setup() 
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(MOTOR_RELAY_PIN, OUTPUT);
  digitalWrite(MOTOR_RELAY_PIN, HIGH); // Start with MOTOR OFF

  pinMode(HOPPER_LIMIT_SWITCH_PIN, INPUT_PULLUP); // Set limit switch pin as input with pull-up resistor
  pinMode(Buzzer_PIN, OUTPUT); // Set buzzer pin as output
  digitalWrite(Buzzer_PIN, LOW); // Start with buzzer OFF
  Serial.begin(9600);
  Serial.println("System Ready");
}

void toggleMotor() 
{
   bool buttonReading = digitalRead(BUTTON_PIN);
  bool isHopperClosed = digitalRead(HOPPER_LIMIT_SWITCH_PIN) == HIGH;

  // Debounce with crude delay
  if (buttonReading == LOW && lastButtonState == HIGH) {
    delay(50); // crude debounce
    if (isHopperClosed) {
      relayState = !relayState;
      digitalWrite(MOTOR_RELAY_PIN, relayState ? LOW : HIGH);
      if (relayState) {
        for(int i = 0; i <= 3; i++) {
          digitalWrite(Buzzer_PIN, HIGH);
          delay(200);
          digitalWrite(Buzzer_PIN, LOW);
          delay(200);
        }
      } else {
        digitalWrite(Buzzer_PIN, LOW); // Stop the buzzer when motor is OFF
      }
      Serial.println(relayState ? "Motor ON" : "Motor OFF");
    } 
    
    else {
      for (int i = 0; i <=3; i++) { // Beep 3 times if hopper is open
        digitalWrite(Buzzer_PIN, HIGH);
        delay(200);
        digitalWrite(Buzzer_PIN, LOW);
        delay(200);
      }
      digitalWrite(MOTOR_RELAY_PIN, HIGH); // Ensure motor is OFF
      relayState = false; // Reset relay state
      Serial.println("Cannot turn ON motor. Hopper is open.");
    }
  }

  lastButtonState = buttonReading;
}

void checkHopperLimitSwitch() {
  static bool lastStableState = HIGH;
  static bool lastReading = HIGH;
  static unsigned long lastDebounceTime = 0;

  bool currentReading = digitalRead(HOPPER_LIMIT_SWITCH_PIN);

  if (currentReading != lastReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (currentReading != lastStableState) {
      lastStableState = currentReading;

      if (currentReading == LOW) {
        if (relayState) {
          relayState = false;
          digitalWrite(MOTOR_RELAY_PIN, HIGH);
          Serial.println("Hopper OPENED. Motor turned OFF.");
        } else {
          Serial.println("Hopper OPENED. Motor already OFF.");
        }
      } else {
        Serial.println("Hopper CLOSED. Motor can now be turned ON.");
      }
    }
  }

  lastReading = currentReading;
}



// Note: Ensure that the HOPPER_LIMIT_SWITCH_PIN is connected to a normally open limit switch


