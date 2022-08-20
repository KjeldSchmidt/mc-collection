#ifndef BURNT_WOOD_LAMP_WEATHERMODE_H
#define BURNT_WOOD_LAMP_WEATHERMODE_H

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <string>
#include <iostream>

#include "ColorMode.h"
#include "NTPClient.h"
#include "../util/fetch/FetchUtil.h"
#include "../util/color/ColorUtil.h"
#include "NotToBeCommited.h"

class WeatherMode : public ColorMode {
public:
	uint16 Update( CRGB *leds_out, uint16 startLed, uint16 ledCount) override {
		int num = UpdateLEDs(leds_out, startLed, ledCount);
		return  (uint16) num;
	}
	
	constexpr static const char *getName() {
		return "WeatherMode";
	}

	WeatherMode() {
 		configTime(TIMEZONE, NTP_SERVER); 
	}

private:
	const CRGB rainColor = CRGB(0, 255, 255);
	const CRGB noRainColor = CRGB(255, 0, 0);

	// 1000 free requests on weather api per day so update every 15minutes will be -> 96/day
	const int fifteenMinutesInMs = 15 * 60 * 1000; //todo increase again
	const int loopDelayWithoutFetch = 1000;
	const int loopsUntilNextFetch = fifteenMinutesInMs / loopDelayWithoutFetch;
	const int loopDelayAfterInvalidFetchResponse = 10;
	const int loopDelayAfterSuccessfulFetchResponse = 1000;
	float rainsCountToLEDRatio = -1;

	const bool debugIsOn = false;

	int loopsSinceLastFetch = loopsUntilNextFetch;
	std::vector<bool> cachedRains;

	int UpdateLEDs(CRGB *leds_out, uint16 startLed, uint16 ledCount) {
		if(rainsCountToLEDRatio == -1){
			rainsCountToLEDRatio = (float)24 / (float)ledCount;
		}

		PrintUpdateLEDs();
		loopsSinceLastFetch++;

		bool shouldFetch = FetchUtil::ShouldFetch(debugIsOn, cachedRains.size(), loopsSinceLastFetch, loopsUntilNextFetch);
		if(shouldFetch){
			return FetchServer(SERVER_NAME_WEATHER);
		}
		WeatherForecast(leds_out, cachedRains, startLed, startLed + ledCount, true); 

		// 1000 free requests on weather api per day so update every 15 minutes will be -> 4*24 => 96/day
		return loopDelayAfterSuccessfulFetchResponse;
	}
	
	void PrintUpdateLEDs() {
	if(!debugIsOn){
		return;
	}
		 Serial.println("--- WeaterMode: UpdateLEDs ---");
	}


	int FetchServer(String serverName) {
		// Create
		WiFiClientSecure client;
		client.setInsecure(); 
		HTTPClient http;

		// Request
		http.begin(client, serverName);

		int httpResponseCode = http.GET();
		PrintFetchServer(httpResponseCode);

		if(httpResponseCode == -1){
			return loopDelayAfterInvalidFetchResponse;
		}		String payload = http.getString();

		std::string clampedPayload = convertPayload(payload);
		std::vector<bool> rains = strToRains(clampedPayload);
		cachedRains = rains;
		loopsSinceLastFetch = 0;
		// Serial.print("Payload: ");
		// Serial.println(payload);

		return loopDelayAfterSuccessfulFetchResponse;
	}

	void PrintFetchServer(int httpResponseCode) {
		if(!debugIsOn){
			return;
		}

		Serial.print("FetchServer: ");
		Serial.println(SERVER_NAME_WEATHER);
		Serial.print("Response Code: ");
		Serial.println(httpResponseCode);
	}

	std::string convertPayload(String payload){
		// Remove []
		payload.remove(0, 1);
		int length = payload.length();
		payload.remove(length-1, 1);

		PrintConvertPayload(payload);

		return payload.c_str();
	}

	void PrintConvertPayload(String payload) {
		if(!debugIsOn){
			return;
		}
		
		 Serial.println("Removed Brackets");
		 Serial.println(payload);
	}


