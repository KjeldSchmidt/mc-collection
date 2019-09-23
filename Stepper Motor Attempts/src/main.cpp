#include <Arduino.h>

#define STEP_DELAY 5

int motorPins[] = {8, 9, 10, 11};
int currentActiveMotorPin = 0;

void stepForwardBy( int steps ) {
    for (int stepsTaken = 0; stepsTaken < steps; ++stepsTaken ) {
        digitalWrite( motorPins[0], LOW);
        digitalWrite( motorPins[1], LOW);
        digitalWrite( motorPins[2], LOW);
        digitalWrite( motorPins[3], LOW);
        currentActiveMotorPin = (currentActiveMotorPin + 1) % 4;
        digitalWrite( motorPins[ currentActiveMotorPin ], HIGH);
        delay( STEP_DELAY );
    }
}

void stepBackwardBy( int steps ) {
    for (int stepsTaken = 0; stepsTaken < steps; ++stepsTaken ) {
        digitalWrite( motorPins[0], LOW);
        digitalWrite( motorPins[1], LOW);
        digitalWrite( motorPins[2], LOW);
        digitalWrite( motorPins[3], LOW);
        currentActiveMotorPin = currentActiveMotorPin - 1;
        if ( currentActiveMotorPin < 0 ) currentActiveMotorPin = 3;
        digitalWrite( motorPins[ currentActiveMotorPin ], HIGH);
        delay( STEP_DELAY );
    }
}

void setup() {
  pinMode( motorPins[0], OUTPUT);
  pinMode( motorPins[1], OUTPUT);
  pinMode( motorPins[2], OUTPUT);
  pinMode( motorPins[3], OUTPUT);
  stepForwardBy( 100 );
  delay( 500 );
  stepBackwardBy( 200 );
}

void loop() {

}