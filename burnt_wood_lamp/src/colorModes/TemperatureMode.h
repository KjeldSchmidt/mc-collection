#ifndef BURNT_WOOD_LAMP_TEMPERATUREMODE_H
#define BURNT_WOOD_LAMP_TEMPERATUREMODE_H

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

class TemperatureMode : public ColorMode {
public:
	uint16 Update( CRGB *leds_out, uint16 startLED, uint16 ledCount) override {
		int num = UpdateLEDs(leds_out, startLED, ledCount);
		// Serial.println(num);
		return  (uint16) num;
	}
	
	constexpr static const char *getName() {
		return "TemperatureMode";
	}

	TemperatureMode() {
 		configTime(TIMEZONE, NTP_SERVER); 
	}

private:
	const CRGB gradientColor1 = CRGB(0,     0, 255);
	const CRGB gradientColor2 = CRGB(0,   255, 255);
	const CRGB gradientColor3 = CRGB(0,   255,   0);
	const CRGB gradientColor4 = CRGB(255, 255,   0);
	const CRGB gradientColor5 = CRGB(255,   0,   0);
	const CRGB gradientColor6 = CRGB(255,   0, 255);

	// 1000 free requests on weather api per day so update every 15minutes will be -> 96/day
	const int fifteenMinutesInMs = 15 * 60 * 1000; //todo increase again
	const int loopDelayWithoutFetch = 1000;
	const int loopsUntilNextFetch = fifteenMinutesInMs / loopDelayWithoutFetch;
	const int loopDelayAfterInvalidFetchResponse = 10;
	const int loopDelayAfterSuccessfulFetchResponse = 1000;

	const bool debugIsOn = false;

	std::vector<int8> cachedTemperatures;
	int loopsSinceLastFetch = loopsUntilNextFetch;

	int UpdateLEDs(CRGB *leds_out, uint16 startLed, uint16 ledcount) {
		PrintUpdateLEDs();
		loopsSinceLastFetch++;

		bool shouldFetch = FetchUtil::ShouldFetch(debugIsOn, cachedTemperatures.size(), loopsSinceLastFetch, loopsUntilNextFetch);
		if(shouldFetch){
			return FetchServer(SERVER_NAME_WEATHER);
		}

		TemperatureForecast(leds_out, cachedTemperatures, startLed, startLed + ledcount);
		return loopDelayWithoutFetch;
	}

	void PrintUpdateLEDs() {
	if(!debugIsOn){
		return;
	}
		Serial.println("--- TemperatureMode: UpdateLEDs ---");
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
		}
		String payload = http.getString();
		std::string clampedPayload = convertPayload(payload);
		std::vector<int8_t> temperatures = strToTemperatures(clampedPayload);
		cachedTemperatures = temperatures;
		
		loopsSinceLastFetch = 0;
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
		//Serial.print("Payload: ");
		//Serial.println(payload);
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

	std::vector<int8_t> strToTemperatures(std::string str) {
		std::string temp;
		std::vector<int8_t> temperatures = {};
		int8_t index = 0;
		for (char const &c: str) {
			if(c == ','){
				int8_t temperature =  std::stoi(temp);
				PrintStrTemperatures(temperature, index);
				temperatures.push_back(temperature);
				index++;
				temp = "";
				if(index >= 24){
					return temperatures;
				}
			}
			else {
				temp += c;
			}
		}

		return temperatures;
	}

