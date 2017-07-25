#include "Arduino.h"
#include "NeoPixelBus.h"

#define PIXEL_COUNT 2
#define PIXEL_PIN 11
#define LEFT_EYE 0
#define RIGHT_EYE 1

RgbColor red( 5, 0, 0 );



NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> pixels( PIXEL_COUNT, PIXEL_PIN );
 
void setup() {
    pixels.SetPixelColor( LEFT_EYE, red );
    pixels.SetPixelColor( RIGHT_EYE, red );
    pixels.Show();
}

void loop() {

}