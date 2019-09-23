#include <Arduino.h>
#include <FastLED.h>

#define RED_PIN A0
#define GREEN_PIN A1
#define BLUE_PIN A4

#define NUM_LEDS 16
#define DATA_PIN 9

CRGB leds[NUM_LEDS];

CRGB color;

uint8_t redReading = 0;
uint8_t greenReading = 0;
uint8_t blueReading = 0;

int mode = 0;
int prevMode = 0;

void setup() {
    pinMode(RED_PIN, INPUT);
    pinMode(GREEN_PIN, INPUT);
    pinMode(BLUE_PIN, INPUT);
    delay(1000);
    FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
}

void loop() {
    redReading = analogRead(RED_PIN) / 4;
    greenReading = analogRead(GREEN_PIN) / 4;
    blueReading = analogRead(BLUE_PIN) / 4;
    color = CRGB(greenReading, redReading, blueReading);

    fill_solid( &(leds[0]), NUM_LEDS, color);
    FastLED.show();

    delay(200);
}