//
// Created by kjeld on 30/01/2021.
//

#ifndef BURNT_WOOD_LAMP_LAMPWEBSERVER_H
#define BURNT_WOOD_LAMP_LAMPWEBSERVER_H

#include <ESP8266WebServer.h>
#include <LightManager.h>

class LampWebServer {
public:
	explicit LampWebServer( LightManager *lightManager );

	void handleClient();

	void initServer( int port = 80 );

private:
	void registerHandlers();

	void setMode();

	void getModes();

	ESP8266WebServer *server;

	LightManager *lightManager;
};


#endif //BURNT_WOOD_LAMP_LAMPWEBSERVER_H
