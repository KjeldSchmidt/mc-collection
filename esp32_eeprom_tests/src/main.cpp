#include <Arduino.h>
#include <EEPROM.h>

uint8_t val;

void setup() {
  EEPROM.begin(128);
  Serial.begin(9600);

  delay(2000);
}

void loop() {
  delay(1000);
  val = EEPROM.read(0);
  Serial.println(val);
  val = 32;
  Serial.println(val);

  delay(2000);
  EEPROM.write(0, 64);
  EEPROM.commit();
  Serial.println("I attempted to write!");

  delay(3000);
}