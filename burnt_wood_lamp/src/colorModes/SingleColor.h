#pragma once

#include "ColorMode.h"

class SingleColor : public ColorMode {
public:
	explicit SingleColor( const CRGB &color ) : color( color ) {}

	uint16 Update( CRGB *leds_out ) override {
		for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
			leds_out[ i ] = color;
		}
		return 10000;
	}

	constexpr static const char *getName() {
		return "SingleColor( Color )";
	}

private:
	CRGB color;
};