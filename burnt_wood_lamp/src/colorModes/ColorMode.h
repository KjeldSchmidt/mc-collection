//
// Created by kjeld on 25/11/2020.
//
#ifndef BURNT_WOOD_LAMP_COLORMODE_H
#define BURNT_WOOD_LAMP_COLORMODE_H

#include "FastLED.h"
#include "config/ConfigVariables.h"

class ColorMode {
public:
	virtual uint16 Update( CRGB *leds_out, uint16 startLED = -1, uint16 ledCount = -1 ) = 0;

	virtual ~ColorMode() = default;

	constexpr static const char *getName() {
		return "ColorMode";
	}
};

#include "ArduinoBorealis.h"

#endif //BURNT_WOOD_LAMP_COLORMODE_H
