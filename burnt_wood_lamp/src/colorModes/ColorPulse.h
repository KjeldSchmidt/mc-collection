#pragma once

#include "ColorMode.h"

class ColorPulse : public ColorMode {
public:
	uint16 Update( CRGB *leds_out, uint16 startLED, uint16 ledCount) override {
		uint16_t endLed = startLED + ledCount;
		unsigned long time = millis();
		float position = (float) time * speed_of_light;
		float red_pos = position * red_waves;
		float green_pos = position * green_waves;
		float blue_pos = position * blue_waves;
		for ( uint8_t i = startLED; i < endLed; i++ ) {
			leds_out[ i ].red = calc( red_waves, (float) i - red_pos, 0, ledCount );
			leds_out[ i ].green = calc( green_waves, (float) i - green_pos, PI / 2, ledCount );
			leds_out[ i ].blue = calc( blue_waves, (float) i - blue_pos, PI, ledCount );
		}

		return 0;
	}

	constexpr static const char *getName() {
		return "ColorPulse";
	}

private:
	unsigned char calc( int freq, float pos, float offset, uint16_t ledCount ) {
		return (char) (( sin( pos * ( PI - offset ) * freq * 2 / ledCount ) * MAX_BRIGHTNESS_PER_CHANNEL ) +
		               ( MAX_BRIGHTNESS_PER_CHANNEL + 1 )) / 2;
	}

	constexpr static uint8_t red_waves = 5;
	constexpr static uint8_t green_waves = 7;
	constexpr static uint8_t blue_waves = 3;
	constexpr static float speed_of_light = 0.001; // In arbitrary units
};