
#include "Arduino.h"
#include <ESP8266WiFi.h>

WiFiServer server(80);

#define CONTROLLED_PIN D2

void setup() {
    Serial.begin( 9600 );
    WiFi.mode( WIFI_AP );
    WiFi.softAP( "D1 Mini", "12345678" );
    server.begin();
    IPAddress server_ip = WiFi.softAPIP();
    Serial.print( "Server operating at IP ");
    Serial.println( server_ip );

    pinMode( CONTROLLED_PIN, OUTPUT );
    digitalWrite( CONTROLLED_PIN, HIGH );
}

void loop() {
    WiFiClient client = server.available();
    if ( !client ) {
        return;
    }
    Serial.println( "A device has connected!" );

    String request = client.readStringUntil('\r');
    if ( request.indexOf("HIGH") != -1 ) {
        digitalWrite( CONTROLLED_PIN, HIGH );
    } else if ( request.indexOf("LOW") != -1 ) {
        digitalWrite( CONTROLLED_PIN, LOW );
    }

    String controlHTML = "HTTP/1.1 200 OK\r\n";
    controlHTML += "Content-Type: text/html\r\n\r\n";
    controlHTML += "<!DOCTYPE HTML>\r\n<html>\r\n";
    controlHTML += "<button onclick=\"location.href = '/HIGH'\">HIGH</button>\r\n";
    controlHTML += "<button onclick=\"location.href = '/LOW'\">LOW</button>\r\n";
    controlHTML += "</html>\r\n";

    client.print(controlHTML);
}