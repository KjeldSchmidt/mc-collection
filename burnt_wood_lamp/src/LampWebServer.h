//
// Created by kjeld on 30/01/2021.
//

#ifndef BURNT_WOOD_LAMP_LAMPWEBSERVER_H
#define BURNT_WOOD_LAMP_LAMPWEBSERVER_H

#include <ESP8266WebServer.h>
#include <LightManager.h>

class LampWebServer {
public:
	explicit LampWebServer( LightManager *lightManager, ESP8266WebServer *server );

	void handleClient();

	void initServer();

	void colorFromPayload() const;

private:
	void registerHandlers();

	void setMode() const;

	void getModes() const;

	void getStatus() const;

	LightManager *lightManager;

	ESP8266WebServer *server;
};


#endif //BURNT_WOOD_LAMP_LAMPWEBSERVER_H
