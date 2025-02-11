#include "FastLED.h"

#define XSTR(x) STR(x)
#define STR(x) #x

CRGB leds[NUM_LEDS];

void turnOffBuiltInLed() {
    pinMode( D4, OUTPUT );
    digitalWrite( D4, HIGH );
}

void setup() {
    turnOffBuiltInLed();
    CFastLED::addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>( leds, NUM_LEDS );

    randomSeed( analogRead( A0 ));
    Serial.begin( 74880 );
    Serial.println();
    Serial.print("Chipset: "); Serial.println(XSTR(CHIPSET));
    Serial.print("NUM_LEDS: "); Serial.println(NUM_LEDS);
}


void loop() {
    Serial.println("");
    Serial.println("Start Sequence");
    Serial.println("Black");
    for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
        leds[ i ] = CRGB::Black;
    }
    FastLED.show();
    delay(1000);
    Serial.println("White");
    for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
        leds[ i ] = CRGB(50, 50, 50);
    }
    FastLED.show();
    delay(1000);
}