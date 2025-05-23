#include "ctrlib.h"



int temperature {};
String MOTOR_STATE = "OFF"; // Variable to store the motor state
String ALARM_BUZZER_STATE = "OFF"; // Variable to store the alarm buzzer state
String START_BUTTON_STATE = "OFF"; // Variable to store the start button state
String STOP_BUTTON_STATE = "OFF"; // Variable to store the stop button state


String CYD_START_BUTTON_STATE = "OFF";
String CYD_STOP_BUTTON_STATE = "OFF";

float slaveValues[NUM_THRESHOLDS];

int UART_SYS_STATUS = 0; // Variable to store the system status

MAX6675 thermocouple(MAX6675_SCK_PIN, MAX6675_MISO_PIN, MAX6675_CS_PIN);

void abacha_system_setup() {
  
  Serial.begin(9600);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(ALARM_BUZZER_PIN, OUTPUT);
  pinMode(START_BUTTON_PIN, INPUT_PULLUP);
  pinMode(STOP_BUTTON_PIN, INPUT_PULLUP);

    digitalWrite(MOTOR_PIN, MOTOR_OFF); // Ensure the motor is off at startup
    digitalWrite(ALARM_BUZZER_PIN, ALARM_BUZZER_OFF); // Ensure the alarm buzzer is off at startup
    Serial.println("System setup complete.");
    delay(1000); // Delay for stability
}

int read_temperature() {
  temperature = thermocouple.readCelsius();
    if(!isnan(temperature)) {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println(" °C");
        return temperature;
    } else {
        Serial.println("Error reading temperature.");
        return {};
    }
}

void control_motor(int state) {
  digitalWrite(MOTOR_PIN, state);
  Serial.print("Motor ");
  Serial.println(state == MOTOR_ON ? "ON" : "OFF");
  MOTOR_STATE = (state == MOTOR_ON) ? "ON" : "OFF"; // Update motor state
}

void start_motor() {
  control_motor(MOTOR_ON);
}

void stop_motor() {
  control_motor(MOTOR_OFF);
}

void alarm_buzzer(int state) {
  digitalWrite(ALARM_BUZZER_PIN, state);
  Serial.print("Alarm Buzzer ");
  Serial.println(state == ALARM_BUZZER_ON ? "ON" : "OFF");
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
        Serial.println("Warning: High temperature detected!");
        return (WARNING_STATUS + 1);
    } else if (temperature > DANGER_STATUS_TEMP) {
        Serial.println("Danger: Critical temperature reached!");
        return (DANGER_STATUS + 2);
    } else {
        Serial.println("Temperature is normal.");
        return (NORMAL_STATUS + 3);
    }
}

void master_uart_send() {
  String response = String('#') + "," + String(temperature) + "," + String(MOTOR_STATE) + "," +
                    String(ALARM_BUZZER_STATE) + "," + String(START_BUTTON_STATE) + "," +
                    String(STOP_BUTTON_STATE) + String(UART_SYS_STATUS) + "," +"\n";

  Serial.print(response); // send to slave


     String input = "";

    while (Serial.available()) {
        char c = Serial.read();

        // Optional: skip garbage before the start character
        if (input.length() == 0 && c != MSG_HEADER) {
            continue;
        }

        input += c;

        // Only process when newline is received
        if (c == MSG_TERMINATOR) {
            input.trim();

            // Strip off header if used
            if (input.charAt(0) == MSG_HEADER) {
                input.remove(0, 1);  // remove header
            }

            char inputBuffer[INPUT_BUFFER_SIZE];
            input.toCharArray(inputBuffer, sizeof(inputBuffer));

            char *token = strtok(inputBuffer, ",");
            int idx = 0;
            while (token != NULL && idx < NUM_THRESHOLDS) {
                slaveValues[idx++] = atof(token);
                token = strtok(NULL, ",");
            }

            if (idx == NUM_THRESHOLDS) {
                // Assign thresholds from master
                CYD_START_BUTTON_STATE = slaveValues[0];
                CYD_STOP_BUTTON_STATE = slaveValues[1];
                

            }
            // input = ""; // reset input buffer
        }
    }
  delay(1000); // Optional delay for stability
}

