#include <Arduino.h>
#include "ctrlib.h"

HardwareSerial Serial2 (9600, PA10, PA9);


void setup() 
{
 abacha_system_setup();
}

void loop() 
{
  master_uart_send();
}
