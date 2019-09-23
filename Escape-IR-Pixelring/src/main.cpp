#include <Arduino.h>
#include <IRremote.h>
#include <FastLED.h>

#define NUM_LEDS 16
#define LED_DATA_PIN 9
#define yellowLedPin 5
#define redLedPin 5
#define greenLedPin 5

int RECV_PIN = 10;

IRrecv irrecv(RECV_PIN);
decode_results results;
CRGBArray<NUM_LEDS> leds;

bool codeCorrect = false;
bool isWon = false;
int numbersEntered = 0;


// HIER DEINEN GEHEIMCODE
int secretCode [3] = {5, 3, 9};



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

int decodeIR( int hex ) { // Indicate what key is pressed

  switch(hex) {

    case 0xFF6897:  
      return 1;
      break;
  
    case 0xFF9867:  
      return 2;
      break;
  
    case 0xFFB04F:  
      return 3;
      break;
  
    case 0xFF30CF:  
      return 4;
      break;
  
    case 0xFF18E7:  
      return 5;
      break;
  
    case 0xFF7A85:  
      return 6;
      break;
  
    case 0xFF10EF:  
      return 7;
      break;
  
    case 0xFF38C7:  
      return 8;
      break;
  
    case 0xFF5AA5:  
      return 9;
      break;
  
    case 0xFF4AB5:  
      return 0;
      break;

    case 0xFFFFFFFF:
      return -2;
      break;
  
    default: 
     return -1;
     break;
  }
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
 
void setup()
{
  Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
  FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);
  colorAll( 120, 0, 0 );
}

void loop() {
  if ( !isWon ) {
    if (irrecv.decode(&results)) {
    int in = decodeIR( results.value );
    enterCode( in );
    Serial.println("decode");
    irrecv.resume(); 
  }
  
  delay(5);
  }
}