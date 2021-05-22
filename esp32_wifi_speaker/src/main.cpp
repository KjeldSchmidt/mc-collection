#include <Arduino.h>
#include <WiFi.h>
#include <driver/dac.h>

const char *ssid = "Heisse Singles in deinem WLAN";
const char *password = "Dauerwerbesendung";
const char *host = "192.168.178.25";

WiFiClient client;

hw_timer_t *timer = nullptr;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

#define BUFFFERMAX 8000
#define speakerPin DAC_CHANNEL_2

uint8_t dataBuffer[BUFFFERMAX];
int readPointer = 0, writePointer = 1;

bool play = false;

int getDistance() {
	int distance = 0;
	if ( readPointer < writePointer ) distance = BUFFFERMAX - writePointer + readPointer;
	else if ( readPointer > writePointer ) distance = readPointer - writePointer;
	return distance;
}

void IRAM_ATTR onTimer() {
	portENTER_CRITICAL_ISR( &timerMux );

	// play data:
	if ( play ) {
		dac_output_voltage( speakerPin, dataBuffer[ readPointer ] );

		readPointer++;
		if ( readPointer == BUFFFERMAX ) {
			readPointer = 0;
		}

		if ( getDistance() == 0 ) {
			Serial.println( "Buffer underrun!" );
			play = false;
		}
	}

	portEXIT_CRITICAL_ISR( &timerMux );
}

void setup() {
	Serial.begin( 115200 );

	dac_output_enable( speakerPin );
	pinMode( 33, INPUT_PULLUP );
	pinMode( 32, INPUT_PULLUP );

	WiFi.mode( WIFI_STA );
	WiFi.begin( ssid, password );

	while ( WiFi.status() != WL_CONNECTED ) {
		delay( 500 );
		Serial.print( "." );
	}

	Serial.println( "" );
	Serial.println( "WiFi connected" );
	Serial.println( "IP address: " );
	Serial.println( WiFi.localIP());


	const int port = 4444;
	while ( !client.connect( host, port )) {
		Serial.println( "connection failed" );
		delay( 1000 );
	}
	Serial.println( "Audio server connection success" );

	timer = timerBegin( 0, 2, true ); // use a prescaler of 2
	timerAttachInterrupt( timer, &onTimer, true );
	timerAlarmWrite( timer, 5000, true );
	timerAlarmEnable( timer );


}

void loop() {
	int distance = getDistance();
	if ( distance <= 800 ) play = true;

	if ( distance >= 800 ) {
		client.write( 255 ); // send the command to send new data

		// read new data:
		while ( client.available() == 0 );
		while ( client.available() >= 1 ) {
			uint8_t value = client.read();
			dataBuffer[ writePointer ] = value;
			writePointer++;
			if ( writePointer == BUFFFERMAX ) writePointer = 0;
		}

	}

}