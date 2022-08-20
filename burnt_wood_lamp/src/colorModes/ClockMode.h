#ifndef BURNT_WOOD_LAMP_CLOCKMODE_H
#define BURNT_WOOD_LAMP_CLOCKMODE_H

#include "ColorMode.h"
#include "NTPClient.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

#include <string>
#include <iostream>
#include "../util/color/ColorUtil.h"
#include "../util/fetch/FetchUtil.h"

class ClockMode : public ColorMode {
public:
	uint16 Update( CRGB *leds_out, uint16 startLed, uint16 ledCount) override {
		int num = UpdateLEDs(leds_out, startLed, ledCount);
		return  (uint16) num;
	}
	
	constexpr static const char *getName() {
		return "ClockMode";
	}

	ClockMode() {
 		configTime(TIMEZONE, NTP_SERVER); 
	}

private:
	const CRGB hourColor = CRGB(255,0,0);
	const CRGB minuteColor = CRGB(0,255,0);
	const CRGB bgColor = CRGB(0,0,255);
	const uint16 ledRadius = 5;
	const uint8 ledRadiusSquareXTimes = 2;
	const CRGB debugColor = CRGB(0,0,0);
	const bool debugIsOn = false;


	int UpdateLEDs(CRGB *leds_out, uint16 startLed, uint16 ledCount) {
		PrintUpdateLEDs();
		Clock(leds_out, startLed, startLed + ledCount); 

		return 1000;
	}

	void PrintUpdateLEDs() {
	if(!debugIsOn){
		return;
	}
		Serial.println("--- ClockMode: UpdateLEDs ---");

	}

	// Display Time on Big Ring using hourColor, minuteColor & bgColor
	void Clock(CRGB *leds_out, uint16 startLed, uint16 endLed) {
		uint16 ledCount = endLed - startLed;
		PrintClockParams(leds_out, startLed, endLed);

		// Time
		time_t now; // this is the epoch
		tm tm; // access time in formats
		time(&now); // read the current time
		localtime_r(&now, &tm); // update the structure tm with the current time
		uint16 hour = tm.tm_hour;
		if(hour >= 12){
			hour -= 12;
		}
		uint16 minute = tm.tm_min;

		// Indices
		uint16 hourLedIndex = startLed + GetLedIndex(hour, HOURS, ledCount);
		uint16 minuteLedIndex = startLed + GetMinuteLedIndex(minute, MINUTES, ledCount, hourLedIndex);
		std::vector<uint16> adjacentHourLedIndices = GetAdjacentLedIndices(hourLedIndex, ledCount, ledRadius, startLed, endLed);
		std::vector<uint16> adjacentMinuteLedIndices = GetAdjacentLedIndices(minuteLedIndex, ledCount, ledRadius, startLed, endLed);

		PrintClockTime(hour, minute, hourLedIndex, minuteLedIndex, adjacentHourLedIndices, adjacentMinuteLedIndices);

		// Bg Color // todo use set single color
   		for ( uint16_t i = startLed; i < endLed; i++ ) {
                leds_out[i] = CRGB(0,0,126);
        }
		
		SetRadiusColors("Set Hour Radius Colors: ", leds_out, ledRadius, adjacentHourLedIndices, hourColor);
		SetRadiusColors("Set Minute Radius Colors: ", leds_out, ledRadius, adjacentMinuteLedIndices, minuteColor);
	}

	void PrintClockParams(CRGB *leds_out, uint16 startLed, uint16 endLed, bool isOn = true) {
		if(!debugIsOn){
			return;
		}

		Serial.println("----------------Update - Big Ring - Clock----------------------");
		Serial.print("Start: ");
		Serial.print(startLed);
		Serial.print(", End: ");
		Serial.print(endLed);
		Serial.println();
		Serial.print(", ledsCount: ");
		Serial.print(sizeof(leds_out));
		Serial.println();
	}

	void PrintClockTime( uint16 hour, uint16 minute, uint16 hourLedIndex, uint16 minuteLedIndex, std::vector<uint16> adjacentHourLedIndices, std::vector<uint16> adjacentMinuteLedIndices ) {
		if(!debugIsOn){
			return;
		}
		
		Serial.print("hour: ");
		Serial.print(hour);
		Serial.print(", minute: ");
		Serial.print(minute);
		Serial.print(", hourLedIndex: ");
		Serial.print(hourLedIndex);
		Serial.print(", minuteLedIndex: ");
		Serial.print(minuteLedIndex);
		Serial.print(", adjacentHourLedIndices: ");
		for (size_t i = 0; i < adjacentHourLedIndices.size(); i++){
			Serial.print(adjacentHourLedIndices[i]);
			Serial.print(", ");
		}
		Serial.println();
		Serial.print(", adjacentMinuteLedIndices: ");
		for (size_t i = 0; i < adjacentMinuteLedIndices.size(); i++){
			Serial.print(adjacentMinuteLedIndices[i]);
			Serial.print(", ");
		}
		Serial.println();
	}

