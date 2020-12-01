//
// Created by kjeld on 25/11/2020.
//

#ifndef BURNT_WOOD_LAMP_COLORMODE_H
#define BURNT_WOOD_LAMP_COLORMODE_H

#include "FastLED.h"


class ColorMode {
public:
	virtual uint16 Update( CRGB *leds_out, uint8 num_leds ) = 0;
};

class FireMode : public ColorMode {
public:
	uint16 Update( CRGB *leds_out, uint8_t num_leds ) override {
		CRGB fire = CRGB( 20, 5, 0 );
		CRGB value = leds_out[ 0 ];
		long r = random( 40 );
		value += fire;
		value -= CRGB( r, r / 4, 0 );
		for ( uint8_t i = 0; i < num_leds; i++ ) {
			leds_out[ i ] = value;
		}
		return 50;
	}
};

class LEDWalk : public ColorMode {
public:
	uint16 Update( CRGB *leds_out, uint8 num_leds ) override {
		leds_out[ ( current_index + num_leds - 1 ) % num_leds ] = CRGB::Black;
		current_index = ( current_index + 1 ) % num_leds;
		leds_out[ current_index ] = CRGB::White;
		return 100;
	}

private:
	int current_index = 0;
};

class GlobalColorTick : public ColorMode {
public:
	uint16 Update( CRGB *leds_out, uint8 num_leds ) override {
		uint8_t r = 8 * ( mode & 4u );
		uint8_t g = 8 * ( mode & 2u );
		uint8_t b = 8 * ( mode & 1u );
		CRGB c{ r, g, b };
		for ( uint8_t i = 0; i < num_leds; i++ ) {
			leds_out[ i ] = c;
		}
		mode += 1;
		mode %= 8;

		return 1500;
	}

private:
	uint8_t mode = 1;
};

#endif //BURNT_WOOD_LAMP_COLORMODE_H
