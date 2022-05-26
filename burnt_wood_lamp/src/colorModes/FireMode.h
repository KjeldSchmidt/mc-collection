#pragma once

#include "ColorMode.h"

class FireMode : public ColorMode {
public:
	uint16 Update( CRGB *leds_out ) override {
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