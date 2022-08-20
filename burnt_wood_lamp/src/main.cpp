#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "FastLED.h"

#include "colorModes/ColorMode.h"
#include "server/LampWebServer.h"
#include "led/LightManager.h"
#include "config/ConfigVariables.h"
#include "NotToBeCommited.h"

// LEDs
CRGB leds[MAX_LEDS];

LightManager *lightManager = new LightManager{ leds };
ESP8266WebServer *server = new ESP8266WebServer{ 80 };
LampWebServer *lampServer = new LampWebServer{ lightManager, server };

bool unhandledExceptionCaught = false;

void turnOffBuiltInLed() {
    pinMode( D4, OUTPUT );
    digitalWrite( D4, HIGH );
}

void setup() {
    turnOffBuiltInLed();
	CFastLED::addLeds<WS2812B, DATA_PIN, GRB>( leds, MAX_LEDS );
	//.setCorrection(CRGB(10,200,255));

	lightManager->updateLEDs();

	randomSeed( analogRead( A0 ));

	Serial.begin( 115200 );
	WiFi.mode( WIFI_STA );
	WiFi.begin( SSID, PASSWORD );


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
//	Serial.end();

/*
while ( true) {
		Serial.println(WiFi.isConnected() );
		WiFiClientSecure client;
		client.setInsecure(); 
		HTTPClient http;

		// Request
		http.begin(client, "https://obuymo6rdfdehomshnutqiyqd40mvmnf.lambda-url.eu-central-1.on.aws/ ");
		int httpResponseCode = http.GET();
		Serial.print("Response Code: ");
		Serial.println(httpResponseCode);
		if(httpResponseCode == 200){
					String payload = http.getString();
		Serial.print("Payload: ");
		Serial.println(payload);
		}
		delay( 5000 );
	}
	*/

	lampServer->initServer();
}


void loop() {
	// Serial.println( "Start next Loop");
	if ( !unhandledExceptionCaught ) {
		try {
		//	 Serial.println( "loop: handleClient");
			lampServer->handleClient();
		//	 Serial.println( "loop: updateLEDs");
			lightManager->updateLEDs();
		} catch ( std::exception &e ) {
			Serial.println( "Error in loop");
			unhandledExceptionCaught = true;
			delete lampServer;
			delete server;
			delete lightManager;
		}
	}
	else {
		Serial.println( "unhandledExceptionCaught");
	}
	// Serial.println( "LED Loop complete");
	ArduinoOTA.handle();
	// Serial.println( "OTA Loop complete");
}