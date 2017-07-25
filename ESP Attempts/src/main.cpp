
#import "Arduino.h"
#include <ESP8266WiFi.h>

WiFiServer server(80);

#define LED_PIN 2

void setup() {
    Serial.begin( 9600 );
    WiFi.mode( WIFI_AP );
    WiFi.softAP( "D1 Mini", "12345678" );
    server.begin();
    IPAddress server_ip = WiFi.softAPIP();
    Serial.print( "Server operating at IP ");
    Serial.println( server_ip );
}

void loop() {
    WiFiClient client = server.available();
    if ( !client ) {
        return;
    }

    Serial.println( "A device has connected!" );
}