#include <Arduino.h>

#define BUZZ_PIN 3
#define BUZZ_TIME 100

unsigned char beats = 4;
unsigned char currentBeat = 0;
unsigned char bpm = 120;

unsigned long interval = 0;

void playBeep() {
    if ( currentBeat != 0 ) {
        analogWrite( BUZZ_PIN, 100 );
    } else {
        analogWrite( BUZZ_PIN, 245 );
    }
    currentBeat = ( currentBeat + 1 ) % beats;
}

void calculateInterval() {
    unsigned char bps = bpm / 60;
    interval = 1000 / bps;
}

void setup() {
    pinMode( BUZZ_PIN, OUTPUT );
    calculateInterval();
}

void loop() {
    playBeep();
    delay( BUZZ_TIME );
    analogWrite( BUZZ_PIN, 0 );
    delay(interval - BUZZ_TIME );
}