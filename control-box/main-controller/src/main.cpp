#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include "Secrets.h"

#include "Arduino.h"
#include <MFRC522.h>
#include <SPI.h>

// RFID Config
#define RFID_CHIP_SELECT_PIN 15
#define RFID_RESET_PIN 16
#define BUFFER_LENGTH_FOR_SPOTIFY_ID 22
#define SPOTIFY_ID_LENGTH 22

MFRC522 rfid(RFID_CHIP_SELECT_PIN, RFID_RESET_PIN);
byte buffer[BUFFER_LENGTH_FOR_SPOTIFY_ID];
MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;
byte buffer_size = BUFFER_LENGTH_FOR_SPOTIFY_ID;

const uint8_t ITERATIONS_WITHOUT_CARD_BEFORE_NEW_READ = 20;
uint8_t noCardCount = 0;

// Network Config
const char* ssid     = WIFI_SSID;
const char* password = WIFI_PASSWORD;
String control_server_base_url = "http://192.168.178.2:5000";

void sendRequest(String url) {
  WiFiClient client;
      HTTPClient http;
      http.begin(client, url );
      int httpCode = http.GET();
      if (httpCode > 0) {
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
        if (httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
}

void setUnprotectedRfidKey() {
  for ( byte i = 0; i < 6; i++ ) { 
		key.keyByte[ i ] = 0xFF;
	}
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

void handleRfidTag() {
  if ( !rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    noCardCount++;
    noCardCount = min(noCardCount, ITERATIONS_WITHOUT_CARD_BEFORE_NEW_READ);
    Serial.println("No card seen in " + String(noCardCount) + " iterations.");
		return;
	}

  if (noCardCount < ITERATIONS_WITHOUT_CARD_BEFORE_NEW_READ) {
    noCardCount = 0;
    Serial.println("Seen card, but only " + String(noCardCount) + " iterations since last card read.");
    return;
  }
  noCardCount = 0;
  
  Serial.println("Seen card, reading spotify ID.");

  String spotifyId = readSpotifyId();
  if (spotifyId.length() == SPOTIFY_ID_LENGTH) {
    Serial.println("Spotify ID: " + spotifyId);
    sendRequest(control_server_base_url + "/spotify/play/album/" + spotifyId);
  }
}

void setup() {
  Serial.begin(9600);

  while (!Serial) { 
    delay(10);
  }

  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi " + String(ssid) + "...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  SPI.begin();
	rfid.PCD_Init();
	setUnprotectedRfidKey();
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    Serial.println("Received: " + input);

    if ( input == "BEDROOM_LIGHTS" ) {
      Serial.println("Executing: BEDROOM_LIGHTS");
      sendRequest(control_server_base_url + "/ceiling/bedroom/toggle");
    }
    if ( input == "BED_WALL_LIGHTS" ) {
      Serial.println("Executing: BED_WALL_LIGHTS");
      sendRequest(control_server_base_url + "/woodlamp/bedLamp/toggle");
    }
    if ( input == "HALLWAY_LIGHTS" ) {
      Serial.println("Executing: HALLWAY_LIGHTS");
      sendRequest(control_server_base_url + "/ceiling/hall/toggle");
    }
    if ( input == "WINDOW_WALL_LIGHTS" ) {
      Serial.println("Executing: WINDOW_WALL_LIGHTS");
      sendRequest(control_server_base_url + "/woodlamp/beamLamp/toggle");
    }
    if ( input == "LIVING_ROOM_LIGHTS" ) {
      Serial.println("Executing: LIVING_ROOM_LIGHTS");
      sendRequest(control_server_base_url + "/ceiling/living%20room/toggle");
    }
    if ( input == "VOLUME_DOWN" ) {
      Serial.println("Executing: VOLUME_DOWN");
      sendRequest(control_server_base_url + "/spotify/volume-down");
    }
    if ( input == "VOLUME_UP" ) {
      Serial.println("Executing: VOLUME_UP");
      sendRequest(control_server_base_url + "/spotify/volume-up");
    }
    if ( input == "PREVIOUS_SONG" ) {
      Serial.println("Executing: PREVIOUS_SONG");
      sendRequest(control_server_base_url + "/spotify/previous");
    }
    if ( input == "PLAY_PAUSE" ) {
      Serial.println("Executing: PLAY_PAUSE");
      sendRequest(control_server_base_url + "/spotify/play-pause");
    }
    if ( input == "NEXT_SONG" ) {
      Serial.println("Executing: NEXT_SONG");
      sendRequest(control_server_base_url + "/spotify/next");
    }
  }

  handleRfidTag();
  delay( 50 );
}