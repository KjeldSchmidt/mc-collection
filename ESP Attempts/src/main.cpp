/*
 * This sketch starts a local wifi server, which serves a web page with two buttons
 * These buttons can be used to pull a pin high or low, controlling, for example, an LED
 */

#include "Arduino.h" // Not neccesary when using the Aruino IDE
#include <ESP8266WiFi.h>

WiFiServer server(80);

#define CONTROLLED_PIN D2 // Normal numbrs of arduino pins do not apply for for the ESP8266

void setup() {
    Serial.begin( 9600 );
    WiFi.mode( WIFI_AP );
    WiFi.softAP( "D1 Mini", "12345678" ); // Wifi name and password
    server.begin();
    IPAddress server_ip = WiFi.softAPIP();
    Serial.print( "Server operating at IP ");
    Serial.println( server_ip );

    pinMode( CONTROLLED_PIN, OUTPUT );
    digitalWrite( CONTROLLED_PIN, HIGH );
}

void loop() {
    WiFiClient client = server.available(); // Check if a request to the server is made
    if ( !client ) { // If not, do nothing.
        return; 
    }
    Serial.println( "A device has connected!" );

    // If the requested url contains "HIGH" or "LOW", set the conrolled pin accordingly
    String request = client.readStringUntil('\r');
    if ( request.indexOf("HIGH") != -1 ) {
        digitalWrite( CONTROLLED_PIN, HIGH );
    } else if ( request.indexOf("LOW") != -1 ) {
        digitalWrite( CONTROLLED_PIN, LOW );
    }

    // Output web page content
    String controlHTML = "HTTP/1.1 200 OK\r\n";
    controlHTML += "Content-Type: text/html\r\n\r\n";
    controlHTML += "<!DOCTYPE HTML>\r\n<html>\r\n";
    controlHTML += "<button onclick=\"location.href = '/HIGH'\">HIGH</button>\r\n";
    controlHTML += "<button onclick=\"location.href = '/LOW'\">LOW</button>\r\n";
    controlHTML += "</html>\r\n";

    client.print(controlHTML);
}
