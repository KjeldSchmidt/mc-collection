#pragma once

#include "ColorMode.h"

class SingleColorBlink : public ColorMode {
public:
	SingleColorBlink( const CRGB &color, uint16 blink_time ):
		color( color ),
		blink_time( blink_time ) {}

	uint16 Update( CRGB *leds_out, uint16 startLED, uint16 ledCount ) override {
		uint16 endLed = startLED + ledCount;

		// TODO
		int NUM_LEDS = 200;
		static bool on = true;
		for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
			if ( on ) {
				leds_out[ i ] = color;
			} else {
				leds_out[ i ] = CRGB::Black;
			}
		}

		on = !on;

		return blink_time;
	}

private:
	CRGB color;
	uint16 blink_time;
};