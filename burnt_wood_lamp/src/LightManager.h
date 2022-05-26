//
// Created by kjeld on 31/01/2021.
//

#ifndef BURNT_WOOD_LAMP_LIGHTMANAGER_H
#define BURNT_WOOD_LAMP_LIGHTMANAGER_H


#include "ColorMode.h"
#include "modes/Mode.h"

class LightManager {

public:
	explicit LightManager( CRGB *leds_out );

	static const char *getModes();


	bool setMode( Mode &newModeName, uint32_t color1 = 0, uint32_t color2 = 0 );

	void updateLEDs();

	virtual ~LightManager();


private:
	static ColorMode *decodeColorModeString( Mode &modeName, uint32_t color1 = 0, uint32_t color2 = 0 );

	ColorMode *currentColorMode;
	

	uint64_t delayTime = 0;
	uint64_t lastUpdateTime = 0;
	CRGB *leds_out;
};


#endif //BURNT_WOOD_LAMP_LIGHTMANAGER_H
