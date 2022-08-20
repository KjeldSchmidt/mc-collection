#ifndef BURNT_WOOD_LAMP_FETCHUTIL_H
#define BURNT_WOOD_LAMP_FETCHUTIL_H

#include "../../colorModes/ColorMode.h"
#include "NTPClient.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

#include <string>
#include <iostream>

class FetchUtil {
    public:
		static bool ShouldFetch(bool debugIsOn, int cachedCount, int loopsSinceLastFetch, int loopsUntilNextFetch){
			bool isCached = cachedCount > 0;
			bool loopsElapsedExceededThreshold = loopsSinceLastFetch > loopsUntilNextFetch;
			bool shouldFetch = !isCached || loopsElapsedExceededThreshold;
			if(debugIsOn){
				Serial.print("ShouldFetchTemperatures: ");
				Serial.print("cachedCount: ");
				Serial.print(cachedCount);
				Serial.print(", isCached: ");
				Serial.print(isCached);
				Serial.print(", loopsSinceLastFetch: ");
				Serial.print(loopsSinceLastFetch);
				Serial.print(", loopsElapsedExceededThreshold: ");
				Serial.print(loopsElapsedExceededThreshold);
				Serial.print(", loopsUntilNextFetch: ");
				Serial.print(loopsUntilNextFetch);
				Serial.print(", shouldFetch: ");
				Serial.print(shouldFetch);
				Serial.println();
			}
			return shouldFetch;
		};
};


#endif