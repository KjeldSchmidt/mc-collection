#include <Arduino.h>
#include <FastLED.h>

#define RED_PIN A0
#define GREEN_PIN A3
#define BLUE_PIN A5

#define NUM_LEDS 30
#define DATA_PIN 9

#define AWAKE_DELAY_TIME 30
#define SIGNIFICANT_CHANGE_THRESH 3

CRGB leds[NUM_LEDS];

CRGB color;

uint8_t rawRedReading = 0;
uint8_t rawGreenReading = 0;
uint8_t rawBlueReading = 0;
uint8_t prevRedReading = 0;
uint8_t prevGreenReading = 0;
uint8_t prevBlueReading = 0;
long lastSignificantChangeTime = 0;

int mode = 0;
int prevMode = 0;

void setup() {
    pinMode(RED_PIN, INPUT);
    pinMode(GREEN_PIN, INPUT);
    pinMode(BLUE_PIN, INPUT);
    delay(1000);
    FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
}


uint8_t logarithmatize( uint8_t input ) {
    if (input < 2) {
        return 0;
    }
    uint8_t t = (input & 31) + 32;
    uint8_t u = input >> 5;
    return u < 5 ? t >> (5 - u) : t << (u - 5);
}

uint8_t abs_difference( uint8_t val_1, uint8_t val_2 ) {
    return val_1 > val_2 ? val_1 - val_2 : val_2 - val_1;
}

bool input_changed_significantly() {
    return ( abs_difference( rawRedReading, prevRedReading ) > SIGNIFICANT_CHANGE_THRESH 
        || abs_difference( rawGreenReading, prevGreenReading ) > SIGNIFICANT_CHANGE_THRESH
        || abs_difference( rawBlueReading, prevBlueReading ) > SIGNIFICANT_CHANGE_THRESH
        || rawRedReading == 0 || rawBlueReading == 0 || rawGreenReading == 0
    );
}

void loop() {
    rawRedReading = analogRead(RED_PIN) / 4;
    rawGreenReading = analogRead(GREEN_PIN) / 4;
    rawBlueReading = analogRead(BLUE_PIN) / 4;
    if ( input_changed_significantly() ) {
        color = CRGB( logarithmatize( rawGreenReading ), logarithmatize( rawRedReading ), logarithmatize( rawBlueReading ) );
    }

    fill_solid( &(leds[0]), NUM_LEDS, color);
    FastLED.show();

    delay( AWAKE_DELAY_TIME );
    prevRedReading = rawRedReading;
    prevGreenReading = rawGreenReading;
    prevBlueReading = rawBlueReading;
}