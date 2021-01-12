#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "FastLED.h"

#define MAX_BRIGHTNESS 72
#define MAX_BRIGHTNESS_PER_CHANNEL 24
#define NUM_LEDS 109
#define NUM_LEDS_LOWER 55
#define NUM_LEDS_UPPER

#include "ColorMode.h"


#define CHIPSET WS2812
#define COLOR_ORDER GRB

#define DATA_PIN D3

CRGB leds[NUM_LEDS];

// Replace with your network credentials
const char *ssid = "Heisse Singles in deinem WLAN";
const char *password = "Dauerwerbesendung";

ColorMode *colorMode = new CityAtSundown{};

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


	CFastLED::addLeds<WS2812B, DATA_PIN, GRB>( leds, NUM_LEDS );
}


void loop() {
	ArduinoOTA.handle();
	int delay_time = colorMode->Update( leds );
	FastLED.show();
	delay( delay_time );
}