//
// Created by kjeld on 31/01/2021.
//

#ifndef BURNT_WOOD_LAMP_LIGHTMANAGER_H
#define BURNT_WOOD_LAMP_LIGHTMANAGER_H


#include "ColorMode.h"

class LightManager {

public:
	explicit LightManager( CRGB *leds_out );

	static const char *getModes();

	bool setMode( const String &newModeName );

	bool setMode( const String &newModeName, uint32_t color );

	void updateLEDs();

	virtual ~LightManager();

private:
	static ColorMode *decodeColorModeString( const String &modeName );

	static ColorMode *decodeColorModeString( const String &modeName, uint32_t color );

	ColorMode *currentColorMode;

	uint64_t delayTime = 0;
	uint64_t lastUpdateTime = 0;
	CRGB *leds_out;
};


#endif //BURNT_WOOD_LAMP_LIGHTMANAGER_H
