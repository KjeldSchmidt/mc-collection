//
// Created by kjeld on 31/01/2021.
//

#include "LightManager.h"

const char *LightManager::getModes() {
	return "CityAtSundown, GlobalColorTick, LightsOut";
}

LightManager::LightManager() {
	currentColorMode = new CityAtSundown{};
}

bool LightManager::setMode( const String &newModeName ) {
	ColorMode *newMode = decodeColorModeString( newModeName );

	if ( newMode == nullptr ) return false;

	delete currentColorMode;
	currentColorMode = newMode;
	return true;
}

void LightManager::updateLEDs( CRGB *leds_out ) {
	static uint64_t lastUpdateTime = 0;
	static uint64_t delayTime = 0;
	uint64_t currentMillis = millis();
	if ( currentMillis - lastUpdateTime >= delayTime ) {
		delayTime = currentColorMode->Update( leds_out );
		lastUpdateTime = currentMillis;
		FastLED.show();
	}
}

ColorMode *LightManager::decodeColorModeString( const String &modeName ) {
	if ( modeName == "CityAtSundown" ) {
		return new CityAtSundown{};
	}
	if ( modeName == "GlobalColorTick" ) {
		return new GlobalColorTick{};
	}
	if ( modeName == "LightsOut" ) {
		return new LightsOut{};
	}
	return nullptr;
}

LightManager::~LightManager() {
	delete currentColorMode;
}


