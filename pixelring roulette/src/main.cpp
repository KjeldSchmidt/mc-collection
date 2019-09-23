#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 16
#define DATA_PIN 9

CRGBArray<NUM_LEDS> leds;
int initialDelay = 5;
int maxDelay = 1200;
int pos = 0;

void colorAllImmediate( CRGB color ) {
    for (int i = 0; i < NUM_LEDS; ++i ) {
        leds[i] = color; 
    }
    FastLED.show();
}

void colorAll( CRGB color ) {
    for (int i = 0; i < NUM_LEDS; ++i ) {
        leds[i] = color; 
    }
}

CRGB yellow = CRGB(35, 25, 0);
CRGB red = CRGB(60, 0, 0);
CRGB green = CRGB(0, 60, 0);

void setup() {
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    randomSeed(analogRead(0));
    int maxDelay = random(600, 1500);
    colorAll( green );
    leds[0] = red;
    FastLED.show();
}

void showSucess() {
    colorAll( green );
}

void showFail() {
    colorAll( red );
}

void roll() {
    pos = random(0, NUM_LEDS);
    colorAll( yellow );
    leds[pos] = red;
    FastLED.show();
}

void loop() {
    if ( initialDelay < maxDelay ) {
        initialDelay += initialDelay/5;
        roll();
        delay(initialDelay);
    } else {
        colorAll( green );
        leds[pos] = red;
        FastLED.show();
    }
}