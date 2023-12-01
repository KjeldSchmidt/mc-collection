#include "Arduino.h"
#include <MFRC522.h>
#include <SPI.h>

#define RFID_CHIP_SELECT_PIN 15
#define RFID_RESET_PIN 16

#define BUFFER_LENGTH_FOR_SPOTIFY_ID 26

MFRC522 rfid(RFID_CHIP_SELECT_PIN, RFID_RESET_PIN);
byte buffer[BUFFER_LENGTH_FOR_SPOTIFY_ID];
MFRC522::MIFARE_Key key;
byte buffer_size = BUFFER_LENGTH_FOR_SPOTIFY_ID;
String spotifyIds[10] = {
  "    6rDoJMiKUputhDBLDXhVI4",
  "    19yVtAOeHin37T0c6caGrk",
  "    39vNHJ3nDr6oO9mWLrRpsI",
  "    5pnJrocLlZ3FWEbcr2PTz0",
  "    0vVekV45lOaVKs6RZQQNob",
  "    50d6Esrv3FnW4p4W6yT5fj",
  "    3BIJimflJCEKdoMFSenabT",
  "    6aylSUY5zgAR3NU1OGq9FI",
  "    4INFVEhhhnLiqNR4MVPaX8",
  "    1s6LKmXfmbYoM2SB1BjMYp",
};

uint8_t spotifyIdIndex = 0;

void setUnprotectedRfidKey() {
  for ( byte i = 0; i < 6; i++ ) { 
		key.keyByte[ i ] = 0xFF;
	}
}

String extractSpotifyIdFromBuffer() {
  String spotifyId = "";
  for ( uint8_t i = 0; i < buffer_size; i++ ) {
    if ( i < 4 ) {
      continue;
    }
    spotifyId += (char) buffer[i];
  }
  return spotifyId;
}

void setup() {
  Serial.begin( 9600 );

	SPI.begin();
	rfid.PCD_Init();
	setUnprotectedRfidKey();
}

void loop() {
  if ( !rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
		delay( 50 );
		return;
	}

  // copy the spotify id into the buffer
  for ( uint8_t i = 0; i < spotifyIds[spotifyIdIndex].length(); i++ ) {
    buffer[i] = spotifyIds[spotifyIdIndex][i];
  }
  
  // Writing the buffer in chunks of 4 bytes, because apparently that is how it works
  for (uint8_t i = 0; i < buffer_size; i += 4) {
    rfid.MIFARE_Ultralight_Write(11 + i, buffer + i, 4);
  }

  rfid.MIFARE_Read(0, buffer, &buffer_size);
  Serial.println(extractSpotifyIdFromBuffer());
  spotifyIdIndex = spotifyIdIndex + 1; 

	delay( 1000 );
}