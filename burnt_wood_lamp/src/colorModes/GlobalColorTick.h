#pragma once

#include "ColorMode.h"

class GlobalColorTick : public ColorMode {
public:
	uint16 Update( CRGB *leds_out, uint16 startLED, uint16 ledCount ) override {
		uint16 endLed = startLED + ledCount;
		uint8_t r = 8 * ( mode & 4u );
		uint8_t g = 8 * ( mode & 2u );
		uint8_t b = 8 * ( mode & 1u );
		CRGB c{ r, g, b };
		for ( uint8_t i = startLED; i < endLed; i++ ) {
			leds_out[ i ] = c;
		}
		mode %= 7;
		mode += 1;

		return 1500;
	}

	constexpr static const char *getName() {
		return "ColorTick";
	}

private:
	uint8_t mode = 1;
};