/*
 * Pins:
 *
 * GND OE S1 S0
 * VCC OUT S2 S3
 *
 * Connecting GND and VCC always makes the LEDs light up.
 * S0, S1 select output frequency scaling
 * H H 100%
 * H L 20%
 * L H 2%
 * L L Power Down
 *
 * S2, S3 select which color to measure
 * H H Green
 * H L White
 * L H Blue
 * L L Red
 *
 * OE ist output enable. As far as I understand, if nothing is connected, it behaves as if pulled high. If pulled high,
 * the sensor works as expected and sends a signal trough OUT. If pulled low, it doesn't send a signal. Thus, many
 * sensors can be used on the same pin via multiplexing.
 */

#include "Arduino.h"

#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8
int frequency = 0;
void setup() {
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    // Setting frequency-scaling to 20%
    digitalWrite(S0,HIGH);
    digitalWrite(S1,LOW);

    Serial.begin(9600);
}
void loop() {
    // Setting red filtered photodiodes to be read
    digitalWrite(S2,LOW);
    digitalWrite(S3,LOW);
    // Reading the output frequency
    frequency = pulseIn(sensorOut, LOW);
    // Printing the value on the serial monitor
    Serial.print("R= ");//printing name
    Serial.print(frequency);//printing RED color frequency
    Serial.print("  ");
    delay(100);
    // Setting Green filtered photodiodes to be read
    digitalWrite(S2,HIGH);
    digitalWrite(S3,HIGH);
    // Reading the output frequency
    frequency = pulseIn(sensorOut, LOW);
    // Printing the value on the serial monitor
    Serial.print("G= ");//printing name
    Serial.print(frequency);//printing RED color frequency
    Serial.print("  ");
    delay(100);
    // Setting Blue filtered photodiodes to be read
    digitalWrite(S2,LOW);
    digitalWrite(S3,HIGH);
    // Reading the output frequency
    frequency = pulseIn(sensorOut, LOW);
    // Printing the value on the serial monitor
    Serial.print("B= ");//printing name
    Serial.print(frequency);//printing RED color frequency
    Serial.println("  ");
    delay(100);
}