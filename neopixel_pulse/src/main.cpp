#import <Arduino.h>
#import <FastLED.h>

#define NUM_LEDS 16
#define LED_DATA_PIN 9

#define MAXIMUM_BRIGHTNESS_PER_CHANNEL 5
#define LOOP_WAIT_TIME 3000

#define RED_WAVES_PER_STRIP 2
#define GREEN_WAVES_PER_STRIP 3
#define BLUE_WAVES_PER_STRIP 4

long lastLoopTime = 0;

unsigned char red;
unsigned char green;
unsigned char blue;

unsigned char offset = 0;

CRGBArray<NUM_LEDS> leds;

void setup() {
	FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);
}

unsigned char calc(int freq, float pos) {
	return (char) (cos( pos*PI*freq*2 / NUM_LEDS ) * MAXIMUM_BRIGHTNESS_PER_CHANNEL) + (MAXIMUM_BRIGHTNESS_PER_CHANNEL + 1);
}

void loop() {
	unsigned long time = millis();
	for ( int i = 0; i < NUM_LEDS; ++i ) {
		red = calc( RED_WAVES_PER_STRIP, (float) i - ( (float) ( time % LOOP_WAIT_TIME ) / (float) LOOP_WAIT_TIME ) );
		green = calc( GREEN_WAVES_PER_STRIP, (float) i - ( (float) ( time % LOOP_WAIT_TIME ) / (float) LOOP_WAIT_TIME ) );
		blue = calc( BLUE_WAVES_PER_STRIP, (float) i - ( (float) ( time % LOOP_WAIT_TIME ) / (float) LOOP_WAIT_TIME ) );

		leds[ ( i + offset ) % NUM_LEDS ] = CRGB( red, green, blue );
	}
	FastLED.show();

	if ( millis() > lastLoopTime + LOOP_WAIT_TIME ) {
		lastLoopTime = time;
		++offset;
	}
}