	void PrintStrTemperatures(int8 temperature, int8 index) {
		if(!debugIsOn){
			return;
		}

		Serial.print("StrToTemperatures - temperature: ");
		Serial.print(temperature);
		Serial.print(", index: ");
		Serial.println(index);
	}

	
	// Display Temperature forcast for current Day
	void TemperatureForecast(CRGB *leds_out, std::vector<int8_t> temperaturesForNext24Hours, uint16 startLED, uint16 endLED) {
		// Config
		uint16_t ledCount = endLED - startLED;

		// Ratio
		uint16_t temperaturesCount = temperaturesForNext24Hours.size();
		float temperatureCountToLEDRatio = (float)temperaturesCount / (float)ledCount;

		int8_t minClampTempInCelsius = -10;
		int8_t maxClampTempInCelsius = 36;
		float offsetToZero = 0 - minClampTempInCelsius;

		if(offsetToZero > 0){
			minClampTempInCelsius += offsetToZero;
			maxClampTempInCelsius += offsetToZero;
		}

		PrintTemperatureForecast1(temperaturesCount, startLED, endLED, ledCount, temperatureCountToLEDRatio);
		
		int8_t temp6 = (temperaturesForNext24Hours[0] + temperaturesForNext24Hours[1] + temperaturesForNext24Hours[2] + temperaturesForNext24Hours[3] + temperaturesForNext24Hours[4] + temperaturesForNext24Hours[5]) / 6;
		int8_t temp12 = (temperaturesForNext24Hours[6] + temperaturesForNext24Hours[7] + temperaturesForNext24Hours[8] + temperaturesForNext24Hours[9] + temperaturesForNext24Hours[10] + temperaturesForNext24Hours[11]) / 6;
		int8_t temp18 = (temperaturesForNext24Hours[12] + temperaturesForNext24Hours[13] + temperaturesForNext24Hours[14] + temperaturesForNext24Hours[15] + temperaturesForNext24Hours[16] + temperaturesForNext24Hours[17]) / 6;
		int8_t temp24 = (temperaturesForNext24Hours[18] + temperaturesForNext24Hours[19] + temperaturesForNext24Hours[20] + temperaturesForNext24Hours[21] + temperaturesForNext24Hours[22] + temperaturesForNext24Hours[23]) / 6;

		// Update Colors		
		double quarterCount = (double)ledCount / (double)4;
		uint16_t ledCountQuarter1Max = floor(1 * quarterCount);
		uint16_t ledCountQuarter2Max = floor(2 * quarterCount);
		uint16_t ledCountQuarter3Max = floor(3 * quarterCount);
		uint16_t ledCountQuarter4Max = floor(4 * quarterCount);

		PrintTemperatureForecast2(temp6, temp12, temp18, temp24, ledCountQuarter1Max,ledCountQuarter2Max,ledCountQuarter3Max,ledCountQuarter4Max);

		for ( uint16_t i = startLED; i < endLED; i++ ) {
			// Get LED Index
			float temperatureIndex1 = (float)i* (float)temperatureCountToLEDRatio;
			temperatureIndex1 = floor(temperatureIndex1);
			// Get Temperature
			float temperature1 = temperaturesForNext24Hours[temperatureIndex1];
			if(offsetToZero > 0){
				temperature1 += offsetToZero;
			}

			uint8_t averageTemperature;
			if(i <= startLED + ledCountQuarter1Max){
				averageTemperature = temp6;
			}else if(i <= startLED + ledCountQuarter2Max){
				averageTemperature = temp12;
			}else if(i <= startLED + ledCountQuarter3Max){
				averageTemperature = temp18;
			}else {
				averageTemperature = temp24;
			}

			{
				//Serial.print(" -> temperature: ");
				//Serial.println(averageTemperature);
				// Serial.println(", temperatureIndex1: ");
				// Serial.print(temperatureIndex1);
				// Serial.print(", baseIndex: ");
				// Serial.print(baseIndex);
				// Serial.print(", i: ");
				// Serial.print(i);
				// Serial.print(", avgTemp: ");
				// Serial.println(averageTemperature);
			}

			// Calculate Color
			CRGB color = TemperatureToColor(averageTemperature, minClampTempInCelsius, maxClampTempInCelsius);
			{
				// Serial.print(", r: ");
				// Serial.print(color.r);
				// Serial.print(", g: ");
				// Serial.print(color.g);
				// Serial.print(", b: ");
				// Serial.print(color.b);
				// Serial.println();

			}

			// Update LED
			ColorUtil::SafeSetColor(leds_out, i, color);
		}

		return;
	}

