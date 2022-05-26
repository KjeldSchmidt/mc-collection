//
// Created by kjeld on 30/01/2021.
//

#ifndef BURNT_WOOD_LAMP_LAMPWEBSERVER_H
#define BURNT_WOOD_LAMP_LAMPWEBSERVER_H

#include <ESP8266WebServer.h>
#include "led/LedManager.h"

class LampWebServer {
public:
	explicit LampWebServer( LightManager *lightManager, ESP8266WebServer *server );

	void handleClient();

	void initServer();

private:
	void registerHandlers();

	void setMode();

	void getModes();

	LightManager *lightManager;

	ESP8266WebServer *server;
};


#endif //BURNT_WOOD_LAMP_LAMPWEBSERVER_H
