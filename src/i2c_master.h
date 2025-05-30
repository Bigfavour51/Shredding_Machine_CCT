#include <Wire.h>
#define SDA PB9
#define SCL PB8
#define I2C_ADDR 0x55

float temp = 32.4;

void i2c_master_setup() {
  Serial.begin(115200);
  delay(3000);
  Wire.setSDA(SDA);
  Wire.setSCL(SCL);
  Wire.begin();  // as master
}

void i2c_master_send() {
  delay(1000);
  Wire.beginTransmission(I2C_ADDR);
  Wire.write((uint8_t *)&temp, sizeof(temp));
  Wire.endTransmission();

  delay(10);
  if (Wire.requestFrom(I2C_ADDR, sizeof(int) + sizeof(bool))) {
    int count;
    bool wifi;
    Wire.readBytes((uint8_t *)&count, sizeof(count));
    Wire.readBytes((uint8_t *)&wifi, sizeof(wifi));
    Serial.printf("Count: %d, WiFi: %s\n", count, wifi ? "true" : "false");
  }
}
