//
// Created by kjeld on 31/01/2021.
//

#include "LightManager.h"

LightManager::LightManager( CRGB *leds_out ) : leds_out( leds_out ) {
	currentColorMode = new CityAtSundown{};
}

bool LightManager::setMode(
	const String &newModeName,
	uint32_t color1,
	uint32_t color2,
	const uint8_t *payload
) {
	ColorMode *newMode = decodeColorModeString( newModeName, color1, color2, payload );

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

const char *LightManager::getModes() {
	return "CityAtSundown, GlobalColorTick, LightsOut, WakeUp, Pacifica, ColorWheel, ColorPulse, DiscoStrobo, LauraPartyMode, KjeldPartyMode";
}

const char *LightManager::getCurrentMode() const {
	return currentColorMode->getName();
}

ColorMode *LightManager::decodeColorModeString( const String &modeName, uint32_t color1, uint32_t color2, const uint8_t *payload ) {
	if ( modeName == "CityAtSundown" ) return new CityAtSundown{};
	if ( modeName == "GlobalColorTick" ) return new GlobalColorTick{};
	if ( modeName == "LightsOut" ) return new LightsOut{};
	if ( modeName == "WakeUp" ) return new WakeUp{};
	if ( modeName == "Pacifica" ) return new Pacifica{};
	if ( modeName == "ColorWheel" ) return new ColorWheel{};
	if ( modeName == "ColorPulse" ) return new ColorPulse{};
	if ( modeName == "DiscoStrobo" ) return new DiscoStrobo{};
	if ( modeName == "LauraPartyMode" ) return new LauraPartyMode{};
	if ( modeName == "KjeldPartyMode" ) return new KjeldPartyMode{};
	if ( modeName == "SingleColor" ) return new SingleColor{ color1 };
	if ( modeName == "ColorFromPayload" ) return new ColorFromPayload{ payload };
	if ( modeName == "DualColor" ) return new DualColor{ color1, color2 };

	return nullptr;
}

LightManager::~LightManager() {
	delete currentColorMode;
}
