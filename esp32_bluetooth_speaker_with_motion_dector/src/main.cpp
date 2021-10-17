#include <Arduino.h>
#include "../lib/ESP32-A2DP-1.4.0/src/BluetoothA2DPSink.h"

#define MOTION_DETECTION_PIN 17

BluetoothA2DPSink a2dp_sink;

void motion_detected() {
	Serial.println( "Motion detected!" );
}

void setup() {
	a2dp_sink.start( "Dschungelmusik" );
	Serial.begin( 9600 );
	pinMode( 17, INPUT );
	attachInterrupt(
			digitalPinToInterrupt( MOTION_DETECTION_PIN ),
			motion_detected,
			RISING
	);
}

void loop() {
}