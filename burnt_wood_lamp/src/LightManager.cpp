//
// Created by kjeld on 31/01/2021.
//

#include "LightManager.h"

LightManager::LightManager( CRGB *leds_out ) : leds_out( leds_out ) {
	currentColorMode = new LightsOut{};
}

bool LightManager::setMode( const String &newModeName, uint32_t color1, uint32_t color2 ) {
	ColorMode *newMode = decodeColorModeString( newModeName, color1, color2 );

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
	return "CityAtSundown, GlobalColorTick, LightsOut, WakeUp, Pacifica, ColorPulse, DiscoStrobo, SingleColor(Color), LaurasPartymodus(Beta), KjeldPartyModus, TimExistentialDreadMode";
}

ColorMode *LightManager::decodeColorModeString( const String &modeName, uint32_t color1, uint32_t color2 ) {
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
	if ( modeName == "Pacifica" ) {
		return new Pacifica{};
	}
	if ( modeName == "ColorPulse" ) {
		return new ColorPulse{};
	}
	if ( modeName == "SingleColor" ) {
		return new SingleColor{ color1 };
	}
	if ( modeName == "DiscoStrobo" ) {
		return new DiscoStrobo{};
	}
	if ( modeName == "LaurasPartymodus(Beta)" ) {
		return new LauraPartyMode{};
	}
	if ( modeName == "KjeldPartyModus" ) {
		return new KjeldPartyMode{};
	}
    if ( modeName == "TimExistentialDreadMode" ) {
        return new TimExistentialDreadMode{};
    }

	if ( modeName == "DualColor" ) {
		return new DualColor{ color1, color2 };
	}

	return nullptr;
}

LightManager::~LightManager() {
	delete currentColorMode;
}
