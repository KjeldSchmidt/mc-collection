#include "ColorMode.h"

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
