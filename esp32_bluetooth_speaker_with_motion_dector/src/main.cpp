#include <Arduino.h>
#include "../lib/ESP32-A2DP-1.4.0/src/BluetoothA2DPSink.h"

BluetoothA2DPSink a2dp_sink;

void setup() {
	a2dp_sink.start( "Dschungelmusik" );
	pinMode( 21, OUTPUT );
}

void loop() {
	digitalWrite( 21, HIGH );
	delay( 500 );
	digitalWrite( 21, LOW );
	delay( 500 );
}