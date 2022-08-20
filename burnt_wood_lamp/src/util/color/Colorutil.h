#ifndef BURNT_WOOD_LAMP_COLORUTIL_H
#define BURNT_WOOD_LAMP_COLORUTIL_H

#include "FastLED.h"
#include "../../config/ConfigVariables.h"

class ColorUtil {
    public:
        static float clampCRGB(int n) {
            if(n < 0){
                return 0;
            }
            if(n > MAX_BRIGHTNESS){
                return MAX_BRIGHTNESS;
            }
            
            return n;
        }

        static float clampTemperature(int n, int8_t min, int8_t max) {
            if(n < min){
                return min;
            }
            if(n > max){
                return max;
            }
                
            return n;
        }

        static uint16 ModuloIndexOnLedStripe(int16_t index, int16_t ledStart, uint16 ledEnd){
            if(index < ledStart){
                int16_t diff = ledStart -index;
                index = ledEnd - diff;
            }
            else if (index > ledEnd) {
                uint16_t diff = index - ledEnd;
                index = ledStart + diff;
            }

            return index;
        }

        static CRGB DecreaseColorPerLed(CRGB color, uint16 offset, double factorPerLed, uint8 squareXTimes) {
            double colorMultiplier = ((double)offset+1) * factorPerLed;
            for (int8 i = 0; i < squareXTimes; i++){
                colorMultiplier = colorMultiplier * colorMultiplier;
            }
            uint8_t r = clampCRGB((double)color.r * colorMultiplier);
            uint8_t g = clampCRGB((double)color.g * colorMultiplier);
            uint8_t b = clampCRGB((double)color.b * colorMultiplier);
            return CRGB(r,g,b);
        }

        static CRGB IncreaseColorPerLed(CRGB color, uint16 offset, double factorPerLed, uint8 squareXTimes) {
            double colorMultiplier = (double)1 - (double)(offset * factorPerLed);
            for (int8 i = 0; i < squareXTimes; i++){
                colorMultiplier = colorMultiplier * colorMultiplier;
            }
            uint8_t r = clampCRGB((double)color.r * colorMultiplier);
            uint8_t g = clampCRGB((double)color.g * colorMultiplier);
            uint8_t b = clampCRGB((double)color.b * colorMultiplier);
            return CRGB(r,g,b);
        }

        static void SetSingleColor( CRGB *leds_out, CRGB color, uint16 startLed, uint16 endLed) {
            for ( uint16_t i = startLed; i < endLed; i++ ) {
                leds_out[i] = color;
            }
        }

        static CRGB TEST( ) {
            return CRGB(0,0,255);
        }

        static void SafeSetColor(CRGB *leds_out, uint16 index, CRGB color){
		if(index >= 0 && index < 10000){
		//	Serial.print("index: ");
		//	Serial.print(index);
		//	Serial.print(" color ");
		//	Serial.print(color.r);
		//	Serial.print(",");
		//	Serial.print(color.g);
		//	Serial.print(",");
		//	Serial.print(color.b);
		//	Serial.println();
			leds_out[index] = color;
		}
		else {
			Serial.println("------------------------INVALID INDEX--------------------------------------------");
			Serial.println(index);
			Serial.println("--------------------------------------------------------------------");
			return;
		}
	}

};

#endif