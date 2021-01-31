//
// Created by kjeld on 30/01/2021.
//

#include "LampWebServer.h"

void LampWebServer::registerHandlers() {
	server->on( "/setMode", [ & ]() { this->setMode(); } );
	server->on( "/getModes", [ & ]() { this->getModes(); } );
}

void LampWebServer::initServer() {
	registerHandlers();
	server->begin();
}

void LampWebServer::setMode() {
	String newModeName = server->arg( "newMode" );
}

void LampWebServer::handleClient() {
	server->handleClient();
}

void LampWebServer::getModes() {
	server->send( 200, "text/plain", lightManager->getModes());
}

LampWebServer::LampWebServer( LightManager *lightManager, ESP8266WebServer *server )
		: lightManager( lightManager ),
		  server( server ) {}
