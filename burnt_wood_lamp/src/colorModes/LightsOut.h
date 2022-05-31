#pragma once

#include "ColorMode.h"
#include "SingleColor.h"

class LightsOut : public SingleColor {
public:
	LightsOut() : SingleColor( CRGB::Black ) {}

	constexpr static const char *getName() {
		return "LightsOut";
	}
};