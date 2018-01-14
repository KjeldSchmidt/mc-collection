#include <Arduino.h>
#include <FastLED.h>

#define REDPIN A0
#define GREENPIN A1
#define BLUEPIN A3

unsigned char red1 = 0;
unsigned char green1 = 0;
unsigned char blue1 = 0;

void setup() {
    Serial.begin(9600);
}

void loop() {
    red1 = analogRead(REDPIN) / 4;
    green1 = analogRead(GREENPIN) / 4;
    blue1 = analogRead(BLUEPIN) / 4;
    Serial.print("Red: ");
    Serial.println(red1, DEC);
    Serial.print("Green: ");
    Serial.println(green1, DEC);
    Serial.print("Blue: ");
    Serial.println(blue1, DEC);
    delay(1000);
}