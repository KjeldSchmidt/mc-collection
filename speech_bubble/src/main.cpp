#include <Arduino.h>
#include <FastLED.h>

#define WAVE_COUNT 6
#define NUM_LEDS 110
#define DATA_PIN 9

#define LIGHT_SCALER 40

#define LX 15
#define LX_SQUARED_INV 1/(LX *LX)

#define LT 30
#define LT_SQUARED_INV 1/(LT *LT)


CRGB leds[NUM_LEDS];

struct Wave
{
	uint8_t center;
	long time;
	uint8_t color;
	bool active = false;
	uint8_t direction = 1;
};

Wave waves[WAVE_COUNT];

void setup() {
    FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
    waves[0].color = 1;
    waves[0].center = 30;
    waves[0].time = 1;
    waves[0].active = true;
}

int calc(int x, long t, int center) {
	long t_squared = t*t;
	long x_squared = (x-center)*(x-center);
	long power = -t_squared * LT_SQUARED_INV - x_squared * LX_SQUARED_INV;
	return LIGHT_SCALER*pow(2, power);
}

void loop() {
	for (int x = 0; x < NUM_LEDS; x += 2)	{
		leds[x] = 0;
	}
	for (int x = 0; x < NUM_LEDS; x += 2)	{
		for (int i = 0; i < WAVE_COUNT; i++) {
				Wave wave = waves[i];
				if (wave.active) {					
					leds[x][wave.color] += calc(x, wave.time, wave.center);
				}
			}
	}

	for ( Wave &wave : waves ) {
		++wave.time;
		wave.center += wave.direction;
	}

    FastLED.show();
    delay(100);
}
