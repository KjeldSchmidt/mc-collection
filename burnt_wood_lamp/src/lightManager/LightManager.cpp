//
// Created by kjeld on 31/01/2021.
//

#include "lightManager/LightManager.h"

LightManager::LightManager( CRGB *leds_out ) : leds_out( leds_out ) {
	currentColorMode = new CityAtSundown{};
}

bool LightManager::setMode( Mode &newModeName, uint32_t color1, uint32_t color2 ) {
	ColorMode *newMode = decodeColorModeString( newModeName, color1, color2 );

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

 /*ColorMode *LightManager::decodeColorModeString( const String &modeName, uint32_t color1, uint32_t color2 ) {*/
ColorMode *LightManager::decodeColorModeString( Mode &modeName, uint32_t color1, uint32_t color2 ) {
	switch (modeName)
	{
	case Mode::CityAtSundownMode:
		return new CityAtSundown{};
	case Mode::GlobalColorTickMode:
		return new GlobalColorTick{};
	case Mode::LightsOutMode:
		return new LightsOut{};
	case Mode::WakeUpMode:
		return new WakeUp{};
	case Mode::PacificaMode:
		return new Pacifica{};
	case Mode::ColorPulseMode:
		return new ColorPulse{};
	case Mode::DiscoStroboMode:
		return new DiscoStrobo{};
	case Mode::SingleColorColorMode:
		return new SingleColor{ color1 };
	case Mode::LaurasPartymodusBetaMode:
		return new LauraPartyMode{};
	case Mode::KjeldPartyModusMode:
		return new KjeldPartyMode{};
	case Mode::TimExistentialDreadModusMode:
		return new TimExistentialDreadMode{};

	default:
		return new CityAtSundown{};
	}
}

LightManager::~LightManager() {
	delete currentColorMode;
}
