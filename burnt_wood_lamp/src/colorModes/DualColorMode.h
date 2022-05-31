#pragma once

#include "ColorMode.h"

class DualColorDrift : public ColorMode {
public:
	uint16 Update( CRGB *leds_out ) override {
		for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
			if ( i < NUM_LEDS_LOWER ) {
				leds_out[ i ] = lower;
			} else {
				leds_out[ i ] = upper;
			}
		}
		drift();
		return 0;
	}

	constexpr static const char *getName() {
		return "DualColorDrift";
	}

private:
	void drift() {
		uint8_t upper_drift_target = random8( 2 );
		if ( upper[ upper_drift_target ] == MAX_BRIGHTNESS ) {
			upper_drift_target = ( upper_drift_target + 1 ) % 3;
		}
		uint8_t upper_drift_source = ( upper_drift_target + 1 ) % 3;
		uint8_t lower_drift_target = random8( 2 );
		if ( lower[ lower_drift_target ] == MAX_BRIGHTNESS ) {
			lower_drift_target = ( lower_drift_target + 1 ) % 3;
		}
		uint8_t lower_drift_source = ( lower_drift_target + 1 ) % 3;

		upper[ upper_drift_target ]++;
		upper[ upper_drift_source ]--;
		lower[ lower_drift_target ]++;
		lower[ lower_drift_source ]--;
	}

	CRGB upper = CRGB( MAX_BRIGHTNESS, 0, 0 );
	CRGB lower = CRGB( 0, MAX_BRIGHTNESS, 0 );
};
