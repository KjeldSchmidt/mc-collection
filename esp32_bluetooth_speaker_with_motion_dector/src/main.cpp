#include <Arduino.h>
#include "SoftwareSerial.h"

#ifndef USE_SOFTWARESERIAL
#define USE_SOFTWARESERIAL 1   ///< Set to 1 to use SoftwareSerial library, 0 for native serial port
#endif

#include "SoundPlayer.h"
#include "DynaConnect.h"

#define MOTION_DETECTION_PIN 17
#define SD_PLAYER_TX 18
#define SD_PLAYER_RX 19

WebServer Server;
AutoConnect Portal( Server );
AutoConnectConfig Config("Dschungelmusik", "");

SoftwareSerial sdPlayerControlStream{ SD_PLAYER_RX, SD_PLAYER_TX };
SoundPlayer player{ sdPlayerControlStream };
DynaConnect *dynaConnect;

volatile bool play = false;

void motion_detected() {
	Serial.println( "Motion detected!" );
	play = true;
}

void setup() {
	delay( 1000 );
	Serial.begin( 9600 );

	sdPlayerControlStream.begin( SoundPlayer::SERIAL_BPS );
	player.begin();
	
	pinMode( MOTION_DETECTION_PIN, INPUT );
	attachInterrupt(
			digitalPinToInterrupt( MOTION_DETECTION_PIN ),
			motion_detected,
			RISING
	);

	Config.autoReconnect = true;
	Config.hostName = "Dschungelmusik";

	Portal.config( Config );

	dynaConnect = new DynaConnect{ Server, Portal };
}

void loop() {
	player.check();
	dynaConnect->handle_client();


	if ( play ) {
		play = false;

		player.play_random_from_folder( 2 );
	}

	delay( 1000 );
}