#include <Arduino.h>

#define a1_pin = A1;
#define a2_pin = A2;
#define a3_pin = A3;
#define a4_pin = A4;
#define a5_pin = A5;
#define a6_pin = A6;
#define a7_pin = A7;


uint16_t a1, a2, a3, a4, a5, a6, a7;


void readAll() {
	a1 = analogRead(A1);
	a2 = analogRead(A2);
	a3 = analogRead(A3);
	a4 = analogRead(A4);
	a5 = analogRead(A5);
	a6 = analogRead(A6);
	a7 = analogRead(A7);
}

void serialPrintAll() {
	String out = "";
	out = out + a1;
	out = out + " ";
	out = out + a2;
	out = out + " ";
	out = out + a3;
	out = out + " ";
	out = out + a4;
	out = out + " ";
	out = out + a5;
	out = out + " ";
	out = out + a6;
	out = out + " ";
	out = out + a7;
	Serial.println( out );
}

void setup() {
	Serial.begin(250000);
}

void loop() {
	readAll();
	serialPrintAll();
	delay(5);
}