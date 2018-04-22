#import <Arduino.h>
#import <FastLED.h>

#define NUM_LEDS 16
#define LED_DATA_PIN 9

#define MAXIMUM_BRIGHTNESS_PER_CHANNEL 10 // On a scale from 1-255, of course. Actual value may be very slightly higher.
#define SPEED_OF_LIGHT 1000 // Speed of animation in 

#define RED_WAVES_PER_STRIP 2 // Experiment with these
#define GREEN_WAVES_PER_STRIP 5 // Experiment with these
#define BLUE_WAVES_PER_STRIP 3 // Experiment with these

unsigned char red;
unsigned char green;
unsigned char blue;
CRGBArray<NUM_LEDS> leds;

void setup() {
	FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);
}

unsigned char calc(int freq, float pos) {
	return (char) ((cos( pos*PI*freq*2 / NUM_LEDS ) * MAXIMUM_BRIGHTNESS_PER_CHANNEL) + (MAXIMUM_BRIGHTNESS_PER_CHANNEL + 1))/2;
}

void loop() {
	unsigned long time = millis();
	for ( int i = 0; i < NUM_LEDS; ++i ) {
		red = calc( RED_WAVES_PER_STRIP, (float) i - ( (float) ( time ) / (float) SPEED_OF_LIGHT ) * RED_WAVES_PER_STRIP );
		green = calc( GREEN_WAVES_PER_STRIP, (float) i - ( (float) ( time ) / (float) SPEED_OF_LIGHT ) * GREEN_WAVES_PER_STRIP );
		blue = calc( BLUE_WAVES_PER_STRIP, (float) i - ( (float) ( time ) / (float) SPEED_OF_LIGHT ) * BLUE_WAVES_PER_STRIP );

		leds[i] = CRGB( red, green, blue );
	}
	FastLED.show();
}