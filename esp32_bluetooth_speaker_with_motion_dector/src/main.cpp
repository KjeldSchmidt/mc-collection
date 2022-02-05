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

volatile bool motion_triggered = false;

void motion_detected() {
	motion_triggered = true;
	Serial.println("MD");
}

void handle_night_event() {
	Serial.println("handle_night_event");
}

void handle_day_event() {
	Serial.println("Handle day event!");
	uint8_t random_value = random( 200 );
	Serial.println(random_value);
	player.play_random_from_folder( random_value == 0 ? 2 : 1 );
}

void setup() {
	delay( 2000 );
	Serial.begin( 9600 );

	randomSeed(analogRead(A0));
	sdPlayerControlStream.begin( SoundPlayer::SERIAL_BPS );
	player.begin();
	
	pinMode( MOTION_DETECTION_PIN, INPUT_PULLDOWN );
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

	Serial.println("checking for motion...");
	if ( motion_triggered ) {
		Serial.println("Found motion!");
		motion_triggered = false;

		time_t t = time( NULL );;
		struct tm *tm = localtime( &t );

		if ( tm->tm_hour < 7 or tm->tm_hour >= 22 ) {
			handle_night_event();
		} else {
			handle_day_event();
		}
	}

	delay( 1000 );
}