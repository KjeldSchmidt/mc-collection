#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include "SoundPlayer.h"
#include "Secrets.h"

SoundPlayer player( TX, RX );

byte buffer_size = 18;
byte buffer[18];

void setup() {
    player.begin();
    delay( 50 );

    WiFi.mode( WIFI_STA );
    WiFi.begin( ssid, password );
    while ( WiFi.waitForConnectResult() != WL_CONNECTED ) {
        delay( 5000 );
    }

    ArduinoOTA.setHostname( "mario-kart-wheelchair" );
    ArduinoOTA.begin();
}

void loop() {
    ArduinoOTA.handle();
    player.check();

    player.play_folder( 5 );

    delay( 10000 );
}
