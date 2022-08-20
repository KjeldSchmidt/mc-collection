#pragma once

#include "modes/Mode.h"
#include "colorModes/ColorMode.h"

#include "colorModes/CityAtSundown.h"
#include "colorModes/GlobalColorTick.h"
#include "colorModes/LightsOut.h"
#include "colorModes/WakeUp.h"
#include "colorModes/Pacifica.h"
#include "colorModes/ColorPulse.h"
#include "colorModes/DiscoStrobo.h"
#include "colorModes/LauraPartyMode.h"
#include "colorModes/KjeldPartyMode.h"
#include "colorModes/TimExistentialDreadMode.h"
#include "colorModes/ClockMode.h"

class ColorModeFactory {
public:
	static ColorMode *createColorMode( Mode modeName, uint32_t color1 = 0, uint32_t color2 = 0 ) {
        switch (modeName)
        {
            case Mode::CityAtSundownMode:
                return new CityAtSundown{};
            case Mode::GlobalColorTickMode:
                return new GlobalColorTick{};
            case Mode::LightsOutMode:
                return new LightsOut{};
            case Mode::WakeUpMode:
                return new WakeUp{};
            case Mode::PacificaMode:
                return new Pacifica{};
            case Mode::ColorPulseMode:
                return new ColorPulse{};
            case Mode::DiscoStroboMode:
                return new DiscoStrobo{};
            case Mode::SingleColorColorMode:
                return new SingleColor{ color1 };
            case Mode::LaurasPartymodusBetaMode:
                return new LauraPartyMode{};
            case Mode::KjeldPartyModusMode:
                return new KjeldPartyMode{};
            case Mode::TimExistentialDreadModusMode:
                return new TimExistentialDreadMode{};
            case Mode::ClockModusMode:
                return new ClockMode{};
            default:
                return new CityAtSundown{};
            }
        }

};
