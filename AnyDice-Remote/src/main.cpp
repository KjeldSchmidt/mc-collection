#include <Arduino.h>
#include <IRremote.h>
#include <TM1637Display.h>

#define IR_PIN 11
#define DISPLAY_CLOCK 9
#define DISPLAY_IO 6

int currentInput = 0;
int sizeOfDiceToRoll = 0;
int numberOfDiceToRoll = 1;
int result = 0;

IRrecv irDetect(IR_PIN);
decode_results irIn;
TM1637Display display( DISPLAY_CLOCK, DISPLAY_IO );


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
        case 0xFF02FD:
            return -1;

        case 0xFF42BD:
            return -3;

        default:
            return -2;


    }
}

int processInput( int irInput ) {
    if ( irInput >= 0 && currentInput <= 999 ) {
        currentInput = currentInput * 10 + irInput;
    } else if ( irInput == -3 ) {
        sizeOfDiceToRoll = currentInput;
        currentInput = 0;
    } else if ( irInput == -1 ) {
        if ( sizeOfDiceToRoll == 0 ) {
            sizeOfDiceToRoll = currentInput;
            numberOfDiceToRoll = 1;
        } else {
            numberOfDiceToRoll = currentInput;
        }
        
        result = numberOfDiceToRoll * sizeOfDiceToRoll;
        currentInput = 0;
        return result;
    }

    return currentInput;
}

void setup() {
    Serial.begin(9600);
    irDetect.enableIRIn();
    display.setBrightness(0x0a);
    display.showNumberDec(0);
}

void loop() {
    if (irDetect.decode(&irIn)) {
      int irInput = decodeIRKey(irIn.value);
      Serial.println( irInput );
      irDetect.resume();
      int out = processInput( irInput );
      if ( irInput != -2 ) {
        display.showNumberDec( out );     
      }
    }
}