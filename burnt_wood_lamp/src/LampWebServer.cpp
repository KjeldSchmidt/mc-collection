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
	bool success = lightManager->setMode( newModeName );
	if ( success ) {
		server->send( 200 );
	} else {
		server->send( 400 );
	}
}

void LampWebServer::handleClient() {
	server->handleClient();
}

void LampWebServer::getModes() {
	server->send( 200, "text/plain", LightManager::getModes());
}

LampWebServer::LampWebServer( LightManager *lightManager, ESP8266WebServer *server )
		: lightManager( lightManager ),
		  server( server ) {}