	void PrintTemperatureForecast1(uint16_t temperaturesCount, uint16_t startLED, uint16_t endLED, uint16_t ledCount, double temperatureCountToLEDRatio) {
		if(!debugIsOn){
			return;
		}

		Serial.println("TemperatureMode - TemperatureForecast");

		Serial.println("temperaturesCount: ");
		Serial.print(temperaturesCount);
		Serial.print(", startLED: ");
		Serial.print(startLED);
		Serial.print(", endLED: ");
		Serial.print(endLED);
		Serial.print(", ledCount: ");
		Serial.print(ledCount);
		Serial.print(", temperatureCountToLEDRatio: ");
		Serial.println(temperatureCountToLEDRatio);

		// temperaturesForNext24Hours = {0,0,0,0,0,0,20,20,20,20,20,20,-10,-10,-10,-10,-10,-10,30,30,30,30,30,30};
		//Serial.println("temperatures for next 24 hours: ");
		//for (uint8 i = 0; i < temperaturesCount; i++)
		//{
		//	Serial.print(", i: ");
		//	Serial.print(i);
		//	Serial.print(" -> temperature: ");
		//	Serial.print(temperaturesForNext24Hours[i]);
		//}
		//Serial.println("------------------------------");
	}

	void PrintTemperatureForecast2(uint16_t temp6, uint16_t temp12, uint16_t temp18, uint16_t temp24, 
		uint16_t ledCountQuarter1Max,
		uint16_t ledCountQuarter2Max,
		uint16_t ledCountQuarter3Max,
		uint16_t ledCountQuarter4Max
	) {
		if(!debugIsOn){
			return;
		}

		Serial.println("temp6: ");
		Serial.print(temp6);
		Serial.print(", temp12: ");
		Serial.print(temp12);
		Serial.print(", temp18: ");
		Serial.print(temp18);
		Serial.print(", temp24: ");
		Serial.println(temp24);

		Serial.println("quartermax: ");
		Serial.println(ledCountQuarter1Max);
		Serial.println(ledCountQuarter2Max);
		Serial.println(ledCountQuarter3Max);
		Serial.println(ledCountQuarter4Max);
	}

	CRGB TemperatureToColor(float temperature, float minClampTempInCelsius, float maxClampTempInCelsius) {
		float range = maxClampTempInCelsius - minClampTempInCelsius;
		
		if(temperature >= range){
			temperature = range - 0.001;
		}

		double ratio = temperature <= 0 ? 0 : ((float)temperature) / range;
		
		PrintTemperatureToColor(temperature, minClampTempInCelsius, maxClampTempInCelsius);
		CRGB color = ratioToGradientColor(ratio);
		return color;
	}

	void PrintTemperatureToColor(float temperature, float minClampTempInCelsius, float maxClampTempInCelsius) {
		if(!debugIsOn){
			return;
		}

		// Serial.println("TemperatureMode - TemperatureToColor - temperature: ");
		// Serial.print(temperature);
		// Serial.print(", minClampTempInCelsius: ");
		// Serial.print(minClampTempInCelsius);
		// Serial.print(", maxClampTempInCelsius: ");
		// Serial.print(maxClampTempInCelsius);
		// Serial.print(", range: ");
		// Serial.print(range);
		// Serial.print(", ratio: ");
		// Serial.println(ratio);
	}

	//input: ratio is between 0.0 to 1.0
	//output: rgb color
	CRGB ratioToGradientColor(double ratio)
	{
		if(ratio == 0){
			ratio = 0.001;
		}
		//we want to normalize ratio so that it fits in to 6 regions
		//where each region is 256 units long
		int normalized = int(ratio * 256 * 6);

		//find the region for this position
		int region = normalized / 256;

		//find the distance to the start of the closest region
		int x = normalized % 256;

		uint16_t r = 0, g = 0, b = 0;
		switch (region)
		{
		case 0: r = gradientColor1.r; g = gradientColor1.g; b = gradientColor1.b; g += x; break;
		case 1: r = gradientColor2.r; g = gradientColor2.g; b = gradientColor2.b; b -= x; break;
		case 2: r = gradientColor3.r; g = gradientColor3.g; b = gradientColor3.b; r += x; break;
		case 3: r = gradientColor4.r; g = gradientColor4.g; b = gradientColor4.b; g -= x; break;
		case 4: r = gradientColor5.r; g = gradientColor5.g; b = gradientColor5.b; b += x; break;
		case 5: r = gradientColor6.r; g = gradientColor6.g; b = gradientColor6.b; break; //r -= x; break;
		}

		// Red is too weak when combined when other colors (whyever?!?)
		if(r > 0){
			if(b > 0 && g == 0){
			b = b/4;
			}
			else if(g > 0 && b ==0){
				g= g/2;
			}
		}
		return CRGB(r, g, b);//r + (g << 8) + (b << 16);
	}

};

#endif