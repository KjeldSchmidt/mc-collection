#include <Arduino.h>

// Wiring Information
// Label on Copper: Digital Pin
// 1: 11
// 2: 9
// 3: 7
// 4: 5
// 5: 3
// 6: 10
// 7: 12
// 8: 8
// 9: 6

#define BEDROOM_LIGHTS_PIN 11
#define BED_WALL_LIGHTS_PIN 9
#define HALLWAY_LIGHTS_PIN 7
#define WINDOW_WALL_LIGHTS_PIN 5
#define LIVING_ROOM_LIGHTS_PIN 3
#define VOLUME_DOWN_PIN 10
#define PREVIOUS_SONG_PIN 12
#define PLAY_PAUSE_PIN 8
#define NEXT_SONG_PIN 6

bool bedroomLightsPressed = false;
bool bedWallLightsPressed = false;
bool hallwayLightsPressed = false;
bool windowWallLightsPressed = false;
bool livingRoomLightsPressed = false;
bool volumeDownPressed = false;
bool previousSongPressed = false;
bool playPausePressed = false;
bool nextSongPressed = false;

bool printOnRisingEdge(const int pin, bool &flag, const char *message) {
  if (digitalRead(pin) == HIGH) {
    if (!flag) {
      flag = true;
      Serial.println(message);
    }
  } else {
    flag = false;
  }
  return flag;
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  printOnRisingEdge(BEDROOM_LIGHTS_PIN, bedroomLightsPressed, "BEDROOM_LIGHTS");
  printOnRisingEdge(BED_WALL_LIGHTS_PIN, bedWallLightsPressed, "BED_WALL_LIGHTS");
  printOnRisingEdge(HALLWAY_LIGHTS_PIN, hallwayLightsPressed, "HALLWAY_LIGHTS");
  printOnRisingEdge(WINDOW_WALL_LIGHTS_PIN, windowWallLightsPressed, "WINDOW_WALL_LIGHTS");
  printOnRisingEdge(LIVING_ROOM_LIGHTS_PIN, livingRoomLightsPressed, "LIVING_ROOM_LIGHTS");
  printOnRisingEdge(VOLUME_DOWN_PIN, volumeDownPressed, "VOLUME_DOWN");
  printOnRisingEdge(PREVIOUS_SONG_PIN, previousSongPressed, "PREVIOUS_SONG");
  printOnRisingEdge(PLAY_PAUSE_PIN, playPausePressed, "PLAY_PAUSE");
  printOnRisingEdge(NEXT_SONG_PIN, nextSongPressed, "NEXT_SONG");

  delay(10);
}
