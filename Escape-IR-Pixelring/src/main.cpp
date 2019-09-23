#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 16
#define LED_DATA_PIN 9
#define yellowLedPin 5
#define redLedPin 5
#define greenLedPin 5

CRGBArray<NUM_LEDS> leds;

bool codeCorrect = false;
bool isWon = false;
int numbersEntered = 0;


// HIER DEINEN GEHEIMCODE EINGEBEN
int secretCode [3] = {3, 4, 5};



void colorAll( int r, int g, int b) {
  for ( int i = 0; i < NUM_LEDS; ++i ) {
		leds[i] = CRGB( r, g, b );
	}
	FastLED.show();
}

void win() {
  colorAll( 0, 0, 0);
  delay( 2000 );
  colorAll( 45, 0, 75);
  isWon = true;
}

void blinkRed( int initialBlink, int blinkSpeed ) {
  colorAll( 0, 0, 0);
  delay( blinkSpeed );
  for ( int i = 0; i < 10; ++i ) {
    colorAll( 60, 30, 0);
    delay( blinkSpeed );
    colorAll( 0, 0, 0);
    delay( blinkSpeed ); 
  }
  delay( initialBlink );
  colorAll(120, 0, 0);
}

void evaluateCode() {
  if ( codeCorrect ) {
    win();
  } else {
    blinkRed( 500, 75 );
    numbersEntered = 0;
    codeCorrect = false;
  }
}

void enterCode( int enteredNumber ) {
  if ( enteredNumber != -2 ) {
    colorAll( 50, 50, 0);
    delay( 150 );
    colorAll( 120, 0, 0);
    if ( enteredNumber == secretCode[ numbersEntered ] ) {
      if ( numbersEntered == 0 ) {
        codeCorrect = true; 
      }
    } else {
      codeCorrect = false;
    }
    ++numbersEntered;
    if ( numbersEntered == sizeof(secretCode)/sizeof(secretCode[0]) ) {
      evaluateCode();
    }
  }
}

int read() {
  bool gotValue = false;
  int value = -1;
  if ( digitalRead(4) && !gotValue ) {
    gotValue = true;
    value = 1;
    while( digitalRead(4) );
  }
  if ( digitalRead(5) && !gotValue ) {
    gotValue = true;
    value = 2;
    while( digitalRead(5) );
  }
  if ( digitalRead(6) && !gotValue ) {
    gotValue = true;
    value = 3;
    while( digitalRead(6) );
  }
  if ( digitalRead(7) && !gotValue ) {
    gotValue = true;
    value = 4;
    while( digitalRead(7) );
  }
  if ( digitalRead(8) && !gotValue ) {
    gotValue = true;
    value = 5;
    while( digitalRead(8) );
  }
  if ( digitalRead(9) && !gotValue ) {
    gotValue = true;
    value = 6;
    while( digitalRead(9) );
  }
  if ( digitalRead(10) && !gotValue ) {
    gotValue = true;
    value = 7;
    while( digitalRead(10) );
  }
  if ( digitalRead(11) && !gotValue ) {
    gotValue = true;
    value = 8;
    while( digitalRead(11) );
  }
  return value;
}

void setup() {
  FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);
  colorAll( 120, 0, 0 );
}

void loop() {
  if ( !isWon ) {
    int in = read();
    if ( in  != -1 ) {
      enterCode( in ); 
    }
  
  delay(5);
  }
}
