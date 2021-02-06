//
// Created by kjeld on 31/01/2021.
//

#include "LightManager.h"

const char *LightManager::getModes() {
	return "CityAtSundown, GlobalColorTick, LightsOut, WakeUp";
}

LightManager::LightManager( CRGB *leds_out ) : leds_out( leds_out ) {
	currentColorMode = new CityAtSundown{};
}

bool LightManager::setMode( const String &newModeName ) {
	ColorMode *newMode = decodeColorModeString( newModeName );

	if ( newMode == nullptr ) return false;

	delete currentColorMode;
	currentColorMode = newMode;
	delayTime = 0;
	return true;
}

void LightManager::updateLEDs() {
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

	if ( modeName == "WakeUp" ) {
		return new WakeUp{};
	}
	return nullptr;
}

LightManager::~LightManager() {
	delete currentColorMode;
}


