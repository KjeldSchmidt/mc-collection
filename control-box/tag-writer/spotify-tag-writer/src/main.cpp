#include "Arduino.h"
#include <MFRC522.h>
#include <SPI.h>

#define RFID_CHIP_SELECT_PIN 15
#define RFID_RESET_PIN 16

#define BUFFER_LENGTH_FOR_SPOTIFY_ID 22
#define SPOTIFY_ID_LENGTH 22

MFRC522 rfid(RFID_CHIP_SELECT_PIN, RFID_RESET_PIN);
byte buffer[BUFFER_LENGTH_FOR_SPOTIFY_ID];
MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;
byte buffer_size = BUFFER_LENGTH_FOR_SPOTIFY_ID;

String spotifyIds[10] = {
  "6rDoJMiKUputhDBLDXhVI4",
  "19yVtAOeHin37T0c6caGrk",
  "39vNHJ3nDr6oO9mWLrRpsI",
  "5pnJrocLlZ3FWEbcr2PTz0",
  "0vVekV45lOaVKs6RZQQNob",
  "50d6Esrv3FnW4p4W6yT5fj",
  "3BIJimflJCEKdoMFSenabT",
  "6aylSUY5zgAR3NU1OGq9FI",
  "4INFVEhhhnLiqNR4MVPaX8",
  "1s6LKmXfmbYoM2SB1BjMYp",
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
    spotifyId += (char) buffer[i];
  }
  return spotifyId;
}

String readSpotifyId() {
  String spotifyId = "";
  status = rfid.MIFARE_Read(6, buffer, &buffer_size);
  for ( uint8_t i = 0; i < 16; i++ ) {
    spotifyId += (char) buffer[i];
  }
  
  status = rfid.MIFARE_Read(10, buffer, &buffer_size);
  for ( uint8_t i = 0; i < 6; i++ ) {
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

  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    Serial.println(rfid.PICC_GetTypeName(piccType));

  String spotifyId = spotifyIds[spotifyIdIndex];
  // convert spotifyId to byte array
  for (uint8_t i = 0; i < spotifyId.length(); i++) {
    buffer[i] = spotifyId[i];
  }
  
  // iterate over buffer in chunks of 4
  for (uint8_t buffer_offset = 0, page_to_write = 6; buffer_offset < SPOTIFY_ID_LENGTH; buffer_offset += 4, page_to_write++) {
    // write 4 bytes at a time
    status = rfid.MIFARE_Ultralight_Write(
      page_to_write,
      buffer + buffer_offset,
      4
    );
    Serial.print("Write status: ");
    Serial.println(rfid.GetStatusCodeName( status ));
  }

  Serial.println(readSpotifyId());


  spotifyIdIndex = (spotifyIdIndex + 1);

	delay( 1000 );
}


