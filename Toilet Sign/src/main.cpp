#include "Arduino.h"
#include "NeoPixelBus.h"

#define PIXEL_COUNT 134
#define PIXEL_PIN 5
#define BLINK_PIN 9
#define STEP_PIN 2
#define MOTION_SENSOR_PIN 11

#define MOTION_DECAY_TIME 60000

RgbColor red( 4, 0, 0 );
RgbColor darkRed( 0, 0, 0);
RgbColor green( 0, 3, 0 );
RgbColor darkGreen( 0, 0, 0);
RgbColor yellow( 4, 3, 0);

unsigned long lastMotionDetected = 0;
char toiletInUse = 0;
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> pixels( PIXEL_COUNT, PIXEL_PIN );

void setup() {
    pixels.Begin();
    pixels.Show();
    pinMode( MOTION_SENSOR_PIN, INPUT );
    pinMode( BLINK_PIN, OUTPUT );
    pinMode( STEP_PIN, OUTPUT );
}

void setToiletInUse() {
    for ( uint8_t i = 0; i < PIXEL_COUNT + 1; ++i ) {
        pixels.SetPixelColor( i, ( i % 2 == 1 ) ? red : darkGreen );
    }
    pixels.Show();
    toiletInUse = 1;
}

void step( float fraction ) {
    pixels.SetPixelColor( (int) ( fraction * (float) PIXEL_COUNT ) , yellow );
    pixels.Show();
}

void setToiletNotInUse() {

    for ( uint8_t i = 0; i < PIXEL_COUNT + 1; ++i ) {
        pixels.SetPixelColor( PIXEL_COUNT - i, ( i % 2 == 0 ) ? green : darkRed );
        pixels.Show();
        delay( 10 );
    }
    
    toiletInUse = 0;
}

void loop() {
    char motionDetection = digitalRead( MOTION_SENSOR_PIN );
    unsigned long currentTime = millis();
    
    if ( motionDetection ) {
        lastMotionDetected = currentTime;
        setToiletInUse();
    }

    delay( 10 );
    if ( toiletInUse ) {
        step( (float) ( currentTime - lastMotionDetected ) / (float) MOTION_DECAY_TIME );
    }

    if ( currentTime > lastMotionDetected + MOTION_DECAY_TIME ) {
        setToiletNotInUse();
    }    
}