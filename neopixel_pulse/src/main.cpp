#import <Arduino.h>
#import <FastLED.h>

#define NUM_LEDS 114
#define LED_DATA_PIN 9

#define MAXIMUM_BRIGHTNESS_PER_CHANNEL 30 // On a scale from 1-255, of course. Actual value may be very slightly higher.
#define SPEED_OF_LIGHT 200 // Speed of animation in <continued months or years later> . . . units?

#define RED_WAVES_PER_STRIP 5 // Experiment with these
#define GREEN_WAVES_PER_STRIP 2 // Experiment with these
#define BLUE_WAVES_PER_STRIP 3 // Experiment with these

#define CONST_SIN_FACTOR 2 / NUM_LEDS

unsigned char red;
unsigned char green;
unsigned char blue;
CRGBArray<NUM_LEDS> leds;

void setup() {
	FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);
}

unsigned char calc(int freq, float pos, float offset) {
	return (char) ( ( sin( pos * (PI - offset) * freq * CONST_SIN_FACTOR ) * MAXIMUM_BRIGHTNESS_PER_CHANNEL) + (MAXIMUM_BRIGHTNESS_PER_CHANNEL + 1 ) ) / 2;
}

void loop() {
	unsigned long time = millis();
	float speed = (float) ( time ) / (float) SPEED_OF_LIGHT;
	float red_speed = speed * RED_WAVES_PER_STRIP;
	float green_speed = speed * GREEN_WAVES_PER_STRIP;
	float blue_speed = speed * BLUE_WAVES_PER_STRIP;
	for ( int i = 0; i < NUM_LEDS; ++i ) {
		leds[i] = 0x000000;
		leds[i].red += calc( RED_WAVES_PER_STRIP, (float) i - red_speed, 0 );
		leds[i].green += calc( GREEN_WAVES_PER_STRIP, (float) i - green_speed, PI/2 );
		leds[i].blue += calc( BLUE_WAVES_PER_STRIP, (float) i - blue_speed, PI );
	}
	FastLED.show();
}