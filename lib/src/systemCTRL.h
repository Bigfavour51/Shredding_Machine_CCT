#ifndef __STSTEMCTRL_H__
#define __STSTEMCTRL_H__

#include <Arduino.h>


#define BUTTON_PIN        PB4
#define MOTOR_RELAY_PIN   PA0
#define HOPPER_LIMIT_SWITCH_PIN PA15
#define Buzzer_PIN PB13 


extern bool relayState; // Initial state of the relay (motor off)
extern bool lastButtonState ;
extern bool hopperOpenPrev;          // To avoid repeated OFF commands
extern bool lastHopperState ;
extern unsigned long lastHopperDebounceTime;
extern const unsigned long debounceDelay;

void system_setup();
void toggleMotor();
void checkHopperLimitSwitch();
;

void readHopper();
void readButton();


#endif // __STSTEMCTRL_H__