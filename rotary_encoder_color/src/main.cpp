#include <Arduino.h>
#include "FastLED.h"
#include "../.pio/libdeps/nanoatmega168/FastLED/src/pixeltypes.h"


#define NUM_LEDS 16
#define CHIPSET WS2812
#define COLOR_ORDER GRB
#define DATA_PIN 8

CRGB leds[NUM_LEDS];

const byte aInterruptPin = 2;
const byte bInterruptPin = 3;

volatile byte aTriggered;
volatile byte bTriggered;
volatile int16_t totalTurned = 0;

void aInterrupt() {
	aTriggered = !aTriggered;
	if ( aTriggered == bTriggered ) {
		totalTurned--;
	}
}

void bInterrupt() {
	bTriggered = !bTriggered;
	if ( aTriggered == bTriggered ) {
		totalTurned++;
	}
}

void setup() {
	Serial.begin(9600);
	Serial.println("Awake!");
	CFastLED::addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>( leds, NUM_LEDS );
	pinMode(aInterruptPin, INPUT_PULLUP);
	pinMode(bInterruptPin, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(aInterruptPin), aInterrupt, CHANGE);
	attachInterrupt(digitalPinToInterrupt(bInterruptPin), bInterrupt, CHANGE);
	for (auto & led : leds) {
		led = CRGB(1, 0, 0);
	}
	FastLED.show();
}

void loop() {
	Serial.println(totalTurned);
}