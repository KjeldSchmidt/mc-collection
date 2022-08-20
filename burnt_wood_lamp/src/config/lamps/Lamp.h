#ifndef BURNT_WOOD_LAMP_LAMP_H
#define BURNT_WOOD_LAMP_LAMP_H

#include "FastLED.h"
#include "../stripes/LedStripe.h"
#include "../../colorModes/ColorMode.h"

class Lamp {
public:
    int Update(CRGB *leds_out);
    ~Lamp() = default;
    std::vector<LedStripe*> LedStripes();
    std::vector<ColorMode*> ColorModes(); 

};


#endif