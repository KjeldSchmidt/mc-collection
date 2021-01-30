//
// Created by kjeld on 30/01/2021.
//

#ifndef BURNT_WOOD_LAMP_LAMPWEBSERVER_H
#define BURNT_WOOD_LAMP_LAMPWEBSERVER_H

#include <ESP8266WebServer.h>

class LampWebServer {
public:
	LampWebServer() = default;

	static void handleClient();

	static void initServer( int port = 80 );

private:
	static void registerHandlers();

	static void handleGenericRequest();

	static ESP8266WebServer *server;
};


#endif //BURNT_WOOD_LAMP_LAMPWEBSERVER_H
