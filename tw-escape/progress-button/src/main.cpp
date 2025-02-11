#include "FastLED.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include <ArduinoOTA.h>
#include "Secrets.h"

#define XSTR(x) STR(x)
#define STR(x) #x

CRGB leds[NUM_LEDS];
CRGB colors[] = {CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Yellow};
Servo servo;

ESP8266WebServer server(80);

void setupAP() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    
    Serial.println("\nAccess Point Started");
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());  // Usually 192.168.4.1
}

void handleLights() {
    if (!server.hasArg("hex")) {
        server.send(400, "text/plain", "Use /lights?hex=FFFFFF (hex color code)");
        return;
    }
    
    String hex = server.arg("hex");
    if (hex.length() != 6) {
        server.send(400, "text/plain", "Invalid hex color. Use 6 characters (e.g. FF0000 for red)");
        return;
    }
    
    // Convert hex to RGB
    uint32_t color = strtoul(hex.c_str(), NULL, 16);
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;
    
    // Set all LEDs to the specified color
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(r, g, b);
    }
    FastLED.show();
    
    server.send(200, "text/plain", "LEDs set to #" + hex);
}

void handleOpen() {
    servo.write(170);
    server.send(200, "text/plain", "Servo Opened");
}

void handleClose() {
    servo.write(35);
    server.send(200, "text/plain", "Servo Closed");
}

void handleServo() {
    // Check if 'angle' parameter exists
    if (!server.hasArg("angle")) {
        server.send(400, "text/plain", "Missing angle parameter");
        return;
    }
    
    // Get and validate the angle
    int angle = server.arg("angle").toInt();
    if (angle < 0 || angle > 180) {
        server.send(400, "text/plain", "Angle must be between 0 and 180");
        return;
    }
    
    servo.write(angle);
    server.send(200, "text/plain", "Servo set to " + String(angle) + " degrees");
}

void setupServer() {
    server.on("/lights", HTTP_GET, handleLights);
    server.on("/open", HTTP_GET, handleOpen);
    server.on("/close", HTTP_GET, handleClose);
    server.on("/servo", HTTP_GET, handleServo);
    
    server.begin();
    Serial.println("HTTP server started");
}

void setupOTA() {
    ArduinoOTA.setHostname("D1Mini");
    
    ArduinoOTA.onStart([]() {
        Serial.println("Starting OTA update...");
    });
    
    ArduinoOTA.onEnd([]() {
        Serial.println("\nOTA update complete!");
    });
    
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    
    ArduinoOTA.begin();
    Serial.println("OTA ready");
}

void setup() {
    CFastLED::addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(MAX_BRIGHTNESS);

    randomSeed(analogRead(A0));
    Serial.begin(115200);
    Serial.println();
    Serial.print("Chipset: "); Serial.println(XSTR(CHIPSET));
    Serial.print("NUM_LEDS: "); Serial.println(NUM_LEDS);

    setupAP();
    setupOTA();
    setupServer();
    
    servo.attach(D4);
}

bool isInQuadrant(int ledIndex, int quadrant) {
    // Outer ring (24 LEDs, indices 0-23)
    if (ledIndex < 24) {
        int section = ledIndex / 6;
        return section == quadrant;
    }
    
    // Second ring (16 LEDs, indices 24-39)
    if (ledIndex < 40) {
        int section = (ledIndex - 24) / 4;
        return section == quadrant;
    }
    
    // Third ring (12 LEDs, indices 40-51)
    if (ledIndex < 52) {
        int section = (ledIndex - 40) / 3;
        return section == quadrant;
    }
    
    // Fourth ring (8 LEDs, indices 52-59)
    if (ledIndex < 60) {
        int section = (ledIndex - 52) / 2;
        return section == quadrant;
    }
    
    // Center LED (index 60)
    return true;  // Center LED can be part of any quadrant
}

void loop() {
    // Handle OTA updates
    ArduinoOTA.handle();
    
    // Handle HTTP requests
    server.handleClient();
}