	// todo only works with more leds than color not other way round
	uint16 GetLedIndex(uint16 hour, uint16 max, uint16 ledCount) {
		float ledCountToValueRatio = (float)ledCount /  (float)max;// 24/26 -> ~0.92
		float float_LED =  (float)hour *  (float)ledCountToValueRatio;
		uint16 LEDIndex = round(float_LED);

		return LEDIndex;
	}

	uint16 GetMinuteLedIndex(uint16 minute,  uint16 max, uint16 ledCount, uint16 hourLedIndex) {
		float ledCountToMinuteRatio = (float)ledCount / (float)max;
		float float_LED =  (float)minute * ledCountToMinuteRatio;
		uint16 minuteLedIndex = round(float_LED);

		if(minuteLedIndex == hourLedIndex){
			// MinutePointer should differ from hourPointer so round it another way
			bool hourLedIndexIsBigger = hourLedIndex > float_LED;
			minuteLedIndex = hourLedIndexIsBigger ? hourLedIndex - 1 : hourLedIndex  + 1;
		}

		// Clamp above 0 and avoid hour pointer
		if(minuteLedIndex < 0){
			minuteLedIndex = 1;
		}

		// don't exceed boundary, better wrap
		return minuteLedIndex % ledCount;
	}

	void SetRadiusColors(String preText, CRGB *leds_out, uint16 ledRadius, std::vector<uint16> indices, CRGB color) {
		double factorPerLed = (double)1 / (double)ledRadius;
		// Increasing Color
		uint16 increment = 0;
		
		if(debugIsOn) {
			Serial.println(preText);
		}
		for (uint16 i = 0; i < ledRadius; i++)
		{
			
			uint16 index = indices[i];
			CRGB finalColor = ColorUtil::IncreaseColorPerLed(color, index, factorPerLed, ledRadiusSquareXTimes);

			ColorUtil::SafeSetColor(leds_out, index, finalColor);

			increment++;
		}

		// MidPoint
		uint16 midPointIndex = indices[ledRadius];
		ColorUtil::SafeSetColor(leds_out, midPointIndex, color);

		// Decreasing Color
		increment = 0;
		uint16 startIndexBack = ledRadius + 1;
		for (uint16 i = startIndexBack; i < startIndexBack + ledRadius; i++)
		{
			uint16 index = indices[i];
			CRGB finalColor = ColorUtil::DecreaseColorPerLed(color, index, factorPerLed, ledRadiusSquareXTimes);
			ColorUtil::SafeSetColor(leds_out, index, finalColor);
			increment++;
		}
	}

	void PrintColor(String preText, uint16 index, double factorPerLed, CRGB color){
		if(!debugIsOn){
			return;
		}
		Serial.println(preText);
		Serial.print("index: ");
		Serial.print(index);
		Serial.print(", factorPerLed");
		Serial.print(factorPerLed);
		Serial.print(" with color: r: ");
		Serial.print(color.r);
		Serial.print(", g: ");
		Serial.print(color.g);
		Serial.print(", b: ");
		Serial.print(color.b);
		Serial.println();
	}
	
	std::vector<uint16> GetAdjacentLedIndices(uint16 LEDIndex, uint16 ledCount, uint16 ledRadius, uint16 min, uint16 max) {
  		std::vector<uint16> ledIndices(0);

		// 1st half
		uint16_t LEDStart1 = LEDIndex - ledRadius;
		uint16 LEDEnd1 = LEDIndex;
		for (int16_t i = LEDStart1; i < LEDEnd1; i++)
		{
			uint16 index = ColorUtil::ModuloIndexOnLedStripe(i, min, max);
			ledIndices.push_back(i);
			PrintAdjacentLedIndices(i, index);
		}

		// MidPoint
		ledIndices.push_back(LEDIndex);
		// Serial.println(LEDIndex);

		// 2nd half
		uint16 LEDStart2 = LEDIndex + 1;
		uint16 LEDEnd2 = LEDIndex + 1 + ledRadius;
		for (uint16 i = LEDStart2; i < LEDEnd2; i++)
		{
			uint16 index = ColorUtil::ModuloIndexOnLedStripe(i, min, max);
			ledIndices.push_back(index);
			PrintAdjacentLedIndices(i, index);
		}

		return ledIndices;
	}

	void PrintAdjacentLedIndices(uint16 i, uint16 index){
		if(!debugIsOn){
			return;
		}

		Serial.print("i: ");
		Serial.print(i);
		Serial.print(", index: ");
		Serial.print(index);
		Serial.println();
	}

};

#endif