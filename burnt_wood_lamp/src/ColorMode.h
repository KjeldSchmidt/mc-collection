//
// Created by kjeld on 25/11/2020.
//

#ifndef BURNT_WOOD_LAMP_COLORMODE_H
#define BURNT_WOOD_LAMP_COLORMODE_H

#include "FastLED.h"

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


class TimExistentialDreadMode : public ColorMode {
// MODES
enum Modes {strobo, ambient, river, switchsort};
struct ModeConfig {
    Modes mode;
    uint32 minRuntime;
    uint32 maxRuntime;
    uint16 minDelay;
    uint16 maxDelay;
    bool init;
    uint8 chanceInPercent;
    //void updateColors(CRGB *leds_out);
    void set_function(std::function<void(CRGB*)> in) {
        SetColors = in;
    }
    std::function<void(CRGB*)> SetColors;
};

    // NOTES
    // LED-Order
    // Bottom Left -> Bottom Right -> Top Right -> Top left

    // MODES
    private: bool init = false;
    private: static const uint8 modeConfigsCount = 4;
    private: ModeConfig modeConfigs[modeConfigsCount];
    private: ModeConfig currentModeConfig;
    private: uint16 modeTimePassedInMs = 0;
    private: uint16 modeTimeMax = -1;

    // Part of TimExistentialDreadMode
    // MODE: STROBO
    // Colors
    private: CRGB stroboOff = CRGB(20, 20, 25);
    private: static const uint8 stroboColorCount = 7;
    private: CRGB stroboColors[stroboColorCount] {};
    // Caching
    private: bool stroboToggle = false;
    private: uint8 lastStroboColorIndex = -1;

    // MODE: SINGLE
    // Colors

    // MODE: RIVER
    // Colors

    // MODE: AMBIENT
    // Colors

    // Override Update
    // Return (uint 16) -> Delay in MilliSeconds
    public: uint16 Update(CRGB *leds_out) override {
        // Init
        Init();

        // Update ModeConfig
        UpdateModeConfig();

        // Set Colors
        currentModeConfig.SetColors(leds_out);

        // Set Random Delay
        const uint8 delay = random16(currentModeConfig.minDelay, currentModeConfig.maxDelay);
        modeTimePassedInMs += delay;

        return delay;
    }

    private: void Init() {
        if(init){
            return;
        }

        init = true;
        // Init Modes
        // STROBO
        modeConfigs[0] = {
                strobo,
                   500, // minRuntime
                  5000, // maxRuntime
                    10, // minDelay
                    30, // maxDelay
                    false, //init
                    10, // chanceInPercent
        };
        modeConfigs[0].set_function([ & ](CRGB* leds_out) { this->SetStroboColor(leds_out); });

        // AMBIENT
        modeConfigs[1] = {
                   ambient,
                     2000, // minRuntime
                     6000, // maxRuntime
                      200, // minDelay
                      200, // maxDelay
                      false, // init
                      100, // chanceInPercent
        };
        modeConfigs[1].set_function([ & ](CRGB* leds_out) { this->SetAmbientColor(leds_out); });

        // RIVER
        modeConfigs[2] = {
                river,
                10000,// minRuntime
                20000,// maxRuntime
                   10,// minDelay
                   10,// maxDelay
                false, // Init
                  100, // chanceInPercent

        };
        modeConfigs[2].set_function([ & ](CRGB* leds_out) { this->SetRiverColor(leds_out); });

        // SWITCH-SORT
        modeConfigs[3] = {
                switchsort,
                100000,// minRuntime
                100000,// maxRuntime
                    100,// minDelay
                    100,// maxDelay
                false, // Init
                   100, // chanceInPercent

        };
        modeConfigs[3].set_function([ & ](CRGB* leds_out) { this->SetSwitchSortColor(leds_out); });

        // Init Strobo
        stroboColors[0] = CRGB(255,   0,   0);
        stroboColors[1] = CRGB(  0, 255,   0);
        stroboColors[2] = CRGB(  0,   0, 255);
        stroboColors[3] = CRGB(255, 255,   0);
        stroboColors[4] = CRGB(255,   0, 255);
        stroboColors[5] = CRGB(  0, 255, 255);
        stroboColors[6] = CRGB(255, 255, 255);

        // Set initial Mode
        SetModeConfig(modeConfigs[0]);
    };
    private: void UpdateModeConfig() {
        if(modeTimePassedInMs < modeTimeMax){
            return;
        }

        // Set Next Mode randomly
        ModeConfig newModeConfig = currentModeConfig;
        uint8 random100 = random8(0, 100);

        SetModeConfig(modeConfigs[3]);
    }

