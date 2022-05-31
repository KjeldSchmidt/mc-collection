#pragma once

#include "ColorMode.h"
#include "SingleColor.h"

class WakeUp : public SingleColor {
public:
	WakeUp() : SingleColor(
			CRGB( MAX_BRIGHTNESS_PER_CHANNEL, MAX_BRIGHTNESS_PER_CHANNEL, MAX_BRIGHTNESS_PER_CHANNEL )
	) {}

	constexpr static const char *getName() {
		return "WakeUp";
	}
};