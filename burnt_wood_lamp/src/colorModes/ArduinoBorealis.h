//
// Created by kjeld on 08/12/2020.
//

#ifndef BURNT_WOOD_LAMP_ARDUINOBOREALIS_H
#define BURNT_WOOD_LAMP_ARDUINOBOREALIS_H

#include "colorModes/ColorMode.h"

//List of colors allowed for waves
//The first dimension of this array must match the second dimension of the colorwighting array
static uint8_t allowedcolors[5][3] = {
		{ 17,  177, 13 },    //Greenish
		{ 148, 242, 5 },    //Greenish
		{ 25,  173, 121 },    //Turquoise
		{ 250, 77,  127 },   //Pink
		{ 171, 101, 221 },  //Purple
};

//Colorweighing allows to give some colors more weight so it is more likely to be choosen for a wave.
//The second dimension of this array must match the first dimension of the allowedcolors array
//Here are 3 presets.
constexpr static uint8_t colorweighting[3][5] = {
		{ 10, 10, 10, 10, 10 },   //Weighting equal (every color is equally likely)
		{ 2,  2,  2,  6,  6 },        //Weighting reddish (red colors are more likely)
		{ 6,  6,  6,  2,  2 }         //Weighting greenish (green colors are more likely)
};

class ArduinoBorealis : public ColorMode {
public:
	ArduinoBorealis() {
		for ( auto &wave : waves ) {
			wave = new BorealisWave{};
		}
	}

	uint16 Update( CRGB *leds_out ) override {
		for ( auto &wave : waves ) {
			wave->update();
			if ( wave->expired()) {
				delete wave;
				wave = new BorealisWave{};
			}
		}

		for ( int i = 0; i < NUM_LEDS; i++ ) {
			CRGB mix = CRGB{ 0, 0, 0 };
			for ( auto &wave : waves ) {
				mix += wave->getColorForLED( i );
			}
			leds_out[ i ] = mix;
		}

		return 0;
	}

private:
	static byte getWeightedColor( byte weighting ) {
		byte sumOfWeights = 0;

		for ( byte i = 0; i < sizeof colorweighting[ 0 ]; i++ ) {
			sumOfWeights += colorweighting[ weighting ][ i ];
		}

		byte randomweight = random( 0, sumOfWeights );

		uint8_t base_color_index = 0;
		while ( randomweight > colorweighting[ weighting ][ base_color_index ] ) {
			randomweight -= colorweighting[ weighting ][ base_color_index ];
		}

		return base_color_index;
	}

	constexpr static uint8_t wave_count = 6;
	constexpr static uint8_t wave_speed = 3;
	constexpr static uint8_t wave_width = 3;
	constexpr static uint8_t color_weight_preset = 1;

	class BorealisWave {
	private:
		int ttl = random( 500, 1501 );
		uint8_t basecolor_index = getWeightedColor( color_weight_preset );
		float basealpha = random( 50, 101 ) / (float) 100;
		int age = 0;
		int width = random( NUM_LEDS / 10, NUM_LEDS / wave_width );
		float center = random( 101 ) / (float) 100 * NUM_LEDS;
		bool goingleft = random( 0, 2 ) == 0;
		float speed = random( 10, 30 ) / (float) 100 * wave_speed;
		bool alive = true;

	public:
		CRGB getColorForLED( int ledIndex ) const {
			if ( ledIndex < center - width / 2 || ledIndex > center + width / 2 ) {
				return CRGB::Black;
			} else {
				CRGB rgb = CRGB();

				//Offset of this led from center of wave
				//The further away from the center, the dimmer the LED
				int offset = abs( ledIndex - (uint8_t) center );
				float offsetFactor = (float) offset / ( width / 2 );

				//The age of the wave determines it brightness.
				//At half its maximum age it will be the brightest.
				float ageFactor = 1;
				if ((float) age / ttl < 0.5 ) {
					ageFactor = (float) age / ( ttl / 2 );
				} else {
					ageFactor = (float) ( ttl - age ) / ((float) ttl * 0.5 );
				}

				//Calculate color based on above factors and basealpha value
				rgb.r = allowedcolors[ basecolor_index ][ 0 ] * ( 1 - offsetFactor ) * ageFactor * basealpha;
				rgb.g = allowedcolors[ basecolor_index ][ 1 ] * ( 1 - offsetFactor ) * ageFactor * basealpha;
				rgb.b = allowedcolors[ basecolor_index ][ 2 ] * ( 1 - offsetFactor ) * ageFactor * basealpha;

				return rgb;
			}
		};

		//Change position and age of wave
		//Determine if its sill "alive"
		void update() {
			if ( goingleft ) {
				center -= speed;
			} else {
				center += speed;
			}

			age++;

			if ( age > ttl ) {
				alive = false;
			} else {
				if ( goingleft ) {
					if ( center + width / 2 < 0 ) {
						alive = false;
					}
				} else {
					if ( center - width / 2 > NUM_LEDS ) {
						alive = false;
					}
				}
			}
		};

		bool expired() const {
			return !alive;
		};
	};

	BorealisWave *waves[wave_count]{};

};

#endif //BURNT_WOOD_LAMP_ARDUINOBOREALIS_H
