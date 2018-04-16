#import <Arduino.h>
#import <FastLED.h>

#define NUM_LEDS 144
#define LED_DATA_PIN 9

#define MAXIMUM_BRIGHTNESS_PER_CHANNEL 50
#define LOOP_WAIT_TIME 50

#define RED_WAVES_PER_STRIP 3
#define GREEN_WAVES_PER_STRIP 2
#define BLUE_WAVES_PER_STRIP 1

long lastLoopTime = 0;

char red;
char green;
char blue;

char offset = 0;

CRGBArray<NUM_LEDS> leds;

void setup() {
	FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);
}

char calc(int freq, char pos) {
	return (char) (cos( PI*freq*pos / NUM_LEDS ) + 1) * MAXIMUM_BRIGHTNESS_PER_CHANNEL/2;
}

void loop() {
	if ( millis() > lastLoopTime + LOOP_WAIT_TIME ) {
		for ( int i = 0; i < NUM_LEDS; ++i ) {
			red = calc( RED_WAVES_PER_STRIP, (i + offset*RED_WAVES_PER_STRIP) % NUM_LEDS );
			green = calc( GREEN_WAVES_PER_STRIP, (i + offset*GREEN_WAVES_PER_STRIP) % NUM_LEDS );
			blue = calc( BLUE_WAVES_PER_STRIP, (i + offset*BLUE_WAVES_PER_STRIP) % NUM_LEDS );

			leds[ (i + offset) % NUM_LEDS  ] = CRGB( red, green, blue );
			++offset;
		}

		FastLED.show();
		lastLoopTime = millis();
	}
}