	std::vector<bool> strToRains(std::string str){
		int8_t index = 0;
		std::vector<bool> rains = {};
	  	for (char const c: str) {
			//Serial.println(c);
			if(c == ','){
				if(index >= 47){
					return rains;
				}
				index++;
			}
			else if(index > 23){
				bool rain = c == '1';
				//Serial.print("rain: ");
			//	Serial.println(rain);
				PrintStrRains(rain, index);
				rains.push_back(rain);
			}
		}	
		return rains;	
	}

	void PrintStrRains(bool rain, int8 index) {
		if(!debugIsOn){
			return;
		}

		Serial.print("StrRains - rain: ");
		Serial.print(rain);
		Serial.print(", index: ");
		Serial.println(index);
	}

	// Display Weather forcast for current Day
	void WeatherForecast(CRGB *leds_out, std::vector<bool> rainsForNext24Hours, uint16 startLed, uint16 endLED, bool isOn = true) {
		if(!isOn){
			ColorUtil::SetSingleColor(leds_out, CRGB(0,0,0), startLed, endLED);
			return;
		}

		// Config
		uint16_t ledCount = endLED - startLed;

		// Ratio
		uint16_t rainsCount = rainsForNext24Hours.size();
	
		// rainsForNext24Hours = {true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,true,false,false,false,false,false,false};
		bool willRain6 = rainsForNext24Hours[0] || rainsForNext24Hours[1] || rainsForNext24Hours[2] || rainsForNext24Hours[3] || rainsForNext24Hours[4] || rainsForNext24Hours[5];
		bool willRain12 = rainsForNext24Hours[6] || rainsForNext24Hours[7] || rainsForNext24Hours[8] || rainsForNext24Hours[9] || rainsForNext24Hours[10] || rainsForNext24Hours[11];
		bool willRain18 = rainsForNext24Hours[12] || rainsForNext24Hours[13] || rainsForNext24Hours[14] || rainsForNext24Hours[15] || rainsForNext24Hours[16] || rainsForNext24Hours[17];
		bool willRain24 = rainsForNext24Hours[18] || rainsForNext24Hours[19] || rainsForNext24Hours[20] || rainsForNext24Hours[21] || rainsForNext24Hours[22] || rainsForNext24Hours[23];

		double quarterCount = (double)ledCount / (double)4;
		uint16_t ledCountQuarter1Max = floor(1 * quarterCount);
		uint16_t ledCountQuarter2Max = floor(2 * quarterCount);
		uint16_t ledCountQuarter3Max = floor(3 * quarterCount);
		uint16_t ledCountQuarter4Max = floor(4 * quarterCount);

	//	for ( uint16_t i = 0; i < rainsForNext24Hours.size(); i++ ) {
	//		Serial.print(i);
	//		Serial.print(": ");
	//		Serial.print(rainsForNext24Hours[i]);
	//		Serial.print(", ");
	//	}
	//	Serial.println();
	//	Serial.print("temp6-24: ");
	//	Serial.print(willRain6);
	//	Serial.print(", ");
	//	Serial.print(willRain12);
	//	Serial.print(", ");
	//	Serial.print(willRain18);
	//	Serial.print(", ");
	//	Serial.print(willRain24);
	//	Serial.println();

		for ( uint16_t i = startLed; i < endLED; i++ ) {
		
			float rainIndexF1 = (float)i * (float)rainsCountToLEDRatio;
			uint8 rainIndex1 = floor(rainIndexF1);

			if(rainIndex1 < 0){
				rainIndex1 = 0;
			}

			bool willRain;
			uint8_t averageTemperature;
			if(i <= startLed + ledCountQuarter1Max){
				willRain = willRain6;
			}else if(i <= startLed + ledCountQuarter2Max){
				willRain = willRain12;
			}else if(i <= startLed + ledCountQuarter3Max){
				willRain = willRain18;
			}else {
				willRain = willRain24;
			}
			 if(willRain == 1) {
				ColorUtil::SafeSetColor(leds_out, i, rainColor);
			 } else {
				ColorUtil::SafeSetColor(leds_out, i, noRainColor);
			 }
		}

	}

};

#endif