//
// Created by kjeld on 25/11/2020.
//

#ifndef BURNT_WOOD_LAMP_COLORMODE_H
#define BURNT_WOOD_LAMP_COLORMODE_H

#include "FastLED.h"
#include "ConfigVariables.h"


class ColorMode {
public:
	virtual uint16 Update( CRGB *leds_out ) = 0;

	virtual ~ColorMode() = default;

	constexpr static const char *getName() {
		return "ColorMode";
	}
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

	constexpr static const char *getName() {
		return "FireMode";
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

	constexpr static const char *getName() {
		return "ColorTick";
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

	constexpr static const char *getName() {
		return "ColorPulse";
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

	constexpr static const char *getName() {
		return "SingleColor( Color )";
	}

private:
	CRGB color;
};

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

class LightsOut : public SingleColor {
public:
	LightsOut() : SingleColor( CRGB::Black ) {}

	constexpr static const char *getName() {
		return "LightsOut";
	}
};

class WakeUp : public SingleColor {
public:
	WakeUp() : SingleColor(
			CRGB( MAX_BRIGHTNESS_PER_CHANNEL, MAX_BRIGHTNESS_PER_CHANNEL, MAX_BRIGHTNESS_PER_CHANNEL )
	) {}

	constexpr static const char *getName() {
		return "WakeUp";
	}
};

class DiscoStrobo : public ColorMode {
public:
	uint16 Update( CRGB *leds_out ) override {
		static bool on = true;
		for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
			if ( on ) {
				leds_out[ i ] = CRGB( MAX_BRIGHTNESS_PER_CHANNEL, MAX_BRIGHTNESS_PER_CHANNEL,
				                      MAX_BRIGHTNESS_PER_CHANNEL );
			} else {
				leds_out[ i ] = CRGB::Black;
			}
		}

		on = !on;

		return 50;
	}
};

class CityAtSundown : public DualColor {
public:
	CityAtSundown() : DualColor(
			CRGB( MAX_BRIGHTNESS - 3, 3, 0 ),
			CRGB( MAX_BRIGHTNESS - 18, 18, 0 )
	) {}

	constexpr static const char *getName() {
		return "CityAtSundown";
	}
};

class Pacifica : public ColorMode {
public:
	uint16 Update( CRGB *leds_out ) override {
		// Increment the four "color index start" counters, one for each wave layer.
		// Each is incremented at a different speed, and the speeds vary over time.
		static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
		static uint32_t sLastms = 0;
		uint32_t ms = GET_MILLIS();
		uint32_t deltams = ms - sLastms;
		sLastms = ms;
		uint16_t speedfactor1 = beatsin16( 3, 179, 269 );
		uint16_t speedfactor2 = beatsin16( 4, 179, 269 );
		uint32_t deltams1 = ( deltams * speedfactor1 ) / 256;
		uint32_t deltams2 = ( deltams * speedfactor2 ) / 256;
		uint32_t deltams21 = ( deltams1 + deltams2 ) / 2;
		sCIStart1 += ( deltams1 * beatsin88( 1011, 10, 13 ));
		sCIStart2 -= ( deltams21 * beatsin88( 777, 8, 11 ));
		sCIStart3 -= ( deltams1 * beatsin88( 501, 5, 7 ));
		sCIStart4 -= ( deltams2 * beatsin88( 257, 4, 6 ));

		// Clear out the LED array to a dim background blue-green
		fill_solid( leds_out, NUM_LEDS, CRGB( 2, 6, 10 ));

		// Render each of four layers, with different scales and speeds, that vary over time
		pacifica_one_layer(
				leds_out,
				pacifica_palette_1,
				sCIStart1,
				beatsin16( 3, 11 * 256, 14 * 256 ),
				beatsin8( 10, 70, 130 ),
				0 - beat16( 301 )
		);

		pacifica_one_layer(
				leds_out,
				pacifica_palette_2,
				sCIStart2,
				beatsin16( 4, 6 * 256, 9 * 256 ),
				beatsin8( 17, 40, 80 ),
				beat16( 401 )
		);

		pacifica_one_layer(
				leds_out,
				pacifica_palette_3,
				sCIStart3,
				6 * 256,
				beatsin8( 9, 10, 38 ),
				0 - beat16( 503 )
		);

		pacifica_one_layer(
				leds_out,
				pacifica_palette_3,
				sCIStart4,
				5 * 256,
				beatsin8( 8, 10, 28 ),
				beat16( 601 )
		);

		// Add brighter 'whitecaps' where the waves lines up more
		pacifica_add_whitecaps( leds_out );

		// Deepen the blues and greens a bit
		pacifica_deepen_colors( leds_out );

		return 20;
	}

	constexpr static const char *getName() {
		return "Pacifica";
	}

private:

	// Add one layer of waves into the led array
	static void
	pacifica_one_layer( CRGB *leds_out, CRGBPalette16 &p, uint16_t cistart, uint16_t wavescale, uint8_t bri,
	                    uint16_t ioff ) {
		uint16_t ci = cistart;
		uint16_t waveangle = ioff;
		uint16_t wavescale_half = ( wavescale / 2 ) + 20;
		for ( uint16_t i = 0; i < NUM_LEDS; i++ ) {
			waveangle += 250;
			uint16_t s16 = sin16( waveangle ) + 32768;
			uint16_t cs = scale16( s16, wavescale_half ) + wavescale_half;
			ci += cs;
			uint16_t sindex16 = sin16( ci ) + 32768;
			uint8_t sindex8 = scale16( sindex16, 240 );
			CRGB c = ColorFromPalette( p, sindex8, bri, LINEARBLEND );
			leds_out[ i ] += c;
		}
	}

// Add extra 'white' to areas where the four layers of light have lined up brightly
	static void pacifica_add_whitecaps( CRGB *leds_out ) {
		uint8_t basethreshold = beatsin8( 9, 55, 65 );
		uint8_t wave = beat8( 7 );

		for ( uint16_t i = 0; i < NUM_LEDS; i++ ) {
			uint8_t threshold = scale8( sin8( wave ), 20 ) + basethreshold;
			wave += 7;
			uint8_t l = leds_out[ i ].getAverageLight();
			if ( l > threshold ) {
				uint8_t overage = l - threshold;
				uint8_t overage2 = qadd8( overage, overage );
				leds_out[ i ] += CRGB( overage, overage2, qadd8( overage2, overage2 ));
			}
		}
	}

// Deepen the blues and greens
	static void pacifica_deepen_colors( CRGB *leds_out ) {
		for ( uint16_t i = 0; i < NUM_LEDS; i++ ) {
			leds_out[ i ].blue = scale8( leds_out[ i ].blue, 145 );
			leds_out[ i ].green = scale8( leds_out[ i ].green, 200 );
			leds_out[ i ] |= CRGB( 2, 5, 7 );
		}
	}

	CRGBPalette16 pacifica_palette_1 =
			{ 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117,
			  0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
	CRGBPalette16 pacifica_palette_2 =
			{ 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117,
			  0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
	CRGBPalette16 pacifica_palette_3 =
			{ 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33,
			  0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };
};


#include "ArduinoBorealis.h"

#endif //BURNT_WOOD_LAMP_COLORMODE_H
