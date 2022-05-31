#pragma once

#include "ColorMode.h"
#include "DualColor.h"

class CityAtSundown : public DualColor {
public:
	CityAtSundown() : DualColor(
			CRGB( 0x610200 ),
			CRGB( 0x611e01 )
	) {}

	constexpr static const char *getName() {
		return "CityAtSundown";
	}
};