#pragma once

#include "ColorMode.h"

class SingleColor : public ColorMode {
public:
	explicit SingleColor( const CRGB &color ) : color( color ) {}

	uint16 Update( CRGB *leds_out, uint16 startLED, uint16 ledCount) override {
		uint16 endLed = startLED + ledCount;
		for ( uint8_t i = startLED; i < endLed; i++ ) {
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