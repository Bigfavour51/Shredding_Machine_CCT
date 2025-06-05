#include <Arduino.h>
#include "ctrlib.h"
#include <SPI.h>
#include "MAX6675.h"
#include <HardwareSerial.h>

// Define the pins for the MAX6675 (change these to match your wiring)
#define MAX6675_SCK   PA5
#define MAX6675_CS    PA4
#define MAX6675_SO    PA6

MAX6675 thermocouple(MAX6675_SCK, MAX6675_CS, MAX6675_SO);
int read_temperature(); // Function prototype to read temperature

void setup() 
{
  abacha_shredder_setup(); // Initialize the system
}

void loop() 
{
 master_uart_send(); // Send data to the slave
 cyd_motor_OP();
 delay(1000); // Delay for 1 second before the next loop iteration
}

int read_temperature() 
{  // This function reads the temperature from the MAX6675 thermocouple
  int status = thermocouple.read();
  if (status != STATUS_OK)
  {
   return 404;
  }
    temperature = thermocouple.getCelsius();
    if(!isnan(temperature)) {
       
        return temperature;
    } else {
        return {};
    }

}