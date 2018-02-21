#include <Arduino.h>
#include <FastLED.h>
#define NUM_LEDS 2
#define DATA_PIN 3

CRGBArray<NUM_LEDS> leds;

void setup() { FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); }

void loop(){ 
    // Turn the first led red for 1 second
    leds[0] = CRGB::Red; 
    FastLED.show();
    delay(1000);
    
    // Set the first led back to black for 1 second
    leds[1] = CRGB::Blue;
    FastLED.show();
    delay(1000);

    // Turn the first led red for 1 second
    leds[0] = CRGB::Black; 
    FastLED.show();
    delay(1000);
    
    // Set the first led back to black for 1 second
    leds[1] = CRGB::Black;
    FastLED.show();
    delay(1000);
}
