//
// Created by kjeld on 23.09.19.
//

#include "main.h"
#include "Arduino.h"
#include "Servo.h"
#include <hcsr04.h>

#define SERVO_PIN 9
#define DELAY_TIME 5 //Seems to be lowest possible speed where the SG90 can keep pace.

#define US_TRIGGER_PIN 12
#define US_ECHO_PIN 13

#define MIN_RESPONSE_DIST 50
#define MAX_RESPONSE_DIST 90

Servo servo;
HCSR04 hcsr04(US_TRIGGER_PIN, US_ECHO_PIN, 20, 4000);


void setup() {
    servo.attach(SERVO_PIN);
}

void loop() {
    uint16_t dist = hcsr04.distanceInMillimeters();
    if (dist < MAX_RESPONSE_DIST + MIN_RESPONSE_DIST) {
        servo.write(( MAX_RESPONSE_DIST + MIN_RESPONSE_DIST - dist ) * 2 );
    } else if (dist < MIN_RESPONSE_DIST) {
        servo.write(180);
    } else {
        servo.write(0);
    }
    delay(DELAY_TIME*5);
}

