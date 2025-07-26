#include "systemCTRL.h"

void setup() {
  // Initialize the system
  system_setup();
}

void loop() {
  // Continuously check the button and hopper limit switch
  toggleMotor();
  checkHopperLimitSwitch();

  
  
}
