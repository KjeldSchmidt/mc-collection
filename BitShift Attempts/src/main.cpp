#include <Arduino.h>

//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

void setup()
{
    //set pins to output so you can control the shift register
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
}

void writeToRegister(unsigned char bitPattern) {
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    shiftOut(dataPin, clockPin, MSBFIRST, bitPattern);
    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
}

void loop()
{
    unsigned char max = 255;
    unsigned char min = 0;
    for ( unsigned char i = min; i < max; i++) {
        writeToRegister(i);
        delay(500);
    }
}
