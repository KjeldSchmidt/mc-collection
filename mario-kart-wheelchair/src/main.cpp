#include <Arduino.h>
#include "SoundPlayer.h"
// Disable interrupts during WS2812 data out so WiFi/Serial/timer don't corrupt timing
#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"
#include "LightManager.h"

const uint8_t PIN_PLAY = D1;  // GPIO5 on D1 Mini

CRGB leds[NUM_LEDS];
LightManager *lightManager = new LightManager{ leds };

SoundPlayer player( TX, RX );

volatile bool button_pressed = false;

void IRAM_ATTR onPlayPressed() {
    button_pressed = true;
}

void setup() {
    player.begin();
    delay( 50 );

    pinMode( PIN_PLAY, INPUT_PULLUP );  // button D1 to GND; trigger on press (FALLING)
    attachInterrupt( digitalPinToInterrupt( PIN_PLAY ), onPlayPressed, FALLING );

    CFastLED::addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>( leds, NUM_LEDS );
    lightManager->updateLEDs();
}

void loop() {
    if ( button_pressed ) {
        button_pressed = false;
        player.play_folder( 1 );
        lightManager->setMode("ColorPulse");
    }

    lightManager->updateLEDs();
}
