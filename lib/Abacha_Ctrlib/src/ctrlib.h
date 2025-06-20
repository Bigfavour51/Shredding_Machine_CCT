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
- Implementation of a system guide that will send a "Warning" or "Danger" message to the CYD screen when the temperature of the motor exceeds a certain threshold 
  and also trigger the alarm buzzer.
*/


#ifndef __CTRLIB__
#define __CTRLIB__

// Included the necessary libraries
#include <Arduino.h>
#include <SPI.h>
#include "MAX31855.h"


// Define the pins for the MAX6675 thermocouple
#define MAX6675_CS_PIN  5// PA4 Chip select pin for the MAX6675
#define MAX6675_SCK_PIN 6 //  PA5 Serial clock pin for the MAX6675 
#define MAX6675_MISO_PIN 4 //PB14 Master in slave out pin for the MAX6675
// Define the pins for the motor control
#define MOTOR_PIN  7// PB4 Pin for controlling the motor
#define START_BUTTON_PIN 8  // PB3Pin for the start button
#define STOP_BUTTON_PIN  9//PA8 Pin for the stop button

#define MOTOR_ON HIGH // Relay ON state
#define MOTOR_OFF LOW // Relay OFF state

// Define the pin for the Alarm Buzzer
#define ALARM_BUZZER_PIN 2  // PB13 Pin for the alarm buzzer
#define ALARM_BUZZER_ON HIGH // Buzzer ON state 
#define ALARM_BUZZER_OFF LOW // Buzzer OFF state
#define BUZZER_DELAY 1000 // Duration for the alarm buzzer in milliseconds



void abacha_system_setup();
int read_temperature();
void control_motor(int state);
void start_motor();
void stop_motor();
void alarm_buzzer(int state);
void start_alarm();
void stop_alarm();
void master_uart_send();
int system_guard();
extern void sysOP();


extern int temperature;
extern String MOTOR_STATE ; // Variable to store the motor state
extern String ALARM_BUZZER_STATE; // Variable to store the alarm buzzer state
extern String START_BUTTON_STATE; // Variable to store the start button state
extern String STOP_BUTTON_STATE; // Variable to store the stop button state



extern String CYD_START_BUTTON_STATE;
extern String CYD_STOP_BUTTON_STATE;


#define NUM_THRESHOLDS 2
#define INPUT_BUFFER_SIZE 64
#define MSG_HEADER '#' // optional: use for identifying start of command
#define MSG_TERMINATOR '\n'
extern float slaveValues[];


#define WARNING_STATUS_TEMP 50 // Variable to store the warning status
#define DANGER_STATUS_TEMP 70 // Variable to store the critical status

#define WARNING_STATUS 0
#define DANGER_STATUS 0
#define NORMAL_STATUS 0
extern int UART_SYS_STATUS;


#endif