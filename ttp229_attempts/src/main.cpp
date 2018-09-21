#include <Arduino.h>

void setup() {
	Serial.begin(9600);
	pinMode(4, INPUT);
	pinMode(5, INPUT);
	pinMode(6, INPUT);
	pinMode(7, INPUT);
	pinMode(8, INPUT);
	pinMode(9, INPUT);
	pinMode(10, INPUT);
	pinMode(11, INPUT);
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

void loop() {
	int value = read();
	if (value != -1) {
		Serial.println( value );
	}
}

