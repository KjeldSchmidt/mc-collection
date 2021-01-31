//
// Created by kjeld on 31/01/2021.
//

#include "LightManager.h"

const char *LightManager::getModes() {
	return "CityAtSundown, GlobalColorTick";
}

LightManager::LightManager() {
	currentColorMode = new CityAtSundown{};
}

void LightManager::setMode( const String &newModeName ) {

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
