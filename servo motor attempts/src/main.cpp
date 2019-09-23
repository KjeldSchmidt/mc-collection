#include <Arduino.h>
#include <Servo.h>

#define SERVO_PIN 9
#define DELAY_TIME 5 //Seems to be lowest possible speed where the SG90 can keep pace.

Servo servo;

int pos = 0;

void setup() {
    servo.attach(SERVO_PIN);
}

void loop() {
    for ( pos = 0; pos <= 180; ++pos ) {
        servo.write(pos);
        delay( DELAY_TIME );
    }

    for ( pos = 180; pos >= 0; --pos ) {
        servo.write(pos);
        delay( DELAY_TIME );
    }
}