#ifndef BURNT_WOOD_LAMP_CLOCKLAMP_H
#define BURNT_WOOD_LAMP_CLOCKLAMP_H

#include "./Lamp.h"
#include "../../colorModes/TemperatureMode.h"
#include "../../colorModes/ClockMode.h"
#include "../../colorModes/WeatherMode.h"

#include "../../colorModes/Pacifica.h"

class ClockLamp: public Lamp {

    
   static const uint16 LED_DELAY = 10; // Todo Update
  // static const uint16 CLOCK_NUM_LEDS_TOTAL = -1; // Todo Update
   static const uint16 CLOCK_RING_RIGHT_START_LED = 0; // Todo Update
   static const uint16 CLOCK_RING_RIGHT_NUM_LEDS = 23; // Todo Update
   static const uint16 CLOCK_BIG_RING_START_LED = 22; // Todo Update
   static const uint16 CLOCK_BIG_RING_NUM_LEDS = 75; // Todo Update
   static const uint16 CLOCK_S_START_LED = 97; // Todo Update
   static const uint16 CLOCK_S_NUM_LEDS = 65; // Todo Update
   //  const int16 CLOCK_S_PART_1_NUM_LEDS 33 // Todo Update
   //  const int16 CLOCK_S_PART_2_NUM_LEDS 32 // Todo Update
   static const uint16 CLOCK_RING_LEFT_START = 162; // Todo Update
   static const uint16 CLOCK_RING_LEFT_NUM_LEDS = 24; // Todo Update
   // static const String WEATHER_URL = 'https://obuymo6rdfdehomshnutqiyqd40mvmnf.lambda-url.eu-central-1.on.aws/';
   static const bool debugIsOn = false;
    std::vector<LedStripe*> ledStripes;
    std::vector<ColorMode*> colorModes;

    std::vector<int> colorModeDelays{0,0,0,0};

    public:
        int Update(CRGB* leds_out){

            // Update Delays
            for (int i = 0; i < colorModeDelays.size(); i++)
            {
                colorModeDelays[i] -= LED_DELAY;
            }

            printUpdate();
            for (int i = 0; i < ledStripes.size(); i++)
            {
                // Only Update when delay has passed
                int colorModeDelay = colorModeDelays[i];
                if(colorModeDelay <= 0){
                    LedStripe& ledStripe = *ledStripes[i];
                    ColorMode& colorMode = *colorModes[i];
                    int startLed = ledStripe.StartLed();
                    int ledCount = ledStripe.LedCount();

                    printUpdateStripeWithColorMode(ledStripe.Name(), colorMode.getName(), startLed, ledCount);

                    // todo does not work for ClockMode, TemperatureMode, WeatherMode
                    
                    int newColorModeDelay = colorMode.Update(leds_out, startLed, ledCount);
                    colorModeDelays[i] = newColorModeDelay;
                }
            }
            // todo use independent delays for each colormode update
            return LED_DELAY;
            
        };

        void printUpdate(){
            if(!debugIsOn){
                return;
            }
            Serial.print("ClockLamp - Update");
        }


        void printUpdateStripeWithColorMode(std::string ledStripleName, std::string colorModeName,int startLed,int ledCount ){

            if(!debugIsOn){
                return;
            }
            
            Serial.print("ClockLamp - Update LedStripe & ColorMode: ");
	        Serial.print("ledStripe: ");
            Serial.print(ledStripleName.c_str());
            Serial.print(", colorMode: ");
            Serial.print(colorModeName.c_str());
            Serial.print("startLed: ");
            Serial.print(startLed);
            Serial.print("ledCount: ");
            Serial.println(ledCount);
        } 
         
        ClockLamp(){
            ledStripes = LedStripes();
            colorModes = ColorModes();
        };

       std::vector<LedStripe*> LedStripes(){
            return {
                new LedStripe{"CLOCK_RING_RIGHT", CLOCK_RING_RIGHT_START_LED, CLOCK_RING_RIGHT_NUM_LEDS},
                new LedStripe{"CLOCK_BIG_RING", CLOCK_BIG_RING_START_LED, CLOCK_BIG_RING_NUM_LEDS},
                new LedStripe{"CLOCK_S", CLOCK_S_START_LED, CLOCK_S_NUM_LEDS},
                new LedStripe{"CLOCK_RING_LEFT", CLOCK_RING_LEFT_START, CLOCK_RING_LEFT_NUM_LEDS},
            };
        };
        std::vector<ColorMode*> ColorModes() {
            return {
                new TemperatureMode{},
                new ClockMode{}, 
                new Pacifica{},
                new WeatherMode{}, 
            };
        };

};

#endif