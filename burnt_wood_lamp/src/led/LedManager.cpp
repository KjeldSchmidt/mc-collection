//
// Created by kjeld on 31/01/2021.
//

#include "LedManager.h"
#include "modes/Mode.h"
#include "modes/Mode.h"
#include "factories/ColorModeFactory.h"

LightManager::LightManager( CRGB *leds_out ) : leds_out( leds_out ) {

	Mode initialMode = Mode::PacificaMode;
	ColorMode* initialColorMode = ColorModeFactory::createColorMode(initialMode);
	currentColorMode = initialColorMode;
}

bool LightManager::setMode( Mode &newModeName, uint32_t color1, uint32_t color2 ) {
	ColorMode *newMode = ColorModeFactory::createColorMode( newModeName, color1, color2 );

	// No Mode
	bool noMode = newMode == nullptr;
	if (noMode) {
		return false;
	}

	// Set New Mode
	delete currentColorMode;
	currentColorMode = newMode;
	delayTime = 0;

	return true;
}

void LightManager::updateLEDs() {

	// Time for LED Update
	uint64_t currentMillis = millis();
	uint64_t millisPassed = currentMillis - lastUpdateTime;
	bool triggerUpdate = millisPassed >= delayTime;

	// Check LED Update
	if (triggerUpdate) {
		delayTime = currentColorMode->Update( leds_out );
		lastUpdateTime = currentMillis;
		FastLED.show();
	}
}

const char *LightManager::getModes() {
	return "CityAtSundown, GlobalColorTick, LightsOut, WakeUp, Pacifica, ColorPulse, DiscoStrobo, SingleColor(Color), LaurasPartymodus(Beta), KjeldPartyModus, TimExistentialDreadMode";
}

LightManager::~LightManager() {
	delete currentColorMode;
}
