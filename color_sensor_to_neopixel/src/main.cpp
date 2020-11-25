/*
 * Pins:
 *
 * GND OE S1 S0
 * VCC OUT S2 S3
 *
 * Connecting GND and VCC always makes the LEDs light up.
 * S0, S1 select output frequency scaling
 * H H 100%
 * H L 20%
 * L H 2%
 * L L Power Down
 *
 * S2, S3 select which color to measure
 * H H Green
 * H L White
 * L H Blue
 * L L Red
 *
 * OE ist output enable. As far as I understand, if nothing is connected, it behaves as if pulled high. If pulled high,
 * the sensor works as expected and sends a signal trough OUT. If pulled low, it doesn't send a signal. Thus, many
 * sensors can be used on the same pin via multiplexing.
 */

#include "Arduino.h"
#include "FastLED.h"


#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

#define NUM_LEDS 16
#define LED_DATA_PIN 9

CRGBArray<NUM_LEDS> leds;

void colorAll( uint8_t r, uint8_t g, uint8_t b) {
    for ( int i = 0; i < NUM_LEDS; ++i ) {
        leds[i] = CRGB( r, g, b );
    }
    FastLED.show();
}

void setup() {
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    // Setting frequency-scaling to 20%
    digitalWrite(S0,HIGH);
    digitalWrite(S1,LOW);

    CFastLED::addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);

    Serial.begin(9600);
}
void loop() {
    digitalWrite(S2,LOW);
    digitalWrite(S3,LOW);
    uint8_t green = pulseIn(sensorOut, LOW);

    digitalWrite(S2,HIGH);
    digitalWrite(S3,HIGH);
    uint8_t red = pulseIn(sensorOut, LOW);

    digitalWrite(S2,LOW);
    digitalWrite(S3,HIGH);
    uint8_t blue = pulseIn(sensorOut, LOW);

    colorAll(red, green, blue);

    delay(100);
}
