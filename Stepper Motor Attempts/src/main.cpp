#include <Arduino.h>
#include "LowPower.h"

#define STEP_DELAY 7
#define RPM 1/120
#define STEP_PER_TURN 2048


int motorPins[] = {4, 5, 6, 7};
int currentActiveMotorPin = 0;

void stepForwardBy( int steps ) {
    for (int stepsTaken = 0; stepsTaken < steps; ++stepsTaken ) {
        digitalWrite( motorPins[0], LOW);
        digitalWrite( motorPins[1], LOW);
        digitalWrite( motorPins[2], LOW);
        digitalWrite( motorPins[3], LOW);
        currentActiveMotorPin = (currentActiveMotorPin + 1) % 4;    
        digitalWrite( motorPins[ currentActiveMotorPin ], HIGH);
        digitalWrite( motorPins[ (currentActiveMotorPin + 1) % 4 ], HIGH);
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
        int secondActivePin = currentActiveMotorPin - 1;
        if ( secondActivePin < 0 ) secondActivePin = 3;
        
        digitalWrite( motorPins[ currentActiveMotorPin ], HIGH);
        digitalWrite( motorPins[ secondActivePin ], HIGH);
        delay( STEP_DELAY );
    }
}

void setup() {
  pinMode( motorPins[0], OUTPUT);
  pinMode( motorPins[1], OUTPUT);
  pinMode( motorPins[2], OUTPUT);
  pinMode( motorPins[3], OUTPUT);
}

void motor_off() {
  digitalWrite( motorPins[0], LOW);
  digitalWrite( motorPins[1], LOW);
  digitalWrite( motorPins[2], LOW);
  digitalWrite( motorPins[3], LOW); 
}

void loop() {
  LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF); 
  stepBackwardBy(1);
  motor_off();
}

