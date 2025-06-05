#include "ctrlib.h"


int temperature {20};
String MOTOR_STATE = "OFF"; // Variable to store the motor state
String ALARM_BUZZER_STATE = "ON"; // Variable to store the alarm buzzer state
String START_BUTTON_STATE = "OFF"; // Variable to store the start button state
String STOP_BUTTON_STATE = "ON"; // Variable to store the stop button state

String CYD_START_BUTTON_STATE = "OFF";
String CYD_STOP_BUTTON_STATE = "OFF";

float slaveValues[NUM_THRESHOLDS];
int UART_SYS_STATUS = 0; // Variable to store the system status

void abacha_shredder_setup()
{
  Serial.begin(9600);         // Debug via USB
  Serial1.begin(115200);      // UART communication to slave (PA9/PA10)
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(ALARM_BUZZER_PIN, OUTPUT);
  digitalWrite(MOTOR_PIN, LOW);
  digitalWrite(ALARM_BUZZER_PIN, LOW);
  delay(2000);
  Serial.println("STM32 Master ready.");
}

void control_motor(int state) 
{
  digitalWrite(MOTOR_PIN, state);
  MOTOR_STATE = (state == MOTOR_ON) ? "ON" : "OFF"; // Update motor state
}

void start_motor() {
  control_motor(MOTOR_ON);
}

void stop_motor() {
  control_motor(MOTOR_OFF);
}

void alarm_buzzer(int state) {
  digitalWrite(PB13, state);
  ALARM_BUZZER_STATE = (state == ALARM_BUZZER_ON) ? "ON" : "OFF"; // Update alarm buzzer state
}

void start_alarm() {
  alarm_buzzer(ALARM_BUZZER_ON);
}

void stop_alarm() {
  alarm_buzzer(ALARM_BUZZER_OFF);
}

void sysOP()
{
    // This function is meant to control the system operation
    if (digitalRead(START_BUTTON_PIN) == LOW || CYD_START_BUTTON_STATE == "ON") {
        start_motor();
        START_BUTTON_STATE = "ON";
        delay(1000); // Debounce delay
    } else if (digitalRead(STOP_BUTTON_PIN) == LOW || CYD_STOP_BUTTON_STATE == "ON") {
        stop_motor();
        STOP_BUTTON_STATE = "ON";
        delay(1000); // Debounce delay
    } else {
        START_BUTTON_STATE = "OFF";
        STOP_BUTTON_STATE = "OFF";
    }
}

int system_guard()
{
    // This function is meant to send a "warning" to the screen when the Motor temperature is high and a "danger" when the temperature is too high
    if (temperature > WARNING_STATUS_TEMP) {
        //Serial.println("Warning: High temperature detected!");
        return (WARNING_STATUS + 1);
    } else if (temperature > DANGER_STATUS_TEMP) {
       // Serial.println("Danger: Critical temperature reached!");
        return (DANGER_STATUS + 2);
    } else {
        //Serial.println("Temperature is normal.");
        return (NORMAL_STATUS + 3);
    }
}

void master_uart_send() {
 // === Send to Slave ===
  String message = String(MSG_HEADER) + "," +
                   String(temperature) + "," +
                   MOTOR_STATE + "," +
                   ALARM_BUZZER_STATE + "," +
                   START_BUTTON_STATE + "," +
                   STOP_BUTTON_STATE + "," +
                   String(UART_SYS_STATUS) + "," +
                   MSG_TERMINATOR;

  Serial1.print(message);
  Serial.print("Sent to slave: ");
  Serial.println(message);

  // === Receive from Slave ===
  static String input = "";

  while (Serial1.available()) {
    char c = Serial1.read();

    if (input.length() == 0 && c != MSG_HEADER) {
      continue;
    }

    input += c;

    if (c == MSG_TERMINATOR) {
      // Remove trailing newline
      input.remove(input.length() - 1);

      // Remove header
      if (input.charAt(0) == MSG_HEADER) {
        input.remove(0, 1);
      }

      char inputBuffer[INPUT_BUFFER_SIZE];
      input.toCharArray(inputBuffer, sizeof(inputBuffer));

      char* token = strtok(inputBuffer, ",");
      int idx = 0;

      while (token != NULL) {
        if (idx == 0) CYD_START_BUTTON_STATE = String(token);
        else if (idx == 1) CYD_STOP_BUTTON_STATE = String(token);
        token = strtok(NULL, ",");
        idx++;
      }

      Serial.print("Received from slave: Start=");
      Serial.print(CYD_START_BUTTON_STATE);
      Serial.print(", Stop=");
      Serial.println(CYD_STOP_BUTTON_STATE);

      input = ""; // Reset buffer
    }
  }
}

void cyd_motor_OP()
{
  if (CYD_START_BUTTON_STATE == "ON")
  {
    start_motor();
    for(int i {}; i<4; i++)
    {
      start_alarm();
      delay(1000);
      stop_alarm();
    }
    
  }
}