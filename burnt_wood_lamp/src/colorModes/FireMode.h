#pragma once

#include "ColorMode.h"

class FireMode : public ColorMode {
public:
	uint16 Update( CRGB *leds_out, uint16 startLED, uint16 ledCount ) override {
		uint16 endLed = startLED + ledCount;

		// TODO
		int NUM_LEDS = 200;
		CRGB fire = CRGB( 20, 5, 0 );
		for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
			CRGB value = leds_out[ i ];
			long r = random( 40 );
			value += fire;
			value -= CRGB( r, r / 4, 0 );
			leds_out[ i ] = value;
		}
		return 50;
	}

	constexpr static const char *getName() {
		return "FireMode";
	}
};