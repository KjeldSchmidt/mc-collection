#include <Arduino.h>
#include <FastLED.h>

#define REDPIN A1
#define GREENPIN A0
#define BLUEPIN A3

#define NUM_LEDS 16
#define DATA_PIN 9

CRGB leds[NUM_LEDS];

CRGB color1;

uint8_t redReading = 0;
uint8_t greenReading = 0;
uint8_t blueReading = 0;

void setup() {
    Serial.begin(9600);
    FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
}

void loop() {
    redReading = analogRead(REDPIN) / 4;
    greenReading = analogRead(GREENPIN) / 4;
    blueReading = analogRead(BLUEPIN) / 4;

    color1 = CRGB(redReading, greenReading, blueReading);

    for (int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = color1;
    }
    FastLED.show();
}