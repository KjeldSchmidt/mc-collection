#pragma once

#include "ColorMode.h"

class DualColor : public ColorMode {
public:
	DualColor( const CRGB &colorUpper, const CRGB &colorLower )
			: color_upper( colorUpper ),
			  color_lower( colorLower ) {}

	uint16 Update( CRGB *leds_out ) override {
		for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
			if ( i < NUM_LEDS_LOWER ) {
				leds_out[ i ] = color_lower;
			} else {
				leds_out[ i ] = color_upper;
			}
		}
		return 10000;
	}

	constexpr static const char *getName() {
		return "DualColor( Color, Color )";
	}

private:
	CRGB color_upper;
	CRGB color_lower;
};