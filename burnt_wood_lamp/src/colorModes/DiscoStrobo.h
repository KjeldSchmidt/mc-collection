#pragma once

#include "ColorMode.h"
#include "SingleColorBlink.h"

class DiscoStrobo : public SingleColorBlink {
public:
	DiscoStrobo() : SingleColorBlink( 
		CRGB(MAX_BRIGHTNESS_PER_CHANNEL, MAX_BRIGHTNESS_PER_CHANNEL, MAX_BRIGHTNESS_PER_CHANNEL),
		50
	) {}
};