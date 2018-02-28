#include <Arduino.h>

void setup() {
	Serial.begin(9600);
	pinMode(13, OUTPUT);
	delay(1000);
}

void answer( char in ) {
	if ( in == 67 ) {
		digitalWrite(13, HIGH);
	} else if ( in == 66) {
		digitalWrite(13, LOW);
	}
}

void loop() {
	if ( Serial.available() > 0 ) {
		char messageReceived = Serial.read();
		answer( messageReceived );
	} 
	delay(100);
}