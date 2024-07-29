//
// Created by kjeld on 25/11/2020.
//

#ifndef BURNT_WOOD_LAMP_COLORMODE_H
#define BURNT_WOOD_LAMP_COLORMODE_H

#include "FastLED.h"
#include "base64decode.h"

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


class ColorFromPayload : public ColorMode {
public:
	explicit ColorFromPayload( const String &payload ): colors{} {
		uint8_t binary_payload[payload.length()];
		base64_decode(payload, binary_payload);

		for (size_t i = 0; i < NUM_LEDS; i++) {
			colors[i] = CRGB(binary_payload[3*i], binary_payload[3*i+1], binary_payload[3*i+2]);
		}
	}

	uint16 Update(CRGB *leds_out) override {
		for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
			leds_out[i] = colors[i];
		}

		return 1000;
	}

	constexpr static const char *getName() {
		return "ColorFromPayload";
	}
private:
	CRGB colors[NUM_LEDS];
};


class KjeldPartyMode : public ColorMode {
public:
	uint16 Update( CRGB *leds_out ) override {
		CRGB colors[5] = {
				CRGB( 255, 0, 0 ),
				CRGB( 0, 255, 0 ),
				CRGB( 0, 0, 255 ),
				CRGB( 0, 0, 0 ),
				CRGB( 255, 200, 0 ),
		};

		if ( !init ) {
			for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
				leds_out[ i ] = colors[ 0 ];
			}
			init = true;
		}

		for ( int _ = 0; _ < NUM_LEDS / 3; _++ ) {
			uint8_t led_index = random8( 0, NUM_LEDS );
			uint8_t color_index = random8( 0, 5 );
			leds_out[ led_index ] = colors[ color_index ];
		}

		return 500;
	}

	constexpr static const char *getName() {
		return "KjeldPartyMode";
	}

private:
	bool init = false;
};

class LauraPartyMode : public ColorMode {
public:
	uint16 Update( CRGB *leds_out ) override {
		static bool Start = false;
		if ( Start == false ) {
			for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
				CRGB value = CRGB( 0, 0, 0 );
				leds_out[ i ] = value;
				if ( i == NUM_LEDS - 1 ) Start = true;
			}
			return 1;
		}
		long i = random( 1, NUM_LEDS );
		long k = random( 1, 50 );
		if ( i == 100 and millis() - time > 15000 and k == 4 ) {
			time = millis();
		}

		if ( millis() - time < 5000 ) {
			if ( leds_out[ 1 ] == CRGB( 255, 255, 255 )) {
				for ( uint8_t j = 0; j < NUM_LEDS; j++ ) {
					CRGB value = CRGB( 0, 0, 0 );
					leds_out[ j ] = value;
				}
			} else {
				for ( uint8_t j = 0; j < NUM_LEDS; j++ ) {
					CRGB value = CRGB( 255, 255, 255 );
					leds_out[ j ] = value;
				}
			}
			return 50;
		}

		CRGB value = leds_out[ i - 1 ];
		long r1 = random( 2 );
		long r2 = random( 2 );
		long r3 = random( 2 );
		long r4 = random( 2 );
		long r5 = random( 2 );
		long r6 = random( 2 );
		value = CRGB( value.red + r1 - r4, value.green + r2 - r5, value.blue + r3 - r6 );
		leds_out[ i ] = value;


		return 10;
	}

	constexpr static const char *getName() {
		return "LauraPartyMode";
	}

private:
	unsigned long time = 0;
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
		return "GlobalColorTick";
	}

private:
	uint8_t mode = 1;
};

class ColorWheel : public ColorMode {
public:
	uint16 Update( CRGB *leds_out ) override {
		hue += 1;
		CHSV color{hue, 255, 255};
		for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
			leds_out[ i ] = color;
		}

		return 0;
	}

	constexpr static const char *getName() {
		return "ColorWheel";
	}

private:
	uint8_t hue = 0;
};

class ColorPulse : public ColorMode {
public:
	uint16 Update( CRGB *leds_out ) override {
		uint64_t currentMillis = millis();
		double start_hue = calc_start_hue(currentMillis);
		double end_hue = calc_end_hue(currentMillis);
		double step = double(end_hue - start_hue)/NUM_LEDS;
		for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
			uint8_t hue = start_hue + step*i;
			leds_out[ i ] = CHSV(hue, 255, 255);
		}

		return 10;
	}

	constexpr static const char *getName() {
		return "ColorPulse";
	}

private:
	static double calc_start_hue(uint64_t currentMillis) {
		double wave1 = sin(double(currentMillis) / 13500.0);
		double wave2 = sin(double(currentMillis) / 26500.0);
		double wave3 = sin(double(currentMillis) / 5500.0);
		return 511 * (wave1*wave2*wave3) + 255;
	}

	static double calc_end_hue(uint64_t currentMillis) {
		double wave1 = sin(double(currentMillis) / 12700.0);
		double wave2 = sin(double(currentMillis) / 25650.0);
		double wave3 = sin(double(currentMillis) / 6555.0);
		return 511 * (wave1*wave2*wave3) + 255;
	}
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
		return "SingleColor";
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
		return 1000;
	}

	constexpr static const char *getName() {
		return "DualColor";
	}

private:
	CRGB color_upper;
	CRGB color_lower;
};

class SingleColorBlink : public ColorMode {
public:
	SingleColorBlink( const CRGB &color, uint16 blink_time ):
		color( color ),
		blink_time( blink_time ) {}

	uint16 Update( CRGB *leds_out ) override {
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

class DiscoStrobo : public SingleColorBlink {
public:
	DiscoStrobo() : SingleColorBlink( 
		CRGB(MAX_BRIGHTNESS_PER_CHANNEL, MAX_BRIGHTNESS_PER_CHANNEL, MAX_BRIGHTNESS_PER_CHANNEL),
		50
	) {}

	constexpr static const char *getName() {
		return "DiscoStrobo";
	}
};

class CityAtSundown : public DualColor {
public:
	CityAtSundown() : DualColor(
			CRGB( 0x610200 ),
			CRGB( 0x611e01 )
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
