#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// Replace with your network credentials
const char *ssid = "Heisse Singles in deinem WLAN";
const char *password = "Dauerwerbesendung";

void setup() {
	Serial.begin( 115200 );
	WiFi.mode( WIFI_STA );
	WiFi.begin( ssid, password );

	while ( WiFi.waitForConnectResult() != WL_CONNECTED ) {
		Serial.println( "Connection Failed! Rebooting..." );
		delay( 5000 );
		ESP.restart();
	}

	ArduinoOTA.onStart( []() {
		Serial.println( "Start" );
	} );
	ArduinoOTA.onEnd( []() {
		Serial.println( "\nEnd" );
	} );
	ArduinoOTA.onProgress( []( unsigned int progress, unsigned int total ) {
		Serial.printf( "Progress: %u%%\r", ( progress / ( total / 100 )));
	} );
	ArduinoOTA.onError( []( ota_error_t error ) {
		Serial.printf( "Error[%u]: ", error );
		if ( error == OTA_AUTH_ERROR ) Serial.println( "Auth Failed" );
		else if ( error == OTA_BEGIN_ERROR ) Serial.println( "Begin Failed" );
		else if ( error == OTA_CONNECT_ERROR ) Serial.println( "Connect Failed" );
		else if ( error == OTA_RECEIVE_ERROR ) Serial.println( "Receive Failed" );
		else if ( error == OTA_END_ERROR ) Serial.println( "End Failed" );
	} );
	ArduinoOTA.begin();
	Serial.println( "Ready" );
	Serial.print( "IP address: " );
	Serial.println( WiFi.localIP());
	Serial.println("Booted OTA Update!");
}


void loop() {
	ArduinoOTA.handle();
}