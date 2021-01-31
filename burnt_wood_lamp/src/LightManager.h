//
// Created by kjeld on 31/01/2021.
//

#ifndef BURNT_WOOD_LAMP_LIGHTMANAGER_H
#define BURNT_WOOD_LAMP_LIGHTMANAGER_H


#include "ColorMode.h"

class LightManager {

public:
	LightManager();

	const char *getModes();

	void setMode( const String &newModeName );

	void updateLEDs( CRGB *leds_out );

private:
	ColorMode *currentColorMode;
};


#endif //BURNT_WOOD_LAMP_LIGHTMANAGER_H
