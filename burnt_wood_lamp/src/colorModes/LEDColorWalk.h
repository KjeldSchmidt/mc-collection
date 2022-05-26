#pragma once

#include "ColorMode.h"

class LEDWalk : public ColorMode {
public:
	uint16 Update( CRGB *leds_out ) override {
		leds_out[ ( current_index + NUM_LEDS - 1 ) % NUM_LEDS ] = CRGB::Black;
		current_index = ( current_index + 1 ) % NUM_LEDS;
		leds_out[ current_index ] = CRGB::White;
		return 100;
	}

	constexpr static const char *getName() {
		return "LEDWalk";
	}

private:
	int current_index = 0;
};
