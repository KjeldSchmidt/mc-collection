#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "FastLED.h"

#include "ConfigVariables.h"

#define XSTR(x) STR(x)
#define STR(x) #x


// LEDs
CRGB leds[NUM_LEDS];

bool unhandledExceptionCaught = false;

void turnOffBuiltInLed() {
    pinMode( LED_BUILTIN, OUTPUT );
    digitalWrite( LED_BUILTIN, HIGH );
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
	delay(1000);
	Serial.println("White");
	for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
		leds[ i ] = CRGB(50, 50, 50);
	}
	delay(1000);
}