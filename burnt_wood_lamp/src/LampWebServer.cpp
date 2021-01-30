//
// Created by kjeld on 30/01/2021.
//

#include "LampWebServer.h"

ESP8266WebServer *LampWebServer::server;

void LampWebServer::registerHandlers() {
	server->on( "/request", LampWebServer::handleGenericRequest );
}

void LampWebServer::initServer( int port ) {
	server = new ESP8266WebServer( port );

	registerHandlers();

	server->begin();
}

void LampWebServer::handleGenericRequest() {
	server->send( 200, "text/plain", "Request was deemed acceptable" );
}

void LampWebServer::handleClient() {
	server->handleClient();
}
