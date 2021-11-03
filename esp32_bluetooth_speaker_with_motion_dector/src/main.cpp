#include <Arduino.h>
#include "../lib/ESP32-A2DP-1.4.0/src/BluetoothA2DPSink.h"
#include "../.pio/libdeps/wemos_d1_mini32/EspSoftwareSerial/src/SoftwareSerial.h"

#ifndef USE_SOFTWARESERIAL
#define USE_SOFTWARESERIAL 1   ///< Set to 1 to use SoftwareSerial library, 0 for native serial port
#endif

#include "SoundPlayer.h"
#include "RadioServer.h"

#define MOTION_DETECTION_PIN 17
#define SD_PLAYER_TX 18
#define SD_PLAYER_RX 19

BluetoothA2DPSink a2dp_sink;
SoftwareSerial sdPlayerControlStream{ SD_PLAYER_RX, SD_PLAYER_TX };
SoundPlayer player{ sdPlayerControlStream };
RadioServer radioServer;

volatile bool play = false;

void motion_detected() {
	Serial.println( "Motion detected!" );
	play = true;
}

void setup() {
	a2dp_sink.start( "Dschungelmusik" );
	Serial.begin( 9600 );
	sdPlayerControlStream.begin( SoundPlayer::SERIAL_BPS );
	pinMode( 17, INPUT );

	attachInterrupt(
			digitalPinToInterrupt( MOTION_DETECTION_PIN ),
			motion_detected,
			RISING
	);

	player.begin();
}

void loop() {
	delay( 2000 );
	player.check();

	if ( play ) {
		play = false;

		player.play_random_from_folder( 2 );
	}
}