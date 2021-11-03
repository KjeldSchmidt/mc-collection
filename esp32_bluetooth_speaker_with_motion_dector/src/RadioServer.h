//
// Created by kjeld on 03/11/2021.
//

#ifndef UNTITLED2_RADIOSERVER_H
#define UNTITLED2_RADIOSERVER_H

#include "../.pio/libdeps/wemos_d1_mini32/AutoConnect/src/AutoConnect.h"
#include <WebServer.h>
#include <WiFi.h>


class RadioServer {
public:
	RadioServer();

	void handleClient();

private:
	void rootPage();

	void startPage();

#if defined(ARDUINO_ARCH_ESP8266)
	ESP8266WebServer Server;
#elif defined(ARDUINO_ARCH_ESP32)
	WebServer Server;
#endif
	AutoConnect Portal;
	AutoConnectConfig Config;
	AutoConnectAux Timezone;
};


#endif //UNTITLED2_RADIOSERVER_H
