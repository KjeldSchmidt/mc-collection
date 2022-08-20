//
// Created by kjeld on 31/01/2021.
//

#ifndef BURNT_WOOD_LAMP_CONFIGVARIABLES_H
#define BURNT_WOOD_LAMP_CONFIGVARIABLES_H

// Init
#define INITIAL_MODE Mode::PacificaMode;

// #define START_LED 0
// todo
 //#define NUM_LEDS 184
#define MAX_LEDS 300

// General Color
#define MAX_BRIGHTNESS 255
#define MAX_BRIGHTNESS_PER_CHANNEL 255
#define COLOR_ORDER GRB

// General MicroController
#define CHIPSET WS2812
#define DATA_PIN D5

// General Time
#define TIMEZONE "CET-1CEST,M3.5.0,M10.5.0/3" // Berlin -> https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
#define NTP_SERVER "de.pool.ntp.org" // TimeZone Server for NTP Protocol

#define HOURS 12
#define MINUTES 60

// Burnt Wood Lamp
#define NUM_LEDS_LOWER 55
#define NUM_LEDS_UPPER 54

#endif //BURNT_WOOD_LAMP_CONFIGVARIABLES_H
