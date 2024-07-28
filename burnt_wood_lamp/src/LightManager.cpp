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
	const String &payload
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
	return "CityAtSundown, GlobalColorTick, LightsOut, WakeUp, Pacifica, ColorWheel, ColorPulse, DiscoStrobo, SingleColor(Color), LaurasPartymodus(Beta), KjeldPartyModus";
}

ColorMode *LightManager::decodeColorModeString( const String &modeName, uint32_t color1, uint32_t color2, const String &payload ) {
	if ( modeName == CityAtSundown::getName() ) {
		return new CityAtSundown{};
	}
	if ( modeName == GlobalColorTick::getName() ) {
		return new GlobalColorTick{};
	}
	if ( modeName == LightsOut::getName() ) {
		return new LightsOut{};
	}
	if ( modeName == WakeUp::getName() ) {
		return new WakeUp{};
	}
	if ( modeName == Pacifica::getName() ) {
		return new Pacifica{};
	}
	if ( modeName == ColorWheel::getName() ) {
		return new ColorWheel{};
	}
	if ( modeName == ColorPulse::getName() ) {
		return new ColorPulse{};
	}
	if ( modeName == SingleColor::getName() ) {
		return new SingleColor{ color1 };
	}
	if ( modeName == ColorFromPayload::getName() ) {
		return new ColorFromPayload{ payload };
	}
	if ( modeName == DiscoStrobo::getName() ) {
		return new DiscoStrobo{};
	}
	if ( modeName == LauraPartyMode::getName() ) {
		return new LauraPartyMode{};
	}
	if ( modeName == KjeldPartyMode::getName() ) {
		return new KjeldPartyMode{};
	}

	if ( modeName == DualColor::getName() ) {
		return new DualColor{ color1, color2 };
	}

	return nullptr;
}

LightManager::~LightManager() {
	delete currentColorMode;
}
