#include <Arduino.h>
#include <IRremote.h>

#define IR_PIN 11

IRrecv irDetect(IR_PIN);
decode_results irIn;


int decodeIRKey(unsigned long IRValue) {
    switch (IRValue) {
        case 0xFF4AB5:
            return 0;

        case 0xFF6897:
            return 1;

        case 0xFF9867:
            return 2;

        case 0xFFB04F:
            return 3;

        case 0xFF30CF:
            return 4;

        case 0xFF18E7:
            return 5;

        case 0xFF7A85:
            return 6;

        case 0xFF10EF:
            return 7;

        case 0xFF38C7:
            return 8;

        case 0xFF5AA5:
            return 9;

        case 0xFF52AD:
        case 0xFF42BD:
        case 0xFF02FD:
            return -1;

        default:
            return -2;
    }
}

void setup() {
    Serial.begin(9600);
    irDetect.enableIRIn();
}

void loop() {
    if (irDetect.decode(&irIn)) {
      int irInput = decodeIRKey(irIn.value);
      Serial.println( irInput );
      irDetect.resume();
    }
}