    private: void SetModeConfig(ModeConfig modeConfig){
        // Reset time passed
        modeTimePassedInMs = 0;
        modeConfig.init = false;

        // Set max play time
        modeTimeMax = random16(modeConfig.minRuntime, modeConfig.maxRuntime);
        currentModeConfig = modeConfig;
    }

    private: void SetSingleColor( CRGB *leds_out, CRGB color ) {
        for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
            leds_out[i] = color;
        }
    }

    private: void SetStroboColor( CRGB *leds_out ) {
        // Toggle
        stroboToggle = !stroboToggle;
        // Get Color
        CRGB color = stroboToggle ? GetStroboColor() : stroboOff;
        // Set Color
        SetSingleColor(leds_out, color);
    }

    private: void SetAmbientColor( CRGB *leds_out ) {
        // Determine Color
        const CRGB color = CRGB(
                random(0, 255),
                random(0, 255),
                random(0, 255)
        );

        // Set Color
        SetSingleColor(leds_out, color);
    }

    private: void SetRiverColor( CRGB *leds_out ) {
        // Init
        if(!currentModeConfig.init){
            currentModeConfig.init = true;

            // Generate Colors
            const uint8 min = 255 - NUM_LEDS;

            // Use random Color Combination
            uint8 rMultiplier = random(2);
            uint8 gMultiplier = random(2);
            uint8 bMultiplier = random(2);
            // But at least one color
            if(rMultiplier == 0 && gMultiplier == 0 && bMultiplier == 0){
                bMultiplier = 1;
            }

            for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
                const uint8 val = min+i;
                CRGB color = CRGB(
                        rMultiplier * val,
                        gMultiplier * val,
                        bMultiplier * val
                        );
                leds_out[ i ] = color;
            }
        }

        ShiftColors(leds_out);
    }

    private: void SetSwitchSortColor( CRGB *leds_out ) {
        // Init
        if(!currentModeConfig.init){
            currentModeConfig.init = true;

            // Generate Colors
            for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
                leds_out[ i ] = GetRandomColor();
            }
        }

        SwitchSortColors(leds_out);
    }

    private: uint8 lastIndex = 1;
    private: uint8 colorIndex = 0;
    private: void SwitchSortColors(CRGB *leds_out) {

        for ( uint8_t i = lastIndex; i < NUM_LEDS; i++ ) {
            CRGB val1 = leds_out[i-1];
            CRGB val2 = leds_out[i];

            // No need to switch
            if(val1.r <= val2.r ) {
                continue;
            }
            else {

                // Switch, set lastIndex & wait for next step
                leds_out[i - 1] = CRGB(val2.r, val1.g, val1.b);// val2;
                leds_out[i] = CRGB(val1.r, val2.g, val2.b);
                lastIndex = i + 1;
            }
        }

        // Completed without switching so current color is sorted -> do next color
        colorIndex++;
        lastIndex = 1;

        // If all 3 colors have completed end mode
        if(colorIndex > 2){
            modeTimePassedInMs += 1000000;
        }
    }

    private: CRGB GetRandomColor() {
        const CRGB color = CRGB(
                random(0, 255),
                random(0, 255),
                random(0, 255)
        );
        return color;
    }

    private: void ShiftColors(CRGB *leds_out){
        // leds_out[0] = CRGB(133,33,72);
        CRGB nextColor = leds_out[NUM_LEDS-1];
        for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
            CRGB currentColor = nextColor;
            nextColor = leds_out[i];
            leds_out[ i ] = currentColor;
        }
    }

    private: CRGB GetStroboColor(){
        uint8 newStroboColorIndex = lastStroboColorIndex;
        while(newStroboColorIndex == lastStroboColorIndex){
            newStroboColorIndex = random8(0, stroboColorCount);
        }

        lastStroboColorIndex = newStroboColorIndex;
        CRGB stroboColor = stroboColors[newStroboColorIndex];
        return stroboColor;
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
		CHSV color{hue, saturation, value};
		for ( uint8_t i = 0; i < NUM_LEDS; i++ ) {
			leds_out[ i ] = color;
		}
		hue += 1;

		return calc_wait_time();
	}

	constexpr static const char *getName() {
		return "ColorPulse";
	}

private:
	uint16_t calc_wait_time() {
		uint64_t currentMillis = millis();
		double fast = sin(currentMillis / 2700);
		double medium = sin(currentMillis / 53000);
		double slow = sin(currentMillis / 71000);
		return (uint16_t) 63 * (fast*medium*slow) + 64;
	}
	uint8_t hue = 0;
	uint8_t saturation = 255;
	uint8_t value = 255;
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
		return 1000;
	}

	constexpr static const char *getName() {
		return "DualColor( Color, Color )";
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
