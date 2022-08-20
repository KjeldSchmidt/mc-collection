//
// Created by kjeld on 31/01/2021.
//
#pragma once
#ifndef BURNT_WOOD_LAMP_LIGHTMANAGER_H
#define BURNT_WOOD_LAMP_LIGHTMANAGER_H


#include "colorModes/ColorMode.h"
#include "modes/Mode.h"
#include "../config/lamps/ClockLamp.h"

class LightManager {

public:
	explicit LightManager( CRGB *leds_out );

	static const char *getModes();

	bool setMode( Mode &newModeName, uint32_t color1 = 0, uint32_t color2 = 0 );

	void updateLEDs();

	virtual ~LightManager();


private:
	// ClockLamp *clockLamp;
	ColorMode *currentColorMode;

	uint64_t delayTime = 0;
	uint64_t lastUpdateTime = 0;
	CRGB *leds_out;
	ClockLamp lamp;
};


#endif //BURNT_WOOD_LAMP_LIGHTMANAGER_H
