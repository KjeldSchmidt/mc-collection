#pragma once

#include<string>
#include "Mode.h"

std::string ModeToString(Mode value)
{
    if (value == Mode::CityAtSundownMode) return "CityAtSundown";
    if (value == Mode::GlobalColorTickMode)  return "GlobalColorTick";
    if (value == Mode::LightsOutMode)  return "LightsOut";
    if (value == Mode::WakeUpMode)  return "WakeUp";
    if (value == Mode::ColorPulseMode)  return "ColorPulse";
    if (value == Mode::DiscoStroboMode)  return "DiscoStrobo";
    if (value == Mode::SingleColorColorMode)  return "SingleColor(Color)";
    if (value == Mode::LaurasPartymodusBetaMode)  return "LaurasPartymodus(Beta)";
    if (value == Mode::KjeldPartyModusMode)  return "KjeldPartyModus";
    if (value == Mode::TimExistentialDreadModusMode)  return "TimExistentialDreadMode";
    if (value == Mode::DualMode)  return "DualMode";

    return "CityAtSundown";
}

Mode ModeFromString(const std::string &value)
{
    if (value == "CityAtSundown") return Mode::CityAtSundownMode;
    if (value == "GlobalColorTick") return Mode::GlobalColorTickMode;
    if (value == "LightsOut") return Mode::LightsOutMode;
    if (value == "WakeUp") return Mode::WakeUpMode;
    if (value == "ColorPulse") return Mode::ColorPulseMode;
    if (value == "DiscoStrobo") return Mode::DiscoStroboMode;
    if (value == "SingleColor(Color)") return Mode::SingleColorColorMode;
    if (value == "LaurasPartymodus(Beta)") return Mode::LaurasPartymodusBetaMode;
    if (value == "KjeldPartyModus") return Mode::KjeldPartyModusMode;
    if (value == "TimExistentialDreadMode")  return Mode::TimExistentialDreadModusMode;
    if (value == "DualMode") return Mode::DualMode;

    return Mode::CityAtSundownMode;
}