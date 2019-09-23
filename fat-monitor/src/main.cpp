//
// Created by kjeld on 04.09.19.
//

#include "Arduino.h"
#include "LedControl.h"
#include "binary.h"

#include "main.h"

#define NUM_MATRICES 4

LedControl lc{ 9, 10, 11, NUM_MATRICES };

void setup() {
    for ( int addr = 0; addr < NUM_MATRICES; addr++ ) {
        reset_and_wake( addr );
    }
    lc.setLed( 0, 0, 7, true );
    lc.setLed( 1, 7, 0, true );
    lc.setLed( 2, 7, 0, true );
    lc.setLed( 3, 7, 0, true );
}

void loop() {
}

void reset_and_wake( int addr ) {
    lc.shutdown( addr, false );
    lc.setIntensity( addr, 8 );
    lc.clearDisplay( addr );
}