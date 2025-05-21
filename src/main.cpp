#include <Arduino.h>
#include <ctrlib.h>






// Forward declaration of library functions
void abacha_system_setup();
void read_temperature();
void master_uart_send();
int system_guard();
void sysOP();

void setup() {
  abacha_system_setup(); // Initialize the system
}


void loop() {
  read_temperature(); // Read the temperature
  sysOP(); // Control the system operation
  int UART_SYS_STATUS = system_guard(); // Check the system status
  master_uart_send(); // Send data to the master device
  // Optional: Add a delay for stability
  delay(1000);

}