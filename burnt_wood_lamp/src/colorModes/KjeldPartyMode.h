#pragma once

#include "ColorMode.h"

class KjeldPartyMode : public ColorMode {
public:
	uint16 Update( CRGB *leds_out, uint16 startLED, uint16 ledCount ) override {
		uint16 endLed = startLED + ledCount;

		// TODO
		int NUM_LEDS = 200;
		CRGB colors[5] = {
				CRGB( 255, 0, 0 ),
				CRGB( 0, 255, 0 ),
				CRGB( 0, 0, 255 ),
				CRGB( 0, 0, 0 ),
				CRGB( 255, 200, 0 ),
		};

		if ( !init ) {
			for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
				leds_out[ i ] = colors[ 0 ];
			}
			init = true;
		}

		for ( int _ = 0; _ < NUM_LEDS / 3; _++ ) {
			uint8_t led_index = random8( 0, NUM_LEDS );
			uint8_t color_index = random8( 0, 5 );
			leds_out[ led_index ] = colors[ color_index ];
		}

		return 500;
	}

private:
	bool init = false;
};
