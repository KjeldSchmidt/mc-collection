#include <Arduino.h>
#include <FastLED.h>

#define WAVE_COUNT 6
#define NUM_LEDS 110
#define DATA_PIN 9

#define LIGHT_SCALER 40

#define LX 15
#define LX_SQUARED_INV 1/(LX *LX)

#define DAMPENING_MULT 10


long t;
long x_squared;
int8_t centered;
float power;


CRGB leds[NUM_LEDS];

struct Wave
{
	uint16_t center;
	uint8_t color;
	int8_t direction = 1;
	uint16_t t_offset = 0;
	float t_pow = 0;

};

Wave waves[WAVE_COUNT];

void setup() {
    FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);


    waves[0].color = 1;
    waves[0].center = 30;
    waves[0].t_offset = 0;
    waves[0].direction = 1;


    waves[1].color = 1;
    waves[1].center = 30;
    waves[1].t_offset = 5;
    waves[1].direction = -1;

    waves[2].color = 2;
    waves[2].center = 70;
    waves[2].t_offset = 10;
    waves[2].direction = 1;

    waves[3].color = 2;
    waves[3].center = 70;
    waves[3].t_offset = 15;
    waves[3].direction = -1;

    waves[4].color = 0;
    waves[4].center = 130;
    waves[4].t_offset = 20;
    waves[4].direction = 1;

    waves[5].color = 0;
    waves[5].center = 130;
    waves[5].t_offset = 25;
    waves[5].direction = -1;
}

int calc(int x, float t_pow, int center) {
	centered = ((x-center % NUM_LEDS) + NUM_LEDS) % NUM_LEDS;
	x_squared = centered*centered;
	power = t_pow - x_squared * LX_SQUARED_INV;
	return LIGHT_SCALER*pow(2, power);
}

void loop() {
	for (int x = 0; x < NUM_LEDS; x += 2)	{
		leds[x] = 0x010101;
		for (int i = 0; i < WAVE_COUNT; i++) {
				Wave wave = waves[i];
				leds[x][wave.color] += calc(x, wave.t_pow, wave.center);
			}
	}

	++t;
	for ( Wave &wave : waves ) {
		wave.t_pow = -DAMPENING_MULT*sin( (t - wave.t_offset)*0.01  );
		wave.center += wave.direction;
	}

    FastLED.show();
}
