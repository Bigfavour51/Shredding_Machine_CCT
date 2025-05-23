#include <Arduino.h>
#include "ctrlib.h"

#define led PC13



void setup() {
  abacha_system_setup(); // Initialize the system
  pinMode(led, OUTPUT); // Set the LED pin as output
}


void loop() {

  digitalWrite(led, HIGH); // Turn the LED on
  delay(1000); // Wait for a second
  digitalWrite(led, LOW); // Turn the LED off
  delay(1000); // Wait for a second
  read_temperature(); // Read the temperature
  sysOP(); // Control the system operation
  UART_SYS_STATUS = system_guard(); // Check the system status
  master_uart_send(); // Send data to the master device
  // Optional: Add a delay for stability
  delay(1000);

}