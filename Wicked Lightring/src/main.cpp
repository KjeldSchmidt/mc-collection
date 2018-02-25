#include <Arduino.h>
#include <FastLED.h>

#define RED_PIN A1
#define GREEN_PIN A0
#define BLUE_PIN A3

#define SWITCH_PIN 4

#define NUM_LEDS 16
#define DATA_PIN 9

CRGB leds[NUM_LEDS];

CRGB color1;

uint8_t redReading = 0;
uint8_t greenReading = 0;
uint8_t blueReading = 0;

int mode = 0;
int prevMode = 0;

void setup() {
    pinMode(RED_PIN, INPUT);
    pinMode(GREEN_PIN, INPUT);
    pinMode(BLUE_PIN, INPUT);
    pinMode(SWITCH_PIN, OUTPUT);
    Serial.begin(9600);
    delay(1000);
    FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
}

void loop() {
    prevMode = mode;
    mode = digitalRead( SWITCH_PIN );
    Serial.println(mode, DEC);
    delay(200);

    redReading = analogRead(RED_PIN) / 4;
    greenReading = analogRead(GREEN_PIN) / 4;
    blueReading = analogRead(BLUE_PIN) / 4;
    color1 = CRGB(redReading, greenReading, blueReading);

    for (int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = color1;
    }
    FastLED.show();
}