//
// Created by kjeld on 25/11/2020.
//

#ifndef BURNT_WOOD_LAMP_COLORMODE_H
#define BURNT_WOOD_LAMP_COLORMODE_H

#include "FastLED.h"


class ColorMode {
public:
	virtual uint16 Update( CRGB *leds_out ) = 0;
};

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
};

class CityAtSundown : public ColorMode {
public:
	uint16 Update( CRGB *leds_out ) override {
		CRGB upper = CRGB( MAX_BRIGHTNESS - 3, 3, 0 );
		CRGB lower = CRGB( MAX_BRIGHTNESS - 18, 18, 0 );
		for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
			if ( i < NUM_LEDS_LOWER ) {
				leds_out[ i ] = lower;
			} else {
				leds_out[ i ] = upper;
			}
		}
		return 10000;
	}
};

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

class LEDWalk : public ColorMode {
public:
	uint16 Update( CRGB *leds_out ) override {
		leds_out[ ( current_index + NUM_LEDS - 1 ) % NUM_LEDS ] = CRGB::Black;
		current_index = ( current_index + 1 ) % NUM_LEDS;
		leds_out[ current_index ] = CRGB::White;
		return 100;
	}

private:
	int current_index = 0;
};

class GlobalColorTick : public ColorMode {
public:
	uint16 Update( CRGB *leds_out ) override {
		uint8_t r = 8 * ( mode & 4u );
		uint8_t g = 8 * ( mode & 2u );
		uint8_t b = 8 * ( mode & 1u );
		CRGB c{ r, g, b };
		for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
			leds_out[ i ] = c;
		}
		mode %= 7;
		mode += 1;

		return 1500;
	}

private:
	uint8_t mode = 1;
};

class ColorPulse : public ColorMode {
public:
	uint16 Update( CRGB *leds_out ) override {
		unsigned long time = millis();
		float position = (float) time * speed_of_light;
		float red_pos = position * red_waves;
		float green_pos = position * green_waves;
		float blue_pos = position * blue_waves;
		for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
			leds_out[ i ].red = calc( red_waves, (float) i - red_pos, 0 );
			leds_out[ i ].green = calc( green_waves, (float) i - green_pos, PI / 2 );
			leds_out[ i ].blue = calc( blue_waves, (float) i - blue_pos, PI );
		}

		return 0;
	}

private:
	unsigned char calc( int freq, float pos, float offset ) {
		return (char) (( sin( pos * ( PI - offset ) * freq * 2 / NUM_LEDS ) * MAX_BRIGHTNESS_PER_CHANNEL ) +
		               ( MAX_BRIGHTNESS_PER_CHANNEL + 1 )) / 2;
	}

	constexpr static uint8_t red_waves = 5;
	constexpr static uint8_t green_waves = 7;
	constexpr static uint8_t blue_waves = 3;
	constexpr static float speed_of_light = 0.001; // In arbitrary units
};

class SingleColor : public ColorMode {
public:
	explicit SingleColor( const CRGB &color ) : color( color ) {}

	uint16 Update( CRGB *leds_out ) override {
		for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
			leds_out[ i ] = color;
		}
		return 10000;
	}

	CRGB color;
};

#include "ArduinoBorealis.h"

#endif //BURNT_WOOD_LAMP_COLORMODE_H
