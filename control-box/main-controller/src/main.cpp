#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include "Secrets.h"

// Connect to WiFi network
const char* ssid     = WIFI_SSID;
const char* password = WIFI_PASSWORD;
String control_server_base_url = "http://192.168.178.2:5000";

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
}

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
}