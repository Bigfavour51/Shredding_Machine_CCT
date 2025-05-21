
#ifndef __CTRLIB__
#define __CTRLIB__
/*
SCOPE OF THE LIBRARY:
This is a library for the control cct for an Abacha_Shredding Machine. Some funtionalities that will be
added to this library are:
- Control of the Motor that will be used to shred the Abacha via Push buttons (i.e Start and Stop)
- control of a Temperature sensor (k-TYPE Thermocouplen + MAX6675 driver)that will be used to monitor the temperature of the Motor and in essence the machine 
- Display of the temperature on an ESP32 2.8 inch CYD screen
- Control of the switching relay that will be used to turn on and off the motor from the CYD screen
- Implemtation of UART communication between the CYD MCU (ESP32) and the main MCU (STM32F103C8T6)
- Implementation of I2C communication between the CYD MCU (ESP32) and the main MCU (STM32F103C8T6) ------ ***optional***
*/

// Included the necessary libraries
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <max6675.h>


// Define the pins for the MAX6675 thermocouple
#define MAX6675_CS_PIN PA4 // Chip select pin for the MAX6675
#define MAX6675_SCK_PIN PA5 // Serial clock pin for the MAX6675 
#define MAX6675_MISO_PIN PB14 // Master in slave out pin for the MAX6675
// Define the pins for the motor control
#define MOTOR_PIN PB4 // Pin for controlling the motor
#define START_BUTTON_PIN PB3 // Pin for the start button
#define STOP_BUTTON_PIN PA8 // Pin for the stop button

#define MOTOR_ON HIGH // Relay ON state
#define MOTOR_OFF LOW // Relay OFF state



void system_setup();







#endif