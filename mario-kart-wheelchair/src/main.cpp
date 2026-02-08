#include <Arduino.h>
#include "SoundPlayer.h"

SoundPlayer player( TX, RX );

byte buffer_size = 18;
byte buffer[18];

void setup() {
    Serial.begin( 57600 );
    player.begin();
    delay( 50 );
}

void loop() {
    player.check();

    player.play_folder( 5 );

    delay( 2000 